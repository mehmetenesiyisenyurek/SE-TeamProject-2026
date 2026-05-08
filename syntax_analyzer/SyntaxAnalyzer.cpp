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

std::vector<Diagnostic> SyntaxAnalyzer::analyze(
    const ASTNode& ast,
    const std::vector<Token>& tokens
) {

    std::vector<Diagnostic> diagnostics;

    if (isTokenListEmpty(tokens)) {
        Diagnostic diagnostic(
            1,
            1,
            "No tokens found for syntax analysis",
            DiagnosticSeverity::CRITICAL,
            "syntax",
            "",
            ""
        );

        diagnostics.push_back(diagnostic);
    }

    return diagnostics;
}