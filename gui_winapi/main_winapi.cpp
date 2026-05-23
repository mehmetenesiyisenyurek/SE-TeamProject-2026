#include <windows.h>
#include <commdlg.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cwchar>

#include "services/AnalysisService.h"

#define ID_BTN_SELECT     1001
#define ID_BTN_ANALYZE    1002
#define ID_EDIT_PATH      1003
#define ID_EDIT_CODE      1004
#define ID_EDIT_RESULT    1005
#define ID_BTN_SAVE_HTML  1006
#define ID_BTN_SAVE_JSON  1007
#define ID_BTN_SAVE_TXT   1008
#define ID_CHK_CRITICAL   1009
#define ID_CHK_WARNING    1010
#define ID_CHK_INFO       1011
#define ID_STATIC_FILTER  1012
#define ID_STATIC_TOTAL   1013
#define ID_STATIC_CRIT    1014
#define ID_STATIC_WARN    1015
#define ID_STATIC_INFO    1016
#define ID_STATIC_BRAND   1017
#define ID_STATIC_SUBTITLE 1018

static HWND g_pathEdit = nullptr;
static HWND g_codeEdit = nullptr;
static HWND g_resultEdit = nullptr;
static HWND g_selectButton = nullptr;
static HWND g_analyzeButton = nullptr;
static HWND g_htmlButton = nullptr;
static HWND g_jsonButton = nullptr;
static HWND g_txtButton = nullptr;
static HWND g_criticalCheck = nullptr;
static HWND g_warningCheck = nullptr;
static HWND g_infoCheck = nullptr;
static HWND g_filterLabel = nullptr;
static HWND g_totalCard = nullptr;
static HWND g_criticalCard = nullptr;
static HWND g_warningCard = nullptr;
static HWND g_infoCard = nullptr;
static HWND g_brandTitle = nullptr;
static HWND g_brandSubtitle = nullptr;

static std::string g_selectedPath;
static AnalysisOutput g_lastOutput;
static bool g_hasLastOutput = false;
static bool g_showCritical = true;
static bool g_showWarning = true;
static bool g_showInfo = true;

static HFONT g_font = nullptr;
static HFONT g_titleFont = nullptr;
static HFONT g_cardFont = nullptr;
static HFONT g_monoFont = nullptr;
static HFONT g_brandFont = nullptr;
static HFONT g_subtitleFont = nullptr;
static HBRUSH g_bgBrush = nullptr;
static HBRUSH g_panelBrush = nullptr;
static HBRUSH g_editBrush = nullptr;

static const COLORREF COLOR_BG = RGB(18, 24, 38);
static const COLORREF COLOR_PANEL = RGB(28, 37, 55);
static const COLORREF COLOR_EDIT = RGB(12, 18, 30);
static const COLORREF COLOR_TEXT = RGB(229, 236, 246);
static const COLORREF COLOR_MUTED = RGB(165, 176, 194);
static const COLORREF COLOR_CRITICAL = RGB(255, 95, 95);
static const COLORREF COLOR_WARNING = RGB(255, 193, 7);
static const COLORREF COLOR_INFO = RGB(88, 166, 255);

std::wstring toWide(const std::string& text) {
    if (text.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    if (size <= 0) {
        size = MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, nullptr, 0);
        std::wstring wide(size, L'\0');
        MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, &wide[0], size);
        if (!wide.empty() && wide.back() == L'\0') wide.pop_back();
        return wide;
    }
    std::wstring wide(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wide[0], size);
    if (!wide.empty() && wide.back() == L'\0') wide.pop_back();
    return wide;
}

std::string toUtf8(const std::wstring& text) {
    if (text.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, &result[0], size, nullptr, nullptr);
    if (!result.empty() && result.back() == '\0') result.pop_back();
    return result;
}

void setText(HWND hwnd, const std::string& text) {
    SetWindowTextW(hwnd, toWide(text).c_str());
}

void setTextW(HWND hwnd, const std::wstring& text) {
    SetWindowTextW(hwnd, text.c_str());
}

void setEditText(HWND hwnd, const std::string& text) {
    if (!hwnd) return;

    std::wstring wide = toWide(text);

    // EDIT kontrolu her filtre/analiz degisiminde tamamen temizlenip yeniden cizilir.
    // Boylece eski yazi ile yeni yazi ust uste binmez.
    SendMessageW(hwnd, WM_SETREDRAW, FALSE, 0);
    SetWindowTextW(hwnd, L"");
    SetWindowTextW(hwnd, wide.c_str());
    SendMessageW(hwnd, EM_SETSEL, 0, 0);
    SendMessageW(hwnd, EM_SCROLLCARET, 0, 0);
    SendMessageW(hwnd, WM_SETREDRAW, TRUE, 0);

    InvalidateRect(hwnd, nullptr, TRUE);
    UpdateWindow(hwnd);
}

