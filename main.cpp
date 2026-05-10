#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

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
#include "pointer/UseAfterFreeRule.h"

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

void printResults(
    const std::string& fileName,
    const std::vector<Diagnostic>& diagnostics,
    long long elapsedMs
) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   CODESENTINEL - ANALIZ RAPORU" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Dosya : " << fileName << std::endl;
    std::cout << "Sure  : " << elapsedMs << " ms" << std::endl;
    std::cout << "Bulgu : " << diagnostics.size() << " adet" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    int critical = 0;
    int warning = 0;
    int info = 0;

    for (const Diagnostic& diag : diagnostics) {
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

        std::cout << label
                  << " Satir " << diag.getLine()
                  << ": " << diag.getMessage();

        if (!diag.getRuleId().empty()) {
            std::cout << " (" << diag.getRuleId() << ")";
        }

        std::cout << std::endl;
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Ozet: "
              << critical << " kritik, "
              << warning << " uyari, "
              << info << " bilgi"
              << std::endl;
    std::cout << "========================================" << std::endl;
}

void analyzeSingleFile(const std::string& filePath) {
    auto startTime = std::chrono::high_resolution_clock::now();

    std::string rawCode = readFile(filePath);

    if (rawCode.empty()) {
        std::cout << "Dosya bos veya okunamadi: " << filePath << std::endl;
        return;
    }

    Preprocessor preprocessor;
    std::string cleanCode = preprocessor.process(rawCode);

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(cleanCode);

    Parser parser;
    ASTNode* ast = parser.parse(tokens);

    if (ast == nullptr) {
        std::cout << "AST olusturulamadi: " << filePath << std::endl;
        return;
    }

    std::vector<Diagnostic> allDiagnostics;

    for (const Diagnostic& d : parser.getDiagnostics()) {
        allDiagnostics.push_back(d);
    }

    SyntaxAnalyzer syntaxAnalyzer;
    std::vector<Diagnostic> syntaxDiagnostics = syntaxAnalyzer.analyze(*ast, tokens);

    for (const Diagnostic& d : syntaxDiagnostics) {
        allDiagnostics.push_back(d);
    }

    RuleEngine ruleEngine;

    NullDereferenceRule nullDereferenceRule;
    ruleEngine.addRule(&nullDereferenceRule);

    UseAfterFreeRule useAfterFreeRule;
    ruleEngine.addRule(&useAfterFreeRule);

    std::vector<Diagnostic> ruleDiagnostics = ruleEngine.analyzeAll(*ast);

    for (const Diagnostic& d : ruleDiagnostics) {
        allDiagnostics.push_back(d);
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    long long elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime
        ).count();

    printResults(filePath, allDiagnostics, elapsedMs);

    delete ast;
}

int main() {
    std::cout << "CodeSentinel v1.0 - Toplu Test Analizi Baslatiliyor..." << std::endl;

    std::vector<std::string> testFiles = readTestFileList("test_dosyalari.txt");

    if (testFiles.empty()) {
        std::cout << "Calistirilacak test dosyasi bulunamadi." << std::endl;
        return 1;
    }

    for (const std::string& filePath : testFiles) {
        analyzeSingleFile(filePath);
    }

    std::cout << "\nTum test dosyalari analiz edildi." << std::endl;

    return 0;
}
