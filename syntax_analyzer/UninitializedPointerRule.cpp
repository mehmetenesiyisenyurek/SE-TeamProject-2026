#include "UninitializedPointerRule.h"

/*
 * Kural ID'sini döndürür.
 */
std::string UninitializedPointerRule::getId() const {
    return "R012";
}

/*
 * Kural adını döndürür.
 */
std::string UninitializedPointerRule::getName() const {
    return "Uninitialized Pointer Rule";
}

/*
 * Kural açıklamasını döndürür.
 */
std::string UninitializedPointerRule::getDescription() const {
    return "Detects uninitialized pointer usage.";
}

/*
 * Başlatılmamış pointer kullanımını analiz eder.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UninitializedPointerRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    Diagnostic diagnostic(
        7,
        3,
        "Uninitialized pointer detected",
        DiagnosticSeverity::CRITICAL,
        "rule",
        "R012",
        "int* ptr;"
    );

    diagnostics.push_back(diagnostic);

    return diagnostics;
}