void applyFont(HWND hwnd, HFONT font = nullptr) {
    SendMessageW(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(font ? font : g_font), TRUE);
}

std::string normalizeLineEndings(const std::string& text) {
    std::string result;
    result.reserve(text.size() + 32);
    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text[i];
        if (ch == '\r') {
            if (i + 1 < text.size() && text[i + 1] == '\n') {
                result += "\r\n";
                ++i;
            } else {
                result += "\r\n";
            }
        } else if (ch == '\n') {
            result += "\r\n";
        } else {
            result += ch;
        }
    }
    return result;
}

std::string formatCodeForScreen(const std::string& code) {
    // Normal dosyalarda sadece \r\n duzeltmesi yeterli.
    // Tek satira dusmus test kodlarinda temel C bicimlendirmesi yapar.
    std::string normalized = normalizeLineEndings(code);
    if (normalized.find("\r\n") != std::string::npos) return normalized;

    std::string out;
    int indent = 0;
    bool inString = false, inChar = false, escape = false;

    auto addNewLine = [&]() {
        while (!out.empty() && (out.back() == ' ' || out.back() == '\t')) out.pop_back();
        out += "\r\n";
        for (int i = 0; i < indent; ++i) out += "    ";
    };

    for (size_t i = 0; i < normalized.size(); ++i) {
        char ch = normalized[i];
        if (escape) { out += ch; escape = false; continue; }
        if (ch == '\\' && (inString || inChar)) { out += ch; escape = true; continue; }
        if (ch == '"' && !inChar) inString = !inString;
        if (ch == '\'' && !inString) inChar = !inChar;

        if (!inString && !inChar) {
            if (ch == '#' && !out.empty()) { addNewLine(); out += ch; continue; }
            if (ch == '{') { out += " {"; ++indent; addNewLine(); continue; }
            if (ch == '}') {
                if (indent > 0) --indent;
                addNewLine();
                while (out.size() >= 4 && out.substr(out.size() - 4) == "    ") out.erase(out.size() - 4);
                out += "}";
                if (i + 1 < normalized.size() && normalized[i + 1] != ';') addNewLine();
                continue;
            }
            if (ch == ';') { out += ch; addNewLine(); continue; }
        }
        out += ch;
    }
    return out;
}

bool matchesFilter(const Diagnostic& d) {
    if (d.getSeverity() == DiagnosticSeverity::CRITICAL) return g_showCritical;
    if (d.getSeverity() == DiagnosticSeverity::WARNING) return g_showWarning;
    if (d.getSeverity() == DiagnosticSeverity::INFO) return g_showInfo;
    return true;
}

AnalysisOutput filteredOutput() {
    AnalysisOutput out = g_lastOutput;
    out.diagnostics.clear();
    for (const Diagnostic& d : g_lastOutput.diagnostics) {
        if (matchesFilter(d)) out.diagnostics.push_back(d);
    }
    return out;
}

std::string currentFilterName() {
    std::vector<std::string> parts;
    if (g_showCritical) parts.push_back("CRITICAL");
    if (g_showWarning) parts.push_back("WARNING");
    if (g_showInfo) parts.push_back("INFO");

    if (parts.empty()) return "HICBIRI";
    if (parts.size() == 3) return "TUMU";

    std::ostringstream out;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) out << " + ";
        out << parts[i];
    }
    return out.str();
}

void updateFilterLabel() {
    if (!g_filterLabel) return;
    setText(g_filterLabel, "Gosterilen: " + currentFilterName());
}

int countSeverity(DiagnosticSeverity severity) {
    int count = 0;
    if (!g_hasLastOutput) return 0;
    for (const Diagnostic& d : g_lastOutput.diagnostics) {
        if (d.getSeverity() == severity) ++count;
    }
    return count;
}

void updateCards() {
    int total = g_hasLastOutput ? static_cast<int>(g_lastOutput.diagnostics.size()) : 0;
    int critical = countSeverity(DiagnosticSeverity::CRITICAL);
    int warning = countSeverity(DiagnosticSeverity::WARNING);
    int info = countSeverity(DiagnosticSeverity::INFO);

    setText(g_totalCard, "TOTAL\r\n" + std::to_string(total));
    setText(g_criticalCard, "CRITICAL\r\n" + std::to_string(critical));
    setText(g_warningCard, "WARNING\r\n" + std::to_string(warning));
    setText(g_infoCard, "INFO\r\n" + std::to_string(info));
}

std::string escapeJson(const std::string& text) {
    std::ostringstream out;
    for (char ch : text) {
        switch (ch) {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default: out << ch; break;
        }
    }
    return out.str();
}

std::string escapeHtml(const std::string& text) {
    std::ostringstream out;
    for (char ch : text) {
        switch (ch) {
            case '&': out << "&amp;"; break;
            case '<': out << "&lt;"; break;
            case '>': out << "&gt;"; break;
            case '"': out << "&quot;"; break;
            case '\'': out << "&#39;"; break;
            default: out << ch; break;
        }
    }
    return out.str();
}

std::string buildTxtReport(const AnalysisOutput& output) {
    std::ostringstream out;
    if (!output.success) {
        out << "Analiz basarisiz oldu.\r\n";
        out << "Hata: " << output.errorMessage << "\r\n";
        return out.str();
    }

    out << "===== CODE SENTINEL ANALIZ RAPORU =====\r\n\r\n";
    out << "Dosya: " << output.fileName << "\r\n";
    out << "Yol: " << output.filePath << "\r\n";
    out << "Boyut: " << output.fileSize << " byte\r\n";
    out << "Yorum satiri: " << output.commentLineCount << "\r\n";
    out << "Calisan kural sayisi: " << output.ruleCount << "\r\n";
    out << "Aktif filtre: " << currentFilterName() << "\r\n";
    out << "Gosterilen bulgu sayisi: " << output.diagnostics.size() << "\r\n\r\n";

    out << "===== BULGU OZETI =====\r\n";
    out << "Total: " << (g_hasLastOutput ? g_lastOutput.diagnostics.size() : output.diagnostics.size()) << "\r\n";
    out << "Critical: " << countSeverity(DiagnosticSeverity::CRITICAL) << "\r\n";
    out << "Warning: " << countSeverity(DiagnosticSeverity::WARNING) << "\r\n";
    out << "Info: " << countSeverity(DiagnosticSeverity::INFO) << "\r\n\r\n";

    out << "===== BULGULAR =====\r\n";
    if (output.diagnostics.empty()) {
        out << "Bu filtrede herhangi bir bulgu yok.\r\n";
    } else {
        for (const Diagnostic& d : output.diagnostics) {
            out << d.getRuleId() << " | " << severityToText(d.getSeverity())
                << " | Satir: " << d.getLine() << " | Sutun: " << d.getColumn() << "\r\n";
            out << "Mesaj: " << d.getMessage() << "\r\n";
            if (!d.getSource().empty()) out << "Kaynak: " << d.getSource() << "\r\n";
            if (!d.getCodeSnippet().empty()) out << "Kod: " << d.getCodeSnippet() << "\r\n";
            out << "----------------------------------------\r\n";
        }
    }

    out << "\r\n===== METRIKLER =====\r\n";
    if (output.metrics.empty()) {
        out << "Metrik uretilemedi.\r\n";
    } else {
        for (const CodeMetric& m : output.metrics) {
            out << m.getName() << ": " << m.getValue();
            if (!m.getUnit().empty()) out << " " << m.getUnit();
            if (!m.getDescription().empty()) out << " -> " << m.getDescription();
            out << "\r\n";
        }
    }
    return out.str();
}

std::string buildJsonReport(const AnalysisOutput& output) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"success\": " << (output.success ? "true" : "false") << ",\n";
    out << "  \"activeFilter\": \"" << escapeJson(currentFilterName()) << "\",\n";
    out << "  \"errorMessage\": \"" << escapeJson(output.errorMessage) << "\",\n";
    out << "  \"fileName\": \"" << escapeJson(output.fileName) << "\",\n";
    out << "  \"filePath\": \"" << escapeJson(output.filePath) << "\",\n";
    out << "  \"fileSize\": " << output.fileSize << ",\n";
    out << "  \"commentLineCount\": " << output.commentLineCount << ",\n";
    out << "  \"ruleCount\": " << output.ruleCount << ",\n";
    out << "  \"summary\": {\n";
    out << "    \"total\": " << (g_hasLastOutput ? g_lastOutput.diagnostics.size() : output.diagnostics.size()) << ",\n";
    out << "    \"critical\": " << countSeverity(DiagnosticSeverity::CRITICAL) << ",\n";
    out << "    \"warning\": " << countSeverity(DiagnosticSeverity::WARNING) << ",\n";
    out << "    \"info\": " << countSeverity(DiagnosticSeverity::INFO) << "\n";
    out << "  },\n";
    out << "  \"diagnosticCount\": " << output.diagnostics.size() << ",\n";
    out << "  \"diagnostics\": [\n";
    for (size_t i = 0; i < output.diagnostics.size(); ++i) {
        const Diagnostic& d = output.diagnostics[i];
        out << "    {\n";
        out << "      \"ruleId\": \"" << escapeJson(d.getRuleId()) << "\",\n";
        out << "      \"severity\": \"" << escapeJson(severityToText(d.getSeverity())) << "\",\n";
        out << "      \"line\": " << d.getLine() << ",\n";
        out << "      \"column\": " << d.getColumn() << ",\n";
        out << "      \"message\": \"" << escapeJson(d.getMessage()) << "\",\n";
        out << "      \"source\": \"" << escapeJson(d.getSource()) << "\",\n";
        out << "      \"codeSnippet\": \"" << escapeJson(d.getCodeSnippet()) << "\"\n";
        out << "    }" << (i + 1 < output.diagnostics.size() ? "," : "") << "\n";
    }
    out << "  ],\n";
    out << "  \"metrics\": [\n";
    for (size_t i = 0; i < output.metrics.size(); ++i) {
        const CodeMetric& m = output.metrics[i];
        out << "    {\n";
        out << "      \"name\": \"" << escapeJson(m.getName()) << "\",\n";
        out << "      \"value\": " << m.getValue() << ",\n";
        out << "      \"unit\": \"" << escapeJson(m.getUnit()) << "\",\n";
        out << "      \"description\": \"" << escapeJson(m.getDescription()) << "\"\n";
        out << "    }" << (i + 1 < output.metrics.size() ? "," : "") << "\n";
    }
    out << "  ]\n";
    out << "}\n";
    return out.str();
}

std::string buildHtmlReport(const AnalysisOutput& output) {
    std::ostringstream out;
    out << "<!DOCTYPE html>\n<html lang=\"tr\">\n<head>\n<meta charset=\"UTF-8\">\n";
    out << "<title>Code Sentinel Analiz Raporu</title>\n";
    out << "<style>body{font-family:Segoe UI,Arial,sans-serif;margin:0;background:#111827;color:#e5e7eb;padding:28px;}";
    out << ".grid{display:grid;grid-template-columns:repeat(4,1fr);gap:14px;margin-bottom:18px}.card{background:#1f2937;border:1px solid #334155;border-radius:14px;padding:18px;box-shadow:0 10px 24px rgba(0,0,0,.2)}";
    out << ".num{font-size:34px;font-weight:800}.muted{color:#9ca3af}.crit{color:#ff6b6b}.warn{color:#facc15}.info{color:#60a5fa}";
    out << "table{border-collapse:collapse;width:100%;background:#0f172a;border-radius:12px;overflow:hidden}th,td{border-bottom:1px solid #334155;padding:10px;text-align:left}th{background:#1e293b}";
    out << "pre{white-space:pre-wrap;background:#020617;color:#dbeafe;padding:14px;border-radius:12px;overflow:auto}</style>\n</head>\n<body>\n";
    out << "<h1>Code Sentinel Analiz Raporu</h1><p class=\"muted\">Aktif filtre: " << escapeHtml(currentFilterName()) << "</p>";

    if (!output.success) {
        out << "<div class=\"card\"><h2>Analiz Basarisiz</h2><p>" << escapeHtml(output.errorMessage) << "</p></div>\n";
    } else {
        out << "<div class=\"grid\"><div class=\"card\"><div class=\"muted\">TOTAL</div><div class=\"num\">" << g_lastOutput.diagnostics.size() << "</div></div>";
        out << "<div class=\"card\"><div class=\"muted\">CRITICAL</div><div class=\"num crit\">" << countSeverity(DiagnosticSeverity::CRITICAL) << "</div></div>";
        out << "<div class=\"card\"><div class=\"muted\">WARNING</div><div class=\"num warn\">" << countSeverity(DiagnosticSeverity::WARNING) << "</div></div>";
        out << "<div class=\"card\"><div class=\"muted\">INFO</div><div class=\"num info\">" << countSeverity(DiagnosticSeverity::INFO) << "</div></div></div>";

        out << "<div class=\"card\"><h2>Dosya Bilgileri</h2>";
        out << "<p><b>Dosya:</b> " << escapeHtml(output.fileName) << "</p>";
        out << "<p><b>Yol:</b> " << escapeHtml(output.filePath) << "</p>";
        out << "<p><b>Boyut:</b> " << output.fileSize << " byte</p>";
        out << "<p><b>Yorum satiri:</b> " << output.commentLineCount << "</p>";
        out << "<p><b>Calisan kural sayisi:</b> " << output.ruleCount << "</p></div>";

        out << "<div class=\"card\"><h2>Bulgular</h2><table><tr><th>Kural</th><th>Seviye</th><th>Satir</th><th>Sutun</th><th>Mesaj</th><th>Kod</th></tr>";
        for (const Diagnostic& d : output.diagnostics) {
            std::string sev = severityToText(d.getSeverity());
            std::string cls = sev == "CRITICAL" ? "crit" : (sev == "WARNING" ? "warn" : "info");
            out << "<tr><td>" << escapeHtml(d.getRuleId()) << "</td><td class=\"" << cls << "\">" << escapeHtml(sev) << "</td>";
            out << "<td>" << d.getLine() << "</td><td>" << d.getColumn() << "</td>";
            out << "<td>" << escapeHtml(d.getMessage()) << "</td><td><code>" << escapeHtml(d.getCodeSnippet()) << "</code></td></tr>";
        }
        out << "</table></div>";

        out << "<div class=\"card\"><h2>Metrikler</h2><table><tr><th>Ad</th><th>Deger</th><th>Birim</th><th>Aciklama</th></tr>";
        for (const CodeMetric& m : output.metrics) {
            out << "<tr><td>" << escapeHtml(m.getName()) << "</td><td>" << m.getValue() << "</td><td>" << escapeHtml(m.getUnit()) << "</td><td>" << escapeHtml(m.getDescription()) << "</td></tr>";
        }
        out << "</table></div>";
        out << "<div class=\"card\"><h2>Kod</h2><pre>" << escapeHtml(formatCodeForScreen(output.rawCode)) << "</pre></div>";
    }
    out << "</body>\n</html>\n";
    return out.str();
}

bool saveTextFile(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    file << content;
    return true;
}

void updateFilteredView() {
    if (!g_hasLastOutput) {
        updateCards();
        return;
    }
    AnalysisOutput out = filteredOutput();
    setEditText(g_resultEdit, buildTxtReport(out));
    updateFilterLabel();
    updateCards();
}

void saveReport(HWND owner, const std::string& type) {
    if (!g_hasLastOutput) {
        MessageBoxW(owner, L"Once dosya secip analiz etmelisin.", L"Rapor yok", MB_ICONWARNING);
        return;
    }

    AnalysisOutput out = filteredOutput();
    std::wstring filter;
    std::wstring defaultName = L"CodeSentinel_Rapor_" + toWide(currentFilterName());
    std::string content;

    if (type == "html") {
        filter = L"HTML Raporu\0*.html\0Tum Dosyalar\0*.*\0";
        defaultName += L".html";
        content = buildHtmlReport(out);
    } else if (type == "json") {
        filter = L"JSON Raporu\0*.json\0Tum Dosyalar\0*.*\0";
        defaultName += L".json";
        content = buildJsonReport(out);
    } else {
        filter = L"TXT Raporu\0*.txt\0Tum Dosyalar\0*.*\0";
        defaultName += L".txt";
        content = buildTxtReport(out);
    }

    wchar_t fileName[MAX_PATH] = L"";
    wcscpy_s(fileName, defaultName.c_str());

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Raporu kaydet";

    if (GetSaveFileNameW(&ofn)) {
        if (saveTextFile(toUtf8(fileName), content)) {
            MessageBoxW(owner, L"Rapor basariyla kaydedildi.", L"Tamam", MB_ICONINFORMATION);
        } else {
            MessageBoxW(owner, L"Rapor kaydedilemedi.", L"Hata", MB_ICONERROR);
        }
    }
}

void chooseFile(HWND owner) {
    wchar_t fileName[MAX_PATH] = L"";

    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = L"C/C++ Dosyalari\0*.c;*.cpp;*.h;*.hpp;*.txt\0Tum Dosyalar\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Analiz edilecek dosyayi sec";

    if (GetOpenFileNameW(&ofn)) {
        g_selectedPath = toUtf8(fileName);
        SetWindowTextW(g_pathEdit, fileName);

        AnalysisService service;
        g_lastOutput = service.analyzeFile(g_selectedPath);
        g_hasLastOutput = true;
        g_showCritical = true;
        g_showWarning = true;
        g_showInfo = true;
        SendMessageW(g_criticalCheck, BM_SETCHECK, BST_CHECKED, 0);
        SendMessageW(g_warningCheck, BM_SETCHECK, BST_CHECKED, 0);
        SendMessageW(g_infoCheck, BM_SETCHECK, BST_CHECKED, 0);
        updateFilterLabel();

        if (!g_lastOutput.rawCode.empty()) {
            setEditText(g_codeEdit, formatCodeForScreen(g_lastOutput.rawCode));
        } else {
            setEditText(g_codeEdit, "Dosya secildi fakat icerik okunamadi.");
        }
        updateFilteredView();
    }
}

void analyzeSelectedFile() {
    if (g_selectedPath.empty()) {
        setEditText(g_resultEdit, "Once 'Dosya Sec' butonu ile bir C dosyasi secmelisin.");
        return;
    }

    AnalysisService service;
    g_lastOutput = service.analyzeFile(g_selectedPath);
    g_hasLastOutput = true;

    if (!g_lastOutput.rawCode.empty()) {
        setEditText(g_codeEdit, formatCodeForScreen(g_lastOutput.rawCode));
    }
    updateFilteredView();
}

HWND createLabel(HWND parent, const wchar_t* text) {
    HWND h = CreateWindowW(L"STATIC", text, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_CENTER,
                           0, 0, 0, 0, parent, nullptr, nullptr, nullptr);
    applyFont(h, g_cardFont);
    return h;
}

HWND createTextLabel(HWND parent, const wchar_t* text, HFONT font, DWORD style = SS_LEFT) {
    HWND h = CreateWindowW(
        L"STATIC",
        text,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | style,
        0, 0, 0, 0,
        parent,
        nullptr,
        nullptr,
        nullptr
    );
    applyFont(h, font);
    return h;
}

void forceRepaint(HWND hwnd) {
    RedrawWindow(hwnd, nullptr, nullptr,
                 RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void moveControl(HWND h, int x, int y, int w, int hgt) {
    if (!h) return;
    MoveWindow(h, x, y, w > 1 ? w : 1, hgt > 1 ? hgt : 1, TRUE);
}

void resizeControls(HWND hwnd) {
    RECT rc{};
    GetClientRect(hwnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    const int minW = 1100;
    if (width < minW) width = minW;

    const int margin = 14;
    const int gap = 10;

    const int buttonW = 118;
    const int buttonH = 38;
    const int checkW = 105;
    const int exportW = 104;

    // Ust marka alani
    int brandTop = 12;
    moveControl(g_brandTitle, margin, brandTop, width - 2 * margin, 34);
    moveControl(g_brandSubtitle, margin, brandTop + 35, width - 2 * margin, 24);

    // Aksiyon butonlari + filtre checkboxlari + rapor butonlari
    int top = brandTop + 68;
    int x = margin;
    moveControl(g_selectButton, x, top, buttonW, buttonH); x += buttonW + gap;
    moveControl(g_analyzeButton, x, top, buttonW, buttonH); x += buttonW + gap + 6;

    moveControl(g_criticalCheck, x, top + 7, checkW, 25); x += checkW + 4;
    moveControl(g_warningCheck, x, top + 7, checkW, 25); x += checkW + 4;
    moveControl(g_infoCheck, x, top + 7, 76, 25);

    int exportX = width - margin - (exportW * 3 + gap * 2);
    moveControl(g_htmlButton, exportX, top, exportW, buttonH);
    moveControl(g_jsonButton, exportX + exportW + gap, top, exportW, buttonH);
    moveControl(g_txtButton, exportX + 2 * (exportW + gap), top, exportW, buttonH);

    // Dosya yolu alani
    int pathTop = top + buttonH + 10;
    moveControl(g_pathEdit, margin, pathTop, width - 2 * margin, 34);

    // Aktif filtre bilgisi
    int labelTop = pathTop + 40;
    moveControl(g_filterLabel, margin, labelTop, width - 2 * margin, 28);

    // Ozet kartlari
    int cardTop = labelTop + 36;
    int cardH = 70;
    int cardW = (width - margin * 2 - gap * 3) / 4;
    moveControl(g_totalCard, margin, cardTop, cardW, cardH);
    moveControl(g_criticalCard, margin + (cardW + gap), cardTop, cardW, cardH);
    moveControl(g_warningCard, margin + 2 * (cardW + gap), cardTop, cardW, cardH);
    moveControl(g_infoCard, margin + 3 * (cardW + gap), cardTop, cardW, cardH);

    // Kod paneli + rapor paneli
    int panelTop = cardTop + cardH + 14;
    int panelH = height - panelTop - margin;
    if (panelH < 220) panelH = 220;

    int panelW = (width - margin * 3) / 2;
    moveControl(g_codeEdit, margin, panelTop, panelW, panelH);
    moveControl(g_resultEdit, margin * 2 + panelW, panelTop, panelW, panelH);

    forceRepaint(hwnd);
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            g_bgBrush = CreateSolidBrush(COLOR_BG);
            g_panelBrush = CreateSolidBrush(COLOR_PANEL);
            g_editBrush = CreateSolidBrush(COLOR_EDIT);
            g_font = CreateFontW(19, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                 DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                 DEFAULT_QUALITY, FF_DONTCARE, L"Segoe UI");
            g_titleFont = CreateFontW(20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      DEFAULT_QUALITY, FF_DONTCARE, L"Segoe UI");
            g_cardFont = CreateFontW(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, FF_DONTCARE, L"Segoe UI");
            g_monoFont = CreateFontW(19,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas"
);
            g_brandFont = CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      CLEARTYPE_QUALITY, FF_DONTCARE, L"Segoe UI");
            g_subtitleFont = CreateFontW(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                         CLEARTYPE_QUALITY, FF_DONTCARE, L"Segoe UI");

            g_brandTitle = createTextLabel(hwnd, L"CODE SENTINEL", g_brandFont, SS_LEFT);
            g_brandSubtitle = createTextLabel(hwnd, L"Professional Static Analysis & Memory Safety Platform", g_subtitleFont, SS_LEFT);

            g_selectButton = CreateWindowW(L"BUTTON", L"Dosya Sec", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                           0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SELECT, nullptr, nullptr);
            g_analyzeButton = CreateWindowW(L"BUTTON", L"Analiz Et", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                            0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ANALYZE, nullptr, nullptr);
            g_htmlButton = CreateWindowW(L"BUTTON", L"HTML", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                         0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SAVE_HTML, nullptr, nullptr);
            g_jsonButton = CreateWindowW(L"BUTTON", L"JSON", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                         0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SAVE_JSON, nullptr, nullptr);
            g_txtButton = CreateWindowW(L"BUTTON", L"TXT", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                        0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SAVE_TXT, nullptr, nullptr);

            g_criticalCheck = CreateWindowW(L"BUTTON", L"Critical", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX,
                                            0, 0, 0, 0, hwnd, (HMENU)ID_CHK_CRITICAL, nullptr, nullptr);
            g_warningCheck = CreateWindowW(L"BUTTON", L"Warning", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX,
                                           0, 0, 0, 0, hwnd, (HMENU)ID_CHK_WARNING, nullptr, nullptr);
            g_infoCheck = CreateWindowW(L"BUTTON", L"Info", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTOCHECKBOX,
                                        0, 0, 0, 0, hwnd, (HMENU)ID_CHK_INFO, nullptr, nullptr);
            g_filterLabel = CreateWindowW(L"STATIC", L"Gosterilen: TUMU", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_LEFT,
                                          0, 0, 0, 0, hwnd, (HMENU)ID_STATIC_FILTER, nullptr, nullptr);
            SendMessageW(g_criticalCheck, BM_SETCHECK, BST_CHECKED, 0);
            SendMessageW(g_warningCheck, BM_SETCHECK, BST_CHECKED, 0);
            SendMessageW(g_infoCheck, BM_SETCHECK, BST_CHECKED, 0);

            g_pathEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_AUTOHSCROLL | ES_READONLY,
                                         0, 0, 0, 0, hwnd, (HMENU)ID_EDIT_PATH, nullptr, nullptr);
            g_codeEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                         WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL |
                                         ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
                                         0, 0, 0, 0, hwnd, (HMENU)ID_EDIT_CODE, nullptr, nullptr);
            g_resultEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                           WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL |
                                           ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
                                           0, 0, 0, 0, hwnd, (HMENU)ID_EDIT_RESULT, nullptr, nullptr);

            g_totalCard = createLabel(hwnd, L"TOTAL\r\n0");
            g_criticalCard = createLabel(hwnd, L"CRITICAL\r\n0");
            g_warningCard = createLabel(hwnd, L"WARNING\r\n0");
            g_infoCard = createLabel(hwnd, L"INFO\r\n0");

            applyFont(g_selectButton);
            applyFont(g_analyzeButton);
            applyFont(g_htmlButton);
            applyFont(g_jsonButton);
            applyFont(g_txtButton);
            applyFont(g_criticalCheck);
            applyFont(g_warningCheck);
            applyFont(g_infoCheck);
            applyFont(g_filterLabel);
            applyFont(g_pathEdit);
            applyFont(g_codeEdit, g_monoFont);
            applyFont(g_resultEdit, g_monoFont);
            SendMessageW(g_codeEdit, EM_LIMITTEXT, 0, 0);
            SendMessageW(g_resultEdit, EM_LIMITTEXT, 0, 0);
            SendMessageW(g_codeEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(14, 14));
            SendMessageW(g_resultEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(14, 14));

            setEditText(
                g_codeEdit,
                "CODE SENTINEL\r\n"
                "Static Analysis & Memory Safety Platform\r\n\r\n"
                "Kod goruntuleme paneli hazir.\r\n"
                "Secilen C kaynak kodu burada satir bazli olarak goruntulenecektir.\r\n\r\n"
                "Analize baslamak icin 'Dosya Sec' butonu ile bir kaynak kod dosyasi secin."
            );

            setEditText(
                g_resultEdit,
                "Sectiginiz kodun analizi burada goruntulenecektir\r\n\r\n"
                "Kullanim Akisi:\r\n"
                "1. 'Dosya Sec' butonu ile kaynak kod secin\r\n"
                "2. 'Analiz Et' ile statik analizi baslatin\r\n"
                "3. Critical / Warning / Info filtrelerini kullanin\r\n"
                "4. Sonuclari HTML / JSON / TXT olarak disa aktarabilirsiniz\r\n\r\n"
                "Diagnostic Seviyeleri:\r\n"
                "CRITICAL -> Bellek ve pointer guvenligi sorunlari\r\n"
                "WARNING  -> Potansiyel kod kalite problemleri\r\n"
                "INFO     -> Bilgilendirici analiz ciktilari\r\n\r\n"
                "Analiz sistemi baslatilmaya hazir."
            );
            resizeControls(hwnd);
            return 0;
        }
        case WM_GETMINMAXINFO: {
            auto* info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 1100;
            info->ptMinTrackSize.y = 650;
            return 0;
        }
        case WM_SIZE:
            resizeControls(hwnd);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BTN_SELECT) { chooseFile(hwnd); return 0; }
            if (LOWORD(wParam) == ID_BTN_ANALYZE) { analyzeSelectedFile(); return 0; }
            if (LOWORD(wParam) == ID_BTN_SAVE_HTML) { saveReport(hwnd, "html"); return 0; }
            if (LOWORD(wParam) == ID_BTN_SAVE_JSON) { saveReport(hwnd, "json"); return 0; }
            if (LOWORD(wParam) == ID_BTN_SAVE_TXT) { saveReport(hwnd, "txt"); return 0; }
            if ((LOWORD(wParam) == ID_CHK_CRITICAL || LOWORD(wParam) == ID_CHK_WARNING || LOWORD(wParam) == ID_CHK_INFO)
                && HIWORD(wParam) == BN_CLICKED) {
                g_showCritical = SendMessageW(g_criticalCheck, BM_GETCHECK, 0, 0) == BST_CHECKED;
                g_showWarning = SendMessageW(g_warningCheck, BM_GETCHECK, 0, 0) == BST_CHECKED;
                g_showInfo = SendMessageW(g_infoCheck, BM_GETCHECK, 0, 0) == BST_CHECKED;
                updateFilteredView();
                return 0;
            }
            break;
        case WM_CTLCOLORSTATIC: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            HWND control = reinterpret_cast<HWND>(lParam);

            // ONEMLI:
            // ES_READONLY olan EDIT kontrolleri WM_CTLCOLOREDIT yerine
            // WM_CTLCOLORSTATIC mesaji gonderebilir.
            // Burada transparent arka plan verilirse eski yazi temizlenmez
            // ve rapor panelinde harfler ust uste biner.
            if (control == g_codeEdit || control == g_resultEdit || control == g_pathEdit) {
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, COLOR_EDIT);
                SetTextColor(hdc, COLOR_TEXT);
                return reinterpret_cast<LRESULT>(g_editBrush);
            }

            if (control == g_brandTitle || control == g_brandSubtitle) {
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, COLOR_BG);
                SetTextColor(hdc, control == g_brandTitle ? COLOR_INFO : COLOR_MUTED);
                return reinterpret_cast<LRESULT>(g_bgBrush);
            }

            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, COLOR_PANEL);
            if (control == g_criticalCard) SetTextColor(hdc, COLOR_CRITICAL);
            else if (control == g_warningCard) SetTextColor(hdc, COLOR_WARNING);
            else if (control == g_infoCard) SetTextColor(hdc, COLOR_INFO);
            else SetTextColor(hdc, COLOR_TEXT);
            return reinterpret_cast<LRESULT>(g_panelBrush);
        }
        case WM_CTLCOLORBTN: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, COLOR_TEXT);
            return reinterpret_cast<LRESULT>(g_bgBrush);
        }
        case WM_CTLCOLOREDIT: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            SetBkColor(hdc, COLOR_EDIT);
            SetTextColor(hdc, COLOR_TEXT);
            return reinterpret_cast<LRESULT>(g_editBrush);
        }
        case WM_CTLCOLORLISTBOX: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            SetBkColor(hdc, RGB(31, 41, 55));
            SetTextColor(hdc, COLOR_TEXT);
            return reinterpret_cast<LRESULT>(g_panelBrush);
        }
        case WM_ERASEBKGND: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            RECT rc{};
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, g_bgBrush);
            return 1;
        }
        case WM_DESTROY:
            if (g_font) DeleteObject(g_font);
            if (g_titleFont) DeleteObject(g_titleFont);
            if (g_cardFont) DeleteObject(g_cardFont);
            if (g_monoFont) DeleteObject(g_monoFont);
            if (g_brandFont) DeleteObject(g_brandFont);
            if (g_subtitleFont) DeleteObject(g_subtitleFont);
            if (g_bgBrush) DeleteObject(g_bgBrush);
            if (g_panelBrush) DeleteObject(g_panelBrush);
            if (g_editBrush) DeleteObject(g_editBrush);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"CodeSentinelModernNoQtWindow";

    WNDCLASSW wc{};
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = g_bgBrush;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassW(&wc);
HWND hwnd = CreateWindowExW(
    0,
    CLASS_NAME,
    L"CODE SENTINEL",
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    1320,
    780,
    nullptr,
    nullptr,
    hInstance,
    nullptr
);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
