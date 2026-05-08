#ifndef ANALYSIS_ENGINE_H
#define ANALYSIS_ENGINE_H

#include <vector>
#include <string>

using namespace std;

class FileLoader;
class Preprocessor;
class Lexer;
class Parser;
class SyntaxAnalyzer;
class RuleEngine;
class MetricCalculator;
class SeverityClassifier;

class SourceFile;
class AnalysisResult;
class Diagnostic;
class CodeMetric;
class ASTNode;
class Token;

// Tüm analiz sürecini yöneten merkez sınıf
class AnalysisEngine {

private:

    // Dosya yükleyici
    FileLoader* fileLoader;

    // Yorum temizleyici
    Preprocessor* preprocessor;

    // Lexer
    Lexer* lexer;

    // Parser
    Parser* parser;

    // Syntax analyzer
    SyntaxAnalyzer* syntaxAnalyzer;

    // Rule engine
    RuleEngine* ruleEngine;

    // Kod metrik hesaplayıcı
    MetricCalculator* metricCalculator;

    // Severity belirleyici
    SeverityClassifier* severityClassifier;

public:

    // Yapıcı metod
    AnalysisEngine();

    // Yıkıcı metod
    ~AnalysisEngine();

    // Ana analiz fonksiyonu
    AnalysisResult analyze(SourceFile file);

    // Tüm bileşenler hazır mı?
    bool isReady();
};

#endif