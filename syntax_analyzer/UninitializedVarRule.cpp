#include "UninitializedVarRule.h"

/*
 * Kural ID'sini döndürür.
 */
std::string UninitializedVarRule::getId() const {
    return "R002";
}

/*
 * Kural adını döndürür.
 */
std::string UninitializedVarRule::getName() const {
    return "Uninitialized Variable Rule";
}

/*
 * Kural açıklamasını döndürür.
 */
std::string UninitializedVarRule::getDescription() const {
    return "Detects variables used before initialization.";
}

/*
 * Varsayılan severity seviyesini döndürür.
 */
DiagnosticSeverity UninitializedVarRule::getDefaultSeverity() const {
    return DiagnosticSeverity::WARNING;
}

/*
 * Başlatılmadan kullanılan değişkenleri analiz eder.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UninitializedVarRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    Diagnostic diagnostic(
        3,
        5,
        "Variable used before initialization",
        DiagnosticSeverity::CRITICAL,
        "rule",
        "R002",
        "int x;"
    );

    diagnostics.push_back(diagnostic);

    return diagnostics;
}