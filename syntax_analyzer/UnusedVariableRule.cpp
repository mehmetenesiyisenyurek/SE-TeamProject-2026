#include "UnusedVariableRule.h"

/*
 * Kural ID'sini döndürür.
 */
std::string UnusedVariableRule::getId() const {
    return "R001";
}

/*
 * Kural adını döndürür.
 */
std::string UnusedVariableRule::getName() const {
    return "Unused Variable Rule";
}

/*
 * Kural açıklamasını döndürür.
 */
std::string UnusedVariableRule::getDescription() const {
    return "Detects variables that are declared but never used.";
}

/*
 * Kullanılmayan değişken analizini yapar.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UnusedVariableRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    Diagnostic diagnostic(
        1,
        1,
        "Unused variable detected",
        DiagnosticSeverity::WARNING,
        "rule",
        "R001",
        "int temp;"
    );

    diagnostics.push_back(diagnostic);

    return diagnostics;
}