#include "SyntaxAnalyzer.h"

/*
 * AST ve token listesi üzerinde syntax analizi yapar.
 * Token listesi boşsa temel syntax hatası üretir.
 */

/*
 * Token listesi boşsa true döndürür.
 */
bool SyntaxAnalyzer::isTokenListEmpty(const std::vector<Token>& tokens) const {
    return tokens.empty();
}

/*
 * Syntax diagnostic nesnesi oluşturur.
 */
Diagnostic SyntaxAnalyzer::createSyntaxDiagnostic(
    int line,
    int column,
    const std::string& message,
    DiagnosticSeverity severity
) const {

    return Diagnostic(
        line,
        column,
        message,
        determineSeverity(message),
        getSyntaxSource(),
        "",
        ""
    );
}

/*
 * Syntax hata mesajına göre severity belirler.
 */
DiagnosticSeverity SyntaxAnalyzer::determineSeverity(
    const std::string& message
) const {

    if (message.empty()) {
        return DiagnosticSeverity::WARNING;
    }

    return DiagnosticSeverity::CRITICAL;
}

std::vector<Diagnostic> SyntaxAnalyzer::analyze(
    const ASTNode& ast,
    const std::vector<Token>& tokens
) {

    std::vector<Diagnostic> diagnostics;

    /*
     * Token listesi boşsa kritik syntax hatası üret.
     */
    if (isTokenListEmpty(tokens)) {

        Diagnostic diagnostic = createSyntaxDiagnostic(
            1,
            1,
            "No tokens found for syntax analysis",
            DiagnosticSeverity::CRITICAL
        );

        diagnostics.push_back(diagnostic);

        return diagnostics;
    }

    checkMissingSemicolon(ast, diagnostics);

    checkUnmatchedBrackets(tokens, diagnostics);

    checkUnmatchedParentheses(tokens, diagnostics);

    checkInvalidDeclarations(ast, diagnostics);

    return diagnostics;
}

/*
 * Syntax diagnostic kaynağını döndürür.
 */
std::string SyntaxAnalyzer::getSyntaxSource() const {
    return "syntax";
}

/*
 * Eksik noktalı virgül kontrolü yapar.
 */
void SyntaxAnalyzer::checkMissingSemicolon(
    const ASTNode& node,
    std::vector<Diagnostic>& diagnostics
) {

}

/*
 * Eşleşmeyen süslü parantezleri kontrol eder.
 */
void SyntaxAnalyzer::checkUnmatchedBrackets(
    const std::vector<Token>& tokens,
    std::vector<Diagnostic>& diagnostics
) {

}

/*
 * Eşleşmeyen parantezleri kontrol eder.
 */
void SyntaxAnalyzer::checkUnmatchedParentheses(
    const std::vector<Token>& tokens,
    std::vector<Diagnostic>& diagnostics
) {

}

/*
 * Geçersiz değişken tanımlarını kontrol eder.
 */
void SyntaxAnalyzer::checkInvalidDeclarations(
    const ASTNode& node,
    std::vector<Diagnostic>& diagnostics
) {

}