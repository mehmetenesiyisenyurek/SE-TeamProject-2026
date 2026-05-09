#ifndef ANALYSIS_ENGINE_H
#define ANALYSIS_ENGINE_H

#include <string>
#include <vector>

#include "../infrastructure/CodeMetric.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/SourceFile.h"

/*
 * AnalysisResult:
 * AnalysisEngine çalıştıktan sonra main.cpp tarafına dönen sonuç modelidir.
 */
class AnalysisResult {
private:
    std::string fileName;
    long fileSize;
    std::vector<Diagnostic> diagnostics;
    std::vector<CodeMetric> metrics;
    int commentLineCount;
    bool successful;

public:
    AnalysisResult(
        const std::string& fileName,
        long fileSize,
        const std::vector<Diagnostic>& diagnostics,
        const std::vector<CodeMetric>& metrics,
        int commentLineCount,
        bool successful
    );

    std::string getFileName() const;
    long getFileSize() const;
    const std::vector<Diagnostic>& getDiagnostics() const;
    const std::vector<CodeMetric>& getMetrics() const;
    int getCommentLineCount() const;
    bool isSuccessful() const;
    bool hasDiagnostics() const;
};

class Preprocessor;
class Lexer;
class Parser;
class SyntaxAnalyzer;
class RuleEngine;

/*
 * AnalysisEngine:
 * SourceFile nesnesini alır; preprocessing, lexical analiz, parsing,
 * syntax analizi, rule engine ve metrik üretimini tek yerden yönetir.
 */
class AnalysisEngine {
private:
    Preprocessor* preprocessor;
    Lexer* lexer;
    Parser* parser;
    SyntaxAnalyzer* syntaxAnalyzer;
    RuleEngine* ruleEngine;

    std::vector<CodeMetric> calculateMetrics(
        const std::string& rawCode,
        const std::string& cleanCode,
        const std::vector<Diagnostic>& diagnostics
    ) const;

    int countLines(const std::string& text) const;

public:
    AnalysisEngine();
    ~AnalysisEngine();

    AnalysisResult analyze(const SourceFile& file);
    bool isReady() const;
};

#endif
