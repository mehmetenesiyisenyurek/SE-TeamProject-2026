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
 * Başlatılmadan kullanılan değişkenleri analiz eder.
 * Şimdilik boş diagnostic listesi döndürülüyor.
 */
std::vector<Diagnostic> UninitializedVarRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    return diagnostics;
}