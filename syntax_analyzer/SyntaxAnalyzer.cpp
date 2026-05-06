#include "SyntaxAnalyzer.h"

/*
 * AST ve token listesi üzerinde syntax analizi yapar.
 * Şimdilik boş diagnostic listesi döndürülüyor.
 */
std::vector<Diagnostic> SyntaxAnalyzer::analyze(
    const ASTNode& ast,
    const std::vector<Token>& tokens
) {

    std::vector<Diagnostic> diagnostics;

    return diagnostics;
}