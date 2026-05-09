#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "parser/Parser.h"
#include "infrastructure/DiagnosticSeverity.h"
#include "infrastructure/Diagnostic.h"
#include "lexer/Token.h"
#include "lexer/Lexer.h"
#include "parser/ASTNode.h"
#include "infrastructure/Preprocessor.h"
#include "syntax_analyzer/SyntaxAnalyzer.h"
#include "syntax_analyzer/IRule.h"
#include "syntax_analyzer/RuleEngine.h"
#include "syntax_analyzer/UnusedVariableRule.h"
#include "pointer/NullDereferenceRule.h"
// ============================================================
//  DOSYA OKUMA (FileLoader yerine basit fonksiyon)
// ============================================================
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "HATA: Dosya acilamadi: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
// ============================================================
//  SONUÇLARI YAZDIRMA (TxtExporter yerine basit fonksiyon)
// ============================================================
void printResults(const std::string& fileName,
                  const std::vector<Diagnostic>& diagnostics,
                  long long elapsedMs) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   CODESENTINEL - ANALIZ RAPORU" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Dosya    : " << fileName << std::endl;
    std::cout << "Sure     : " << elapsedMs << " ms" << std::endl;
    std::cout << "Bulgu    : " << diagnostics.size() << " adet" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    // Sayaçlar
    int critical = 0, warning = 0, info = 0;
    for (const auto& diag : diagnostics) {
        // Seviye etiketi
        std::string label;
        switch (diag.getSeverity()) {
            case DiagnosticSeverity::CRITICAL:
                label = "[KRITIK]";
                critical++;
                break;
            case DiagnosticSeverity::WARNING:
                label = "[UYARI] ";
                warning++;
                break;
            case DiagnosticSeverity::INFO:
                label = "[BILGI] ";
                info++;
                break;
        }
        std::cout << label << " Satir " << diag.getLine()
                  << ": " << diag.getMessage();
        // Kural ID varsa göster
        if (!diag.getRuleId().empty()) {
            std::cout << " (" << diag.getRuleId() << ")";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Ozet: " << critical << " kritik, "
              << warning << " uyari, "
              << info << " bilgi" << std::endl;
    std::cout << "========================================\n" << std::endl;
}
// ============================================================
//  ANA FONKSİYON
// ============================================================
int main(int argc, char* argv[]) {
    // Dosya yolunu komut satırından al veya varsayılan kullan
    std::string filePath = "test_hatali.c";
    if (argc > 1) {
        filePath = argv[1];
    }
    std::cout << "CodeSentinel v1.0 - Statik Kod Analizi Baslatiliyor..." << std::endl;
    std::cout << "Hedef dosya: " << filePath << std::endl;
    // Zamanlayıcı başlat
    auto startTime = std::chrono::high_resolution_clock::now();
    // ---- ADIM 1: Dosyayı oku ----
    std::string rawCode = readFile(filePath);
    if (rawCode.empty()) {
        std::cerr << "Dosya bos veya okunamadi. Cikiliyor." << std::endl;
        return 1;
    }
    // ---- ADIM 2: Yorumları temizle ----
    Preprocessor preprocessor;
    std::string cleanCode = preprocessor.process(rawCode);
    // ---- ADIM 3: Tokenize et ----
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(cleanCode);
    std::cout << "Token sayisi: " << tokens.size() << std::endl;
    // ---- ADIM 4: AST oluştur ----
    Parser parser;
    ASTNode* ast = parser.parse(tokens);
    std::cout << "AST olusturuldu." << std::endl;
    // (Opsiyonel) AST'yi ekrana yazdır
    // std::cout << ast->toString() << std::endl;
    // ---- ADIM 5: Parser hatalarını topla ----
    std::vector<Diagnostic> allDiagnostics;
    for (const auto& d : parser.getDiagnostics()) {
        allDiagnostics.push_back(d);
    }
    // ---- ADIM 6: Syntax analizi ----
    SyntaxAnalyzer syntaxAnalyzer;
    std::vector<Diagnostic> syntaxDiags = syntaxAnalyzer.analyze(ast, tokens);
    for (const auto& d : syntaxDiags) {
        allDiagnostics.push_back(d);
    }
    // ---- ADIM 7: Kural motoru ----
    RuleEngine ruleEngine;
    ruleEngine.addRule(new UnusedVariableRule());
    ruleEngine.addRule(new NullDereferenceRule());
    // Daha fazla kural eklenebilir:
    // ruleEngine.addRule(new MagicNumberRule());
    // ruleEngine.addRule(new UseAfterFreeRule());
    std::vector<Diagnostic> ruleDiags = ruleEngine.analyzeAll(ast);
    for (const auto& d : ruleDiags) {
        allDiagnostics.push_back(d);
    }
    // ---- ADIM 8: Zamanlayıcı durdur ----
    auto endTime = std::chrono::high_resolution_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    // ---- ADIM 9: Sonuçları yazdır ----
    printResults(filePath, allDiagnostics, elapsed);
    // ---- TEMİZLİK ----
    delete ast;  // Tüm ağaç özyinelemeli silinir
    return 0;
}