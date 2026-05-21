
#include "AnalysisEngine.h"

#include <iostream>

// Constructor
AnalysisEngine::AnalysisEngine() {

    /*
     * Gerçek projede burada:
     *
     * fileLoader = new FileLoader();
     * parser = new Parser();
     * vs...
     *
     * yapılacak
     */

    fileLoader = nullptr;
    preprocessor = nullptr;
    lexer = nullptr;
    parser = nullptr;
    syntaxAnalyzer = nullptr;
    ruleEngine = nullptr;
    metricCalculator = nullptr;
    severityClassifier = nullptr;
}

// Destructor
AnalysisEngine::~AnalysisEngine() {

    delete fileLoader;
    delete preprocessor;
    delete lexer;
    delete parser;
    delete syntaxAnalyzer;
    delete ruleEngine;
    delete metricCalculator;
    delete severityClassifier;
}

// Ana analiz akışı
AnalysisResult AnalysisEngine::analyze(
        SourceFile file
) {

    // Dosya içeriğini al
    string rawCode =
            file.getContent();

    // Yorumları temizle
    string cleanCode =
            preprocessor->process(rawCode);

    // Lexer çalıştır
    vector<Token> tokens =
            lexer->tokenize(cleanCode);

    // Parser çalıştır
    ASTNode* ast =
            parser->parse(tokens);

    // Parser diagnosticlerini al
    vector<Diagnostic> parserDiagnostics =
            parser->getDiagnostics();

    // Syntax analiz
    vector<Diagnostic> syntaxDiagnostics =
            syntaxAnalyzer->analyze(
                    ast,
                    tokens
            );

    // Rule engine çalıştır
    vector<Diagnostic> ruleDiagnostics =
            ruleEngine->analyzeAll(ast);

    // Kod metriklerini hesapla
    vector<CodeMetric> metrics =
            metricCalculator->calculate(
                    ast,
                    rawCode,
                    cleanCode
            );

    // Tüm diagnosticleri birleştir
    vector<Diagnostic> allDiagnostics;

    allDiagnostics.insert(
            allDiagnostics.end(),
            parserDiagnostics.begin(),
            parserDiagnostics.end()
    );

    allDiagnostics.insert(
            allDiagnostics.end(),
            syntaxDiagnostics.begin(),
            syntaxDiagnostics.end()
    );

    allDiagnostics.insert(
            allDiagnostics.end(),
            ruleDiagnostics.begin(),
            ruleDiagnostics.end()
    );

    // Severity ata
    for (Diagnostic& diagnostic : allDiagnostics) {

        severityClassifier->classify(
                diagnostic
        );
    }

    // AnalysisResult oluştur
    AnalysisResult result(
            file.getFileName(),
            file.getFileSize(),
            allDiagnostics,
            metrics,
            0
    );

    return result;
}

// Tüm bileşenler hazır mı?
bool AnalysisEngine::isReady() {

    return
            fileLoader != nullptr &&
            preprocessor != nullptr &&
            lexer != nullptr &&
            parser != nullptr &&
            syntaxAnalyzer != nullptr &&
            ruleEngine != nullptr &&
            metricCalculator != nullptr &&
            severityClassifier != nullptr;
}