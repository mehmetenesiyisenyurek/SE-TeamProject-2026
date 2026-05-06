#include "SyntaxAnalyzer.h"

/*
 * AST ve token listesi üzerinde syntax analizi yapar.
 * Şimdilik örnek syntax diagnostic üretir.
 */
std::vector<Diagnostic> SyntaxAnalyzer::analyze(
    const ASTNode& ast,
    const std::vector<Token>& tokens
) {

    std::vector<Diagnostic> diagnostics;

    Diagnostic diagnostic(
        10,
        12,
        "Missing semicolon",
        DiagnosticSeverity::CRITICAL,
        "syntax",
        "",
        "int number = 5"
    );

    diagnostics.push_back(diagnostic);

    return diagnostics;
}