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
        severity,
        "syntax",
        "",
        ""
    );
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

    return diagnostics;
}