#include "AnalysisEngine.h"

#include <sstream>

#include "../infrastructure/Preprocessor.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "../parser/ASTNode.h"
#include "../syntax_analyzer/SyntaxAnalyzer.h"
#include "../syntax_analyzer/RuleEngine.h"

AnalysisResult::AnalysisResult(
    const std::string& fileName,
    long fileSize,
    const std::vector<Diagnostic>& diagnostics,
    const std::vector<CodeMetric>& metrics,
    int commentLineCount,
    bool successful
) {
    this->fileName = fileName;
    this->fileSize = fileSize;
    this->diagnostics = diagnostics;
    this->metrics = metrics;
    this->commentLineCount = commentLineCount;
    this->successful = successful;
}

std::string AnalysisResult::getFileName() const {
    return fileName;
}

long AnalysisResult::getFileSize() const {
    return fileSize;
}

const std::vector<Diagnostic>& AnalysisResult::getDiagnostics() const {
    return diagnostics;
}

const std::vector<CodeMetric>& AnalysisResult::getMetrics() const {
    return metrics;
}

int AnalysisResult::getCommentLineCount() const {
    return commentLineCount;
}

bool AnalysisResult::isSuccessful() const {
    return successful;
}

bool AnalysisResult::hasDiagnostics() const {
    return !diagnostics.empty();
}

AnalysisEngine::AnalysisEngine() {
    preprocessor = new Preprocessor();
    lexer = new Lexer();
    parser = new Parser();
    syntaxAnalyzer = new SyntaxAnalyzer();
    ruleEngine = new RuleEngine();
}

AnalysisEngine::~AnalysisEngine() {
    delete preprocessor;
    delete lexer;
    delete parser;
    delete syntaxAnalyzer;
    delete ruleEngine;
}

AnalysisResult AnalysisEngine::analyze(const SourceFile& file) {
    std::vector<Diagnostic> allDiagnostics;
    std::vector<CodeMetric> metrics;

    if (!isReady()) {
        allDiagnostics.emplace_back(
            0,
            0,
            "AnalysisEngine bileşenleri hazır değil.",
            DiagnosticSeverity::CRITICAL,
            "analysis_engine",
            "ENGINE_NOT_READY",
            ""
        );

        return AnalysisResult(
            file.getFileName(),
            file.getFileSize(),
            allDiagnostics,
            metrics,
            0,
            false
        );
    }

    if (file.isEmpty()) {
        allDiagnostics.emplace_back(
            0,
            0,
            "Analiz edilecek dosya boş.",
            DiagnosticSeverity::WARNING,
            "analysis_engine",
            "EMPTY_FILE",
            ""
        );
    }

    const std::string rawCode = file.getContent();
    const std::string cleanCode = preprocessor->process(rawCode);

    std::vector<Token> tokens = lexer->tokenize(cleanCode);
    ASTNode* ast = parser->parse(tokens);

    const std::vector<Diagnostic>& parserDiagnostics = parser->getDiagnostics();
    allDiagnostics.insert(
        allDiagnostics.end(),
        parserDiagnostics.begin(),
        parserDiagnostics.end()
    );

    if (ast != nullptr) {
        std::vector<Diagnostic> syntaxDiagnostics = syntaxAnalyzer->analyze(*ast, tokens);
        std::vector<Diagnostic> ruleDiagnostics = ruleEngine->analyzeAll(*ast);

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
    }
    else {
        allDiagnostics.emplace_back(
            0,
            0,
            "Parser AST oluşturamadı.",
            DiagnosticSeverity::CRITICAL,
            "analysis_engine",
            "AST_NULL",
            ""
        );
    }

    metrics = calculateMetrics(rawCode, cleanCode, allDiagnostics);

    delete ast;

    return AnalysisResult(
        file.getFileName(),
        file.getFileSize(),
        allDiagnostics,
        metrics,
        preprocessor->getCommentLineCount(),
        true
    );
}

bool AnalysisEngine::isReady() const {
    return preprocessor != nullptr &&
           lexer != nullptr &&
           parser != nullptr &&
           syntaxAnalyzer != nullptr &&
           ruleEngine != nullptr;
}

std::vector<CodeMetric> AnalysisEngine::calculateMetrics(
    const std::string& rawCode,
    const std::string& cleanCode,
    const std::vector<Diagnostic>& diagnostics
) const {
    std::vector<CodeMetric> metrics;

    int criticalCount = 0;
    int warningCount = 0;
    int infoCount = 0;

    for (const Diagnostic& diagnostic : diagnostics) {
        if (diagnostic.getSeverity() == DiagnosticSeverity::CRITICAL) {
            criticalCount++;
        }
        else if (diagnostic.getSeverity() == DiagnosticSeverity::WARNING) {
            warningCount++;
        }
        else {
            infoCount++;
        }
    }

    metrics.emplace_back(
        "Toplam Satır",
        countLines(rawCode),
        "satır",
        "Orijinal kaynak dosyadaki satır sayısı"
    );

    metrics.emplace_back(
        "Temiz Kod Satırı",
        countLines(cleanCode),
        "satır",
        "Yorumlar çıkarıldıktan sonraki satır sayısı"
    );

    metrics.emplace_back(
        "Diagnostic Sayısı",
        static_cast<double>(diagnostics.size()),
        "adet",
        "Parser, syntax analyzer ve rule engine tarafından üretilen toplam bulgu"
    );

    metrics.emplace_back("Critical", criticalCount, "adet", "Kritik bulgu sayısı");
    metrics.emplace_back("Warning", warningCount, "adet", "Uyarı bulgu sayısı");
    metrics.emplace_back("Info", infoCount, "adet", "Bilgi bulgusu sayısı");

    return metrics;
}

int AnalysisEngine::countLines(const std::string& text) const {
    if (text.empty()) {
        return 0;
    }

    int lineCount = 1;
    for (char ch : text) {
        if (ch == '\n') {
            lineCount++;
        }
    }

    return lineCount;
}
