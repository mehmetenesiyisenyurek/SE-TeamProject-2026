#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"
#include "parser/ASTNode.h"

#include "infrastructure/Preprocessor.h"
#include "infrastructure/Diagnostic.h"
#include "infrastructure/DiagnosticSeverity.h"

#include "syntax_analyzer/SyntaxAnalyzer.h"
#include "syntax_analyzer/RuleEngine.h"

#include "pointer/NullDereferenceRule.h"

struct FileAnalysisReport {
    std::string filePath;
    std::string rawCode;
    std::vector<std::string> codeLines;
    std::vector<Diagnostic> diagnostics;
    std::vector<std::string> generalMessages;
    long long elapsedMs = 0;
};

std::string readFile(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> readTestFileList(const std::string& path) {
    std::vector<std::string> fileNames;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "HATA: " << path << " acilamadi." << std::endl;
        return fileNames;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            fileNames.push_back(line);
        }
    }

    return fileNames;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream stream(text);
    std::string line;

    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) {
        lines.push_back("");
    }

    return lines;
}

std::string htmlEscape(const std::string& text) {
    std::string escaped;

    for (char ch : text) {
        switch (ch) {
            case '&': escaped += "&amp;"; break;
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&#39;"; break;
            default: escaped += ch; break;
        }
    }

    return escaped;
}

std::string severityToText(DiagnosticSeverity severity) {
    switch (severity) {
        case DiagnosticSeverity::CRITICAL:
            return "KRITIK";
        case DiagnosticSeverity::WARNING:
            return "UYARI";
        case DiagnosticSeverity::INFO:
            return "BILGI";
    }

    return "BILINMIYOR";
}

std::string severityToCssClass(DiagnosticSeverity severity) {
    switch (severity) {
        case DiagnosticSeverity::CRITICAL:
            return "critical";
        case DiagnosticSeverity::WARNING:
            return "warning";
        case DiagnosticSeverity::INFO:
            return "info";
    }

    return "info";
}

std::string formatDiagnostic(const Diagnostic& diagnostic) {
    std::stringstream output;

    output << "<div class=\"diagnostic "
           << severityToCssClass(diagnostic.getSeverity())
           << "\">";

    output << "<span class=\"badge\">"
           << severityToText(diagnostic.getSeverity())
           << "</span> ";

    if (!diagnostic.getRuleId().empty()) {
        output << "<strong>"
               << htmlEscape(diagnostic.getRuleId())
               << "</strong> - ";
    }

    output << htmlEscape(diagnostic.getMessage());

    if (diagnostic.getColumn() > 0) {
        output << " <span class=\"muted\">(Kolon: "
               << diagnostic.getColumn()
               << ")</span>";
    }

    output << "</div>";

    return output.str();
}

FileAnalysisReport analyzeSingleFile(const std::string& filePath) {
    FileAnalysisReport report;
    report.filePath = filePath;

    auto startTime = std::chrono::high_resolution_clock::now();

    report.rawCode = readFile(filePath);
    report.codeLines = splitLines(report.rawCode);

    if (report.rawCode.empty()) {
        report.generalMessages.push_back("Dosya bos veya okunamadi.");
        return report;
    }

    Preprocessor preprocessor;
    std::string cleanCode = preprocessor.process(report.rawCode);

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(cleanCode);

    Parser parser;
    ASTNode* ast = parser.parse(tokens);

    for (const Diagnostic& diagnostic : parser.getDiagnostics()) {
        report.diagnostics.push_back(diagnostic);
    }

    if (ast == nullptr) {
        report.generalMessages.push_back("AST olusturulamadi. Parser sonrasi analizler calistirilamadi.");
    } else {
        SyntaxAnalyzer syntaxAnalyzer;
        std::vector<Diagnostic> syntaxDiagnostics = syntaxAnalyzer.analyze(*ast, tokens);

        for (const Diagnostic& diagnostic : syntaxDiagnostics) {
            report.diagnostics.push_back(diagnostic);
        }

        RuleEngine ruleEngine;

        // RuleEngine varsayilan kurallari ekliyor.
        // NullDereferenceRule varsayilan listede yoksa ekstra eklenir.
        NullDereferenceRule nullDereferenceRule;
        ruleEngine.addRule(&nullDereferenceRule);

        std::vector<Diagnostic> ruleDiagnostics = ruleEngine.analyzeAll(*ast);

        for (const Diagnostic& diagnostic : ruleDiagnostics) {
            report.diagnostics.push_back(diagnostic);
        }

        delete ast;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    report.elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime
    ).count();

    return report;
}

void writeHtmlReport(
    const std::vector<FileAnalysisReport>& reports,
    const std::string& outputPath
) {
    std::ofstream html(outputPath);

    if (!html.is_open()) {
        std::cerr << "HATA: HTML rapor dosyasi olusturulamadi: " << outputPath << std::endl;
        return;
    }

    int totalDiagnostics = 0;
    int totalCritical = 0;
    int totalWarning = 0;
    int totalInfo = 0;

    for (const FileAnalysisReport& report : reports) {
        totalDiagnostics += static_cast<int>(report.diagnostics.size());

        for (const Diagnostic& diagnostic : report.diagnostics) {
            if (diagnostic.getSeverity() == DiagnosticSeverity::CRITICAL) {
                totalCritical++;
            } else if (diagnostic.getSeverity() == DiagnosticSeverity::WARNING) {
                totalWarning++;
            } else if (diagnostic.getSeverity() == DiagnosticSeverity::INFO) {
                totalInfo++;
            }
        }
    }

    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"tr\">\n";
    html << "<head>\n";
    html << "<meta charset=\"UTF-8\">\n";
    html << "<title>CodeSentinel Analiz Raporu</title>\n";
    html << "<style>\n";
    html << "body{font-family:Arial,Helvetica,sans-serif;background:#f4f6f8;margin:0;color:#1f2937;}\n";
    html << ".container{max-width:1400px;margin:0 auto;padding:24px;}\n";
    html << ".header{background:#111827;color:white;border-radius:14px;padding:24px;margin-bottom:20px;}\n";
    html << ".header h1{margin:0 0 10px 0;font-size:28px;}\n";
    html << ".summary{display:flex;gap:12px;flex-wrap:wrap;margin-top:16px;}\n";
    html << ".summary-card{background:white;color:#111827;border-radius:10px;padding:12px 16px;min-width:130px;}\n";
    html << ".summary-card strong{display:block;font-size:22px;}\n";
    html << ".file-card{background:white;border-radius:14px;margin-bottom:26px;box-shadow:0 4px 14px rgba(0,0,0,.08);overflow:hidden;}\n";
    html << ".file-title{background:#e5e7eb;padding:16px 18px;border-bottom:1px solid #d1d5db;}\n";
    html << ".file-title h2{font-size:20px;margin:0 0 6px 0;}\n";
    html << ".muted{color:#6b7280;font-size:13px;}\n";
    html << "table{width:100%;border-collapse:collapse;table-layout:fixed;}\n";
    html << "th{background:#f9fafb;text-align:left;padding:10px;border-bottom:1px solid #d1d5db;font-size:14px;}\n";
    html << "td{vertical-align:top;padding:0;border-bottom:1px solid #edf0f2;}\n";
    html << ".line-no{width:70px;text-align:right;color:#6b7280;background:#f9fafb;padding:9px 10px;font-family:Consolas,monospace;}\n";
    html << ".code{font-family:Consolas,'Courier New',monospace;white-space:pre-wrap;padding:9px 12px;background:#ffffff;}\n";
    html << ".result{padding:8px 10px;background:#fcfcfd;}\n";
    html << ".diagnostic{border-radius:8px;padding:8px 10px;margin:2px 0;font-size:14px;line-height:1.4;}\n";
    html << ".critical{background:#fee2e2;border-left:5px solid #dc2626;}\n";
    html << ".warning{background:#fef3c7;border-left:5px solid #d97706;}\n";
    html << ".info{background:#dbeafe;border-left:5px solid #2563eb;}\n";
    html << ".clean{color:#9ca3af;font-size:13px;padding:9px 0;}\n";
    html << ".badge{font-size:12px;font-weight:bold;letter-spacing:.3px;}\n";
    html << ".general{padding:14px 18px;background:#fff7ed;border-top:1px solid #fed7aa;}\n";
    html << "</style>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "<div class=\"container\">\n";
    html << "<div class=\"header\">\n";
    html << "<h1>CodeSentinel Analiz Raporu</h1>\n";
    html << "<div>C kodu ve C++ analiz çıktısı yan yana gösterilir.</div>\n";
    html << "<div class=\"summary\">\n";
    html << "<div class=\"summary-card\"><strong>" << reports.size() << "</strong>Dosya</div>\n";
    html << "<div class=\"summary-card\"><strong>" << totalDiagnostics << "</strong>Toplam Bulgu</div>\n";
    html << "<div class=\"summary-card\"><strong>" << totalCritical << "</strong>Kritik</div>\n";
    html << "<div class=\"summary-card\"><strong>" << totalWarning << "</strong>Uyarı</div>\n";
    html << "<div class=\"summary-card\"><strong>" << totalInfo << "</strong>Bilgi</div>\n";
    html << "</div>\n";
    html << "</div>\n";

    for (const FileAnalysisReport& report : reports) {
        std::map<int, std::vector<Diagnostic>> diagnosticsByLine;
        std::vector<Diagnostic> diagnosticsWithoutLine;

        for (const Diagnostic& diagnostic : report.diagnostics) {
            if (diagnostic.getLine() > 0) {
                diagnosticsByLine[diagnostic.getLine()].push_back(diagnostic);
            } else {
                diagnosticsWithoutLine.push_back(diagnostic);
            }
        }

        html << "<section class=\"file-card\">\n";
        html << "<div class=\"file-title\">\n";
        html << "<h2>" << htmlEscape(report.filePath) << "</h2>\n";
        html << "<div class=\"muted\">Sure: " << report.elapsedMs
             << " ms | Bulgu: " << report.diagnostics.size() << "</div>\n";
        html << "</div>\n";

        if (!report.generalMessages.empty()) {
            html << "<div class=\"general\">\n";
            for (const std::string& message : report.generalMessages) {
                html << "<div>" << htmlEscape(message) << "</div>\n";
            }
            html << "</div>\n";
        }

        if (!diagnosticsWithoutLine.empty()) {
            html << "<div class=\"general\">\n";
            html << "<strong>Satır bilgisi olmayan analiz çıktıları:</strong>\n";
            for (const Diagnostic& diagnostic : diagnosticsWithoutLine) {
                html << formatDiagnostic(diagnostic) << "\n";
            }
            html << "</div>\n";
        }

        html << "<table>\n";
        html << "<thead><tr>"
             << "<th style=\"width:70px;\">Satır</th>"
             << "<th>C Kodu</th>"
             << "<th>C++ Analiz Çıktısı</th>"
             << "</tr></thead>\n";
        html << "<tbody>\n";

        for (size_t i = 0; i < report.codeLines.size(); ++i) {
            int lineNumber = static_cast<int>(i + 1);

            html << "<tr>\n";
            html << "<td class=\"line-no\">" << lineNumber << "</td>\n";
            html << "<td class=\"code\">" << htmlEscape(report.codeLines[i]) << "</td>\n";
            html << "<td class=\"result\">";

            auto found = diagnosticsByLine.find(lineNumber);

            if (found == diagnosticsByLine.end()) {
                html << "<div class=\"clean\">Bulgu yok</div>";
            } else {
                for (const Diagnostic& diagnostic : found->second) {
                    html << formatDiagnostic(diagnostic);
                }
            }

            html << "</td>\n";
            html << "</tr>\n";
        }

        html << "</tbody>\n";
        html << "</table>\n";
        html << "</section>\n";
    }

    html << "</div>\n";
    html << "</body>\n";
    html << "</html>\n";
}

void openHtmlReport(const std::string& outputPath) {
#ifdef _WIN32
    std::string command = "start \"\" \"" + outputPath + "\"";
#elif __APPLE__
    std::string command = "open \"" + outputPath + "\"";
#else
    std::string command = "xdg-open \"" + outputPath + "\"";
#endif

    std::system(command.c_str());
}

int main() {
    const std::string testListPath = "test_dosyalari.txt";
    const std::string reportPath = "analysis_report.html";

    std::vector<std::string> testFiles = readTestFileList(testListPath);

    if (testFiles.empty()) {
        std::cout << "Calistirilacak test dosyasi bulunamadi." << std::endl;
        std::cout << "Once " << testListPath << " dosyasina analiz edilecek .c dosyalarini ekleyin." << std::endl;
        return 1;
    }

    std::vector<FileAnalysisReport> reports;

    for (const std::string& filePath : testFiles) {
        reports.push_back(analyzeSingleFile(filePath));
    }

    writeHtmlReport(reports, reportPath);

    std::cout << "HTML analiz raporu olusturuldu: " << reportPath << std::endl;
    std::cout << "Rapor tarayicida aciliyor..." << std::endl;

    openHtmlReport(reportPath);

    return 0;
}
