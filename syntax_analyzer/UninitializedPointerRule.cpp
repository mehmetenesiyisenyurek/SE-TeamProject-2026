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
 * Başlatılmamış pointer kullanımını analiz eder.
 * Şimdilik boş diagnostic listesi döndürülüyor.
 */
std::vector<Diagnostic> UninitializedPointerRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    return diagnostics;
}