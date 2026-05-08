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
 * Varsayılan severity seviyesini döndürür.
 */
DiagnosticSeverity UninitializedPointerRule::getDefaultSeverity() const {
    return DiagnosticSeverity::CRITICAL;
}

/*
 * Başlatılmamış pointer kullanımını analiz eder.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UninitializedPointerRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    std::vector<std::string> pointers;

    std::vector<std::string> dereferencedPointers;

    collectUninitializedPointers(ast, pointers);

    collectDereferencedPointers(ast, dereferencedPointers);

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

/*
 * Başlatılmamış pointerları toplar.
 */
void UninitializedPointerRule::collectUninitializedPointers(
    const ASTNode& node,
    std::vector<std::string>& pointers
) const {

}

/*
 * Pointer dereference kullanımlarını toplar.
 */
void UninitializedPointerRule::collectDereferencedPointers(
    const ASTNode& node,
    std::vector<std::string>& dereferencedPointers
) const {

}