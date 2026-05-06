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
 * Kullanılmayan değişken analizini yapar.
 * Şimdilik örnek olarak boş diagnostic listesi döndürülüyor.
 */
std::vector<Diagnostic> UnusedVariableRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    return diagnostics;
}