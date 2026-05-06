#ifndef SE_TEAMPROJECT_2026_UNUSEDVARIABLERULE_H
#define SE_TEAMPROJECT_2026_UNUSEDVARIABLERULE_H

#include <string>
#include <vector>

#include "IRule.h"
#include "../infrastructure/Diagnostic.h"

class ASTNode;

/*
 * UnusedVariableRule:
 * Tanımlanmış fakat kullanılmamış değişkenleri tespit eder.
 * Kural ID: R001
 */
class UnusedVariableRule : public IRule {
public:

    // Kural ID'sini döndürür
    std::string getId() const override;

    // Kural adını döndürür
    std::string getName() const override;

    // AST üzerinde kullanılmayan değişken analizi yapar
    std::vector<Diagnostic> check(const ASTNode& ast) override;
};

#endif