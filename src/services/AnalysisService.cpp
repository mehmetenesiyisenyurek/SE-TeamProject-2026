#include "AnalysisService.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>

std::string severityToText(DiagnosticSeverity severity) {
    switch (severity) {
        case DiagnosticSeverity::CRITICAL: return "CRITICAL";
        case DiagnosticSeverity::WARNING:  return "WARNING";
        case DiagnosticSeverity::INFO:     return "INFO";
        default:                           return "UNKNOWN";
    }
}

namespace {

struct VarInfo {
    std::string name;
    int line = 1;
    bool pointer = false;
    bool initialized = false;
    bool structField = false;
};

static std::string trim(const std::string& s) {
    size_t b = 0;
    while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    size_t e = s.size();
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

static std::string baseName(const std::string& path) {
    size_t p = path.find_last_of("/\\");
    if (p == std::string::npos) return path;
    return path.substr(p + 1);
}

static long fileSizeOf(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    return f.is_open() ? static_cast<long>(f.tellg()) : 0;
}

static std::string readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) throw std::runtime_error("Dosya acilamadi: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static std::vector<std::string> splitLines(const std::string& code) {
    std::vector<std::string> lines;
    std::stringstream ss(code);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    if (code.empty() || (!code.empty() && code.back() == '\n')) {
        // getline son bos satiri uretmez; metrik icin sorun degil.
    }
    return lines;
}

static bool isIdentChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

static bool containsWord(const std::string& line, const std::string& word) {
    size_t pos = line.find(word);
    while (pos != std::string::npos) {
        bool left = pos == 0 || !isIdentChar(line[pos - 1]);
        bool right = pos + word.size() >= line.size() || !isIdentChar(line[pos + word.size()]);
        if (left && right) return true;
        pos = line.find(word, pos + 1);
    }
    return false;
}

static std::string removeCommentsKeepStrings(const std::string& code, int& commentLines) {
    std::string out;
    out.reserve(code.size());
    bool inString = false, inChar = false, inSingle = false, inMulti = false, escape = false;
    bool commentOnThisLine = false;
    commentLines = 0;

    for (size_t i = 0; i < code.size(); ++i) {
        char c = code[i];
        char n = (i + 1 < code.size()) ? code[i + 1] : '\0';

        if (inSingle) {
            if (c == '\n') {
                if (commentOnThisLine) ++commentLines;
                commentOnThisLine = false;
                inSingle = false;
                out += '\n';
            } else {
                out += ' ';
            }
            continue;
        }
        if (inMulti) {
            if (c == '\n') {
                if (commentOnThisLine) ++commentLines;
                commentOnThisLine = true;
                out += '\n';
            } else if (c == '*' && n == '/') {
                out += "  ";
                ++i;
                inMulti = false;
            } else {
                out += ' ';
            }
            continue;
        }

        if (escape) { out += c; escape = false; continue; }
        if ((inString || inChar) && c == '\\') { out += c; escape = true; continue; }
        if (!inChar && c == '"') { inString = !inString; out += c; continue; }
        if (!inString && c == '\'') { inChar = !inChar; out += c; continue; }

        if (!inString && !inChar && c == '/' && n == '/') {
            inSingle = true;
            commentOnThisLine = true;
            out += "  ";
            ++i;
            continue;
        }
        if (!inString && !inChar && c == '/' && n == '*') {
            inMulti = true;
            commentOnThisLine = true;
            out += "  ";
            ++i;
            continue;
        }
        out += c;
    }
    if ((inSingle || inMulti) && commentOnThisLine) ++commentLines;
    return out;
}

static std::string stripStringLiterals(const std::string& line) {
    std::string out = line;
    bool inString = false, inChar = false, escape = false;
    for (char& c : out) {
        if (escape) { c = ' '; escape = false; continue; }
        if ((inString || inChar) && c == '\\') { c = ' '; escape = true; continue; }
        if (!inChar && c == '"') { inString = !inString; c = ' '; continue; }
        if (!inString && c == '\'') { inChar = !inChar; c = ' '; continue; }
        if (inString || inChar) c = ' ';
    }
    return out;
}

static void addDiag(std::vector<Diagnostic>& diags, int line, int col, const std::string& msg,
                    DiagnosticSeverity sev, const std::string& source, const std::string& rule,
                    const std::string& snippet) {
    diags.emplace_back(line, col > 0 ? col : 1, msg, sev, source, rule, trim(snippet));
}


static bool isControlStatementStart(const std::string& t) {
    return t.rfind("if", 0) == 0 ||
           t.rfind("while", 0) == 0 ||
           t.rfind("for", 0) == 0 ||
           t.rfind("switch", 0) == 0 ||
           t.rfind("else", 0) == 0 ||
           t.rfind("do", 0) == 0;
}

static bool isFunctionDefinitionHeader(const std::vector<std::string>& lines, size_t index) {
    if (index >= lines.size()) return false;

    std::string t = trim(stripStringLiterals(lines[index]));
    if (t.empty()) return false;
    if (t.back() == ';') return false;          // Prototip: int f();
    if (isControlStatementStart(t)) return false;

    // int main(), void foo(int x), char *name(...) gibi fonksiyon başlıkları
    std::regex funcHeader(R"(^\s*(?:static\s+|inline\s+|extern\s+|const\s+|unsigned\s+|signed\s+)*(?:struct\s+\w+|int|char|float|double|void|long|short|bool|size_t)\s+\*?\s*[A-Za-z_]\w*\s*\([^;]*\)\s*$)");
    if (!std::regex_search(t, funcHeader)) return false;

    // Açılış süslü parantez aynı satırda veya bir sonraki boş olmayan satırda olabilir.
    if (t.find('{') != std::string::npos) return true;
    for (size_t j = index + 1; j < lines.size(); ++j) {
        std::string next = trim(stripStringLiterals(lines[j]));
        if (next.empty()) continue;
        return !next.empty() && next[0] == '{';
    }

    return false;
}

static bool isLineThatDoesNotNeedSemicolon(const std::vector<std::string>& lines, size_t index) {
    std::string t = trim(stripStringLiterals(lines[index]));
    if (t.empty()) return true;
    if (t[0] == '#') return true;
    if (t.back() == ';' || t.back() == '{' || t.back() == '}' || t.back() == ':') return true;
    if (isControlStatementStart(t)) return true;
    if (isFunctionDefinitionHeader(lines, index)) return true;
    return false;
}

static bool isTypeStart(const std::string& s) {
    static const std::vector<std::string> types = {
        "int", "char", "float", "double", "long", "short", "void", "bool", "size_t"
    };
    std::string t = trim(s);
    for (const auto& ty : types) {
        if (containsWord(t, ty)) return true;
    }
    return t.find("struct ") != std::string::npos;
}

static std::vector<VarInfo> parseDeclarations(const std::vector<std::string>& lines) {
    std::vector<VarInfo> vars;
    bool inStruct = false;
    int braceDepth = 0;

    std::regex declRegex(R"(^\s*(?:const\s+|unsigned\s+|signed\s+|static\s+|register\s+|volatile\s+)*(?:struct\s+\w+|int|char|float|double|long|short|bool|size_t|void)\s+(.+);)");
    std::regex nameRegex(R"((\*?\s*)([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(=\s*[^,;]+)?)");

    for (size_t i = 0; i < lines.size(); ++i) {
        std::string line = stripStringLiterals(lines[i]);
        std::string t = trim(line);

        if (std::regex_search(t, std::regex(R"(^struct\s+\w*\s*\{)"))) {
            inStruct = true;
        }
        for (char c : t) {
            if (c == '{') ++braceDepth;
            else if (c == '}') {
                if (braceDepth > 0) --braceDepth;
                if (inStruct && braceDepth == 0) inStruct = false;
            }
        }

        if (t.find('(') != std::string::npos && t.find(';') == std::string::npos) continue; // fonksiyon imzasi
        std::smatch m;
        if (!std::regex_search(line, m, declRegex)) continue;
        std::string rest = m[1];
        if (rest.find('(') != std::string::npos) continue;

        std::stringstream ss(rest);
        std::string part;
        while (std::getline(ss, part, ',')) {
            part = trim(part);
            if (part.empty()) continue;
            std::smatch nm;
            if (std::regex_search(part, nm, nameRegex)) {
                VarInfo v;
                v.name = nm[2];
                v.line = static_cast<int>(i + 1);
                v.pointer = part.find('*') != std::string::npos;
                v.initialized = part.find('=') != std::string::npos;
                v.structField = inStruct;
                if (!v.name.empty() && v.name != "return") vars.push_back(v);
            }
        }
    }
    return vars;
}

static int columnOf(const std::string& line, const std::string& text) {
    size_t p = line.find(text);
    return p == std::string::npos ? 1 : static_cast<int>(p + 1);
}

static std::vector<CodeMetric> calculateMetrics(const std::vector<std::string>& rawLines,
                                                const std::vector<std::string>& cleanLines,
                                                int commentLines,
                                                const std::vector<VarInfo>& vars) {
    int total = static_cast<int>(rawLines.size());
    int empty = 0, code = 0, funcs = 0, structs = 0;
    std::regex funcRegex(R"(^\s*(?:int|char|float|double|void|long|short|bool|size_t|struct\s+\w+)\s+\*?\s*([A-Za-z_]\w*)\s*\([^;]*\)\s*\{?)");
    std::regex structRegex(R"(^\s*struct\s+\w+\s*\{)");
    std::vector<int> funcStart;

    for (size_t i = 0; i < cleanLines.size(); ++i) {
        std::string t = trim(cleanLines[i]);
        if (trim(rawLines[i]).empty()) ++empty;
        if (!t.empty()) ++code;
        if (std::regex_search(t, funcRegex) && t.find("if") != 0 && t.find("while") != 0 && t.find("for") != 0) {
            ++funcs;
            funcStart.push_back(static_cast<int>(i + 1));
        }
        if (std::regex_search(t, structRegex)) ++structs;
    }

    double avgLen = 0.0;
    if (!funcStart.empty()) {
        int sum = 0;
        for (size_t i = 0; i < funcStart.size(); ++i) {
            int start = funcStart[i];
            int end = (i + 1 < funcStart.size()) ? funcStart[i + 1] - 1 : total;
            sum += std::max(1, end - start + 1);
        }
        avgLen = static_cast<double>(sum) / funcStart.size();
    }

    int nonStructVars = 0;
    for (const auto& v : vars) if (!v.structField) ++nonStructVars;

    return {
        CodeMetric("Toplam Satir Sayisi", total, "satir", "Dosyadaki toplam satir sayisi"),
        CodeMetric("Kod Satiri", code, "satir", "Bos olmayan ve yorum olmayan kod satiri sayisi"),
        CodeMetric("Bos Satir", empty, "satir", "Tamamen bos satir sayisi"),
        CodeMetric("Yorum Satiri", commentLines, "satir", "Yorum satiri sayisi"),
        CodeMetric("Fonksiyon Sayisi", funcs, "adet", "Tespit edilen fonksiyon sayisi"),
        CodeMetric("Struct Sayisi", structs, "adet", "Tespit edilen struct sayisi"),
        CodeMetric("Degisken Sayisi", nonStructVars, "adet", "Struct alanlari haric degisken sayisi"),
        CodeMetric("Ort. Fonksiyon Uzunlugu", avgLen, "satir", "Fonksiyonlarin ortalama satir uzunlugu")
    };
}

static bool hasNullCheck(const std::vector<std::string>& lines, const std::string& var, int fromLine, int toLine) {
    std::regex r1("if\\s*\\([^)]*(" + var + ")\\s*(==|!=)\\s*(NULL|nullptr|0)[^)]*\\)");
    std::regex r2("if\\s*\\(\\s*!\\s*" + var + "\\s*\\)");
    for (int i = std::max(1, fromLine); i <= std::min<int>(toLine, lines.size()); ++i) {
        std::string s = stripStringLiterals(lines[i - 1]);
        if (std::regex_search(s, r1) || std::regex_search(s, r2)) return true;
    }
    return false;
}

static bool isAssignmentTo(const std::string& line, const std::string& var) {
    std::regex r("\\b" + var + "\\b\\s*=");
    return std::regex_search(line, r);
}

static bool isDereferenced(const std::string& line, const std::string& var) {
    std::string s = stripStringLiterals(line);
    std::regex d1("\\*\\s*" + var + "\\b");
    std::regex d2("\\b" + var + "\\s*\\[");
    std::regex d3("\\b" + var + "\\s*->");
    return std::regex_search(s, d1) || std::regex_search(s, d2) || std::regex_search(s, d3);
}

static bool isFreeCall(const std::string& line, std::string& var) {
    std::smatch m;
    std::regex r(R"(\bfree\s*\(\s*([A-Za-z_]\w*)\s*\))");
    if (std::regex_search(line, m, r)) { var = m[1]; return true; }
    return false;
}

static bool isAllocLine(const std::string& line, std::string& var) {
    std::smatch m;
    std::regex r1(R"(\b([A-Za-z_]\w*)\s*=\s*(?:\([^)]*\)\s*)?(malloc|calloc|realloc)\s*\()");
    if (std::regex_search(line, m, r1)) { var = m[1]; return true; }
    return false;
}

static void runRules(AnalysisOutput& output, const std::vector<std::string>& rawLines,
                     const std::vector<std::string>& cleanLines, const std::vector<VarInfo>& vars) {
    auto& diags = output.diagnostics;
    std::map<std::string, VarInfo> varMap;
    for (const auto& v : vars) if (!v.structField) varMap[v.name] = v;

    // Parser/syntax temel kontrolleri: parantez dengesi ve eksik noktalı virgül.
    std::vector<std::pair<char, int>> stack;
    for (size_t i = 0; i < cleanLines.size(); ++i) {
        const std::string line = stripStringLiterals(cleanLines[i]);
        for (char c : line) {
            if (c == '(' || c == '{' || c == '[') stack.push_back({c, static_cast<int>(i + 1)});
            else if (c == ')' || c == '}' || c == ']') {
                if (stack.empty()) {
                    addDiag(diags, static_cast<int>(i + 1), 1, "Eslesmeyen kapatma parantezi.", DiagnosticSeverity::CRITICAL, "syntax", "", rawLines[i]);
                } else {
                    char o = stack.back().first;
                    bool ok = (o == '(' && c == ')') || (o == '{' && c == '}') || (o == '[' && c == ']');
                    if (ok) stack.pop_back();
                    else addDiag(diags, static_cast<int>(i + 1), 1, "Eslesmeyen parantez/suslu parantez.", DiagnosticSeverity::CRITICAL, "syntax", "", rawLines[i]);
                }
            }
        }
        std::string t = trim(line);
        if (!isLineThatDoesNotNeedSemicolon(cleanLines, i)) {
            bool statementLike =
                isTypeStart(t) ||
                t.find('=') != std::string::npos ||
                t.rfind("return", 0) == 0 ||
                std::regex_search(t, std::regex("\\b[A-Za-z_]\\w*\\s*\\("));

            if (statementLike) {
                addDiag(diags, static_cast<int>(i + 1), static_cast<int>(t.size()), "Eksik noktali virgul olabilir.", DiagnosticSeverity::CRITICAL, "syntax", "", rawLines[i]);
            }
        }
    }
    for (auto& p : stack) {
        addDiag(diags, p.second, 1, "Kapatilmamis parantez/suslu parantez.", DiagnosticSeverity::CRITICAL, "syntax", "", rawLines[p.second - 1]);
    }

    // R001 kullanilmayan degisken; struct alanlarini sayma.
    for (const auto& v : vars) {
        if (v.structField) continue;
        int uses = 0;
        for (size_t i = 0; i < cleanLines.size(); ++i) {
            if (static_cast<int>(i + 1) == v.line) {
                std::string afterDecl = cleanLines[i];
                size_t pos = afterDecl.find(v.name);
                if (pos != std::string::npos) afterDecl.erase(pos, v.name.size());
                if (containsWord(afterDecl, v.name)) ++uses;
            } else if (containsWord(cleanLines[i], v.name)) ++uses;
        }
        if (uses == 0) {
            addDiag(diags, v.line, columnOf(rawLines[v.line - 1], v.name),
                    "Degisken '" + v.name + "' tanimlanmis ancak hic kullanilmamis.",
                    DiagnosticSeverity::WARNING, "rule", "R001", rawLines[v.line - 1]);
        }
    }

    // R002/R012 baslatilmamis degisken/pointer ilk kullanim.
    std::map<std::string, bool> initialized;
    for (const auto& kv : varMap) initialized[kv.first] = kv.second.initialized;
    for (size_t i = 0; i < cleanLines.size(); ++i) {
        std::string line = stripStringLiterals(cleanLines[i]);
        int lineNo = static_cast<int>(i + 1);
        for (auto& kv : initialized) {
            const std::string& name = kv.first;
            const VarInfo& v = varMap[name];
            if (lineNo < v.line) continue;
            if (lineNo == v.line) continue; // tanim satirinda kullanma sayma
            if (isAssignmentTo(line, name)) { kv.second = true; continue; }
            if (!kv.second && containsWord(line, name)) {
                if (v.pointer && isDereferenced(line, name)) {
                    addDiag(diags, lineNo, columnOf(rawLines[i], name),
                            "Pointer '" + name + "' baslatilmadan dereference ediliyor.",
                            DiagnosticSeverity::CRITICAL, "rule", "R012", rawLines[i]);
                } else if (!v.pointer) {
                    addDiag(diags, lineNo, columnOf(rawLines[i], name),
                            "Degisken '" + name + "' baslatilmadan kullaniliyor.",
                            DiagnosticSeverity::WARNING, "rule", "R002", rawLines[i]);
                }
                kv.second = true; // ayni degisken icin rapor spamini azalt.
            }
        }
    }

    // R003 magic number
    std::regex numberRegex(R"(\b(\d+)(?:\.\d+)?\b)");
    for (size_t i = 0; i < cleanLines.size(); ++i) {
        std::string line = stripStringLiterals(cleanLines[i]);
        if (line.find("#include") != std::string::npos) continue;
        for (std::sregex_iterator it(line.begin(), line.end(), numberRegex), end; it != end; ++it) {
            std::string n = (*it)[1];
            if (n == "0" || n == "1") continue;
            addDiag(diags, static_cast<int>(i + 1), static_cast<int>(it->position() + 1),
                    "Magic number '" + n + "' tespit edildi. Sabit olarak tanimlayin.",
                    DiagnosticSeverity::WARNING, "rule", "R003", rawLines[i]);
        }
    }

    // R004 fonksiyon uzunlugu, R005 goto, R006 infinite loop.
    std::regex funcRegex(R"(^\s*(?:int|char|float|double|void|long|short|bool|size_t|struct\s+\w+)\s+\*?\s*([A-Za-z_]\w*)\s*\([^;]*\)\s*\{?)");
    for (size_t i = 0; i < cleanLines.size(); ++i) {
        std::string t = trim(stripStringLiterals(cleanLines[i]));
        if (containsWord(t, "goto")) {
            addDiag(diags, static_cast<int>(i + 1), columnOf(rawLines[i], "goto"), "'goto' kullanimi tespit edildi.", DiagnosticSeverity::WARNING, "rule", "R005", rawLines[i]);
        }
        if (std::regex_search(t, std::regex(R"(while\s*\(\s*(1|true)\s*\))")) || std::regex_search(t, std::regex(R"(for\s*\(\s*;\s*;\s*\))"))) {
            addDiag(diags, static_cast<int>(i + 1), 1, "Potansiyel sonsuz dongu tespit edildi.", DiagnosticSeverity::WARNING, "rule", "R006", rawLines[i]);
        }
        if (std::regex_search(t, funcRegex)) {
            int depth = 0, endLine = static_cast<int>(i + 1);
            bool started = false;
            for (size_t j = i; j < cleanLines.size(); ++j) {
                for (char c : cleanLines[j]) {
                    if (c == '{') { ++depth; started = true; }
                    else if (c == '}') --depth;
                }
                if (started && depth <= 0) { endLine = static_cast<int>(j + 1); break; }
            }
            int len = endLine - static_cast<int>(i + 1) + 1;
            if (len > 50) {
                addDiag(diags, static_cast<int>(i + 1), 1, "Fonksiyon " + std::to_string(len) + " satir. Daha kucuk fonksiyonlara bolun.", DiagnosticSeverity::WARNING, "rule", "R004", rawLines[i]);
            }
        }
    }

    // Pointer memory rules.
    std::map<std::string, int> allocLine;
    std::map<std::string, int> freeLine;
    std::set<std::string> freed;
    std::set<std::string> reportedUaf;

    for (size_t i = 0; i < cleanLines.size(); ++i) {
        int lineNo = static_cast<int>(i + 1);
        std::string line = stripStringLiterals(cleanLines[i]);
        std::string var;
        if (isAllocLine(line, var)) {
            allocLine[var] = lineNo;
            freed.erase(var);
            freeLine.erase(var);
            if (!hasNullCheck(cleanLines, var, lineNo, std::min<int>(lineNo + 6, cleanLines.size()))) {
                addDiag(diags, lineNo, columnOf(rawLines[i], var),
                        "'" + var + "' icin malloc/calloc/realloc sonrasi NULL kontrolu yapilmamis.",
                        DiagnosticSeverity::WARNING, "rule", "R007", rawLines[i]);
            }
        }
        if (isFreeCall(line, var)) {
            if (freed.count(var)) {
                addDiag(diags, lineNo, columnOf(rawLines[i], var),
                        "'" + var + "' zaten satir " + std::to_string(freeLine[var]) + " icinde free edilmis. Cift free tespit edildi.",
                        DiagnosticSeverity::CRITICAL, "rule", "R010", rawLines[i]);
            }
            freed.insert(var);
            freeLine[var] = lineNo;
        }

        std::vector<std::string> toRemoveFromFreed;
        for (const auto& f : freed) {
            if (lineNo <= freeLine[f]) continue;
            if (isAssignmentTo(line, f) && (line.find("NULL") != std::string::npos || line.find("nullptr") != std::string::npos || line.find("= 0") != std::string::npos)) {
                // NULL atamasi dangling'i giderir, UAF kontrolunu de sonlandirir.
                toRemoveFromFreed.push_back(f);
                continue;
            }
            if (!reportedUaf.count(f) && containsWord(line, f) && line.find("free") == std::string::npos) {
                addDiag(diags, lineNo, columnOf(rawLines[i], f),
                        "'" + f + "' free edildikten sonra kullaniliyor.",
                        DiagnosticSeverity::CRITICAL, "rule", "R008", rawLines[i]);
                reportedUaf.insert(f);
            }
        }
        for (const auto& f : toRemoveFromFreed) freed.erase(f);
    }

    // R011 dangling: free sonrasi hemen NULL atanmayan pointer.
    for (const auto& kv : freeLine) {
        const std::string& var = kv.first;
        int fl = kv.second;
        bool nullAfter = false;
        for (int i = fl + 1; i <= std::min<int>(fl + 2, cleanLines.size()); ++i) {
            std::string line = stripStringLiterals(cleanLines[i - 1]);
            if (std::regex_search(line, std::regex("\\b" + var + "\\b\\s*=\\s*(NULL|nullptr|0)"))) nullAfter = true;
        }
        if (!nullAfter) {
            addDiag(diags, fl, columnOf(rawLines[fl - 1], var),
                    "'" + var + "' free edildikten sonra NULL atanmamis.",
                    DiagnosticSeverity::INFO, "rule", "R011", rawLines[fl - 1]);
        }
    }

    // R009 leak: allocate var not freed and not returned.
    for (const auto& kv : allocLine) {
        const std::string& var = kv.first;
        if (freeLine.count(var)) continue;
        bool returned = false;
        for (const auto& l : cleanLines) {
            std::string line = stripStringLiterals(l);
            if (std::regex_search(line, std::regex("\\breturn\\s+" + var + "\\b"))) returned = true;
        }
        if (!returned) {
            int al = kv.second;
            addDiag(diags, al, columnOf(rawLines[al - 1], var),
                    "'" + var + "' icin bellek ayrilmis ancak free edilmemis.",
                    DiagnosticSeverity::WARNING, "rule", "R009", rawLines[al - 1]);
        }
    }

    // tekrar eden ayni rule/line/message kayitlarini temizle.
    std::vector<Diagnostic> unique;
    std::set<std::string> seen;
    for (const auto& d : diags) {
        std::string key = d.getRuleId() + "|" + std::to_string(d.getLine()) + "|" + d.getMessage();
        if (seen.insert(key).second) unique.push_back(d);
    }
    diags.swap(unique);
}

} // namespace

AnalysisOutput AnalysisService::analyzeFile(const std::string& filePath) const {
    AnalysisOutput output;
    output.filePath = filePath;
    output.fileName = baseName(filePath);
    output.ruleCount = 12;

    try {
        output.rawCode = readFile(filePath);
        output.fileSize = fileSizeOf(filePath);

        int commentLines = 0;
        output.cleanCode = removeCommentsKeepStrings(output.rawCode, commentLines);
        output.commentLineCount = commentLines;

        std::vector<std::string> rawLines = splitLines(output.rawCode);
        std::vector<std::string> cleanLines = splitLines(output.cleanCode);
        if (cleanLines.size() < rawLines.size()) cleanLines.resize(rawLines.size());
        if (rawLines.empty()) throw std::runtime_error("Dosya icerigi bos.");

        std::vector<VarInfo> vars = parseDeclarations(cleanLines);
        output.metrics = calculateMetrics(rawLines, cleanLines, output.commentLineCount, vars);
        runRules(output, rawLines, cleanLines, vars);

        output.success = true;
        return output;
    }
    catch (const std::exception& ex) {
        output.success = false;
        output.errorMessage = ex.what();
        return output;
    }
}
