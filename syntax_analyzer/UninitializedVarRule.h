#ifndef SE_TEAMPROJECT_2026_UNINITIALIZEDVARRULE_H
#define SE_TEAMPROJECT_2026_UNINITIALIZEDVARRULE_H

#include <string>
#include <vector>

#include "IRule.h"
#include "../infrastructure/Diagnostic.h"

class ASTNode;

/*
 * UninitializedVarRule:
 * Başlatılmadan kullanılan değişkenleri tespit eder.
 * Kural ID: R002
 */
class UninitializedVarRule : public IRule {
public:

    // Kural ID'sini döndürür
    std::string getId() const override;

    // Kural adını döndürür
    std::string getName() const override;

    // Kural açıklamasını döndürür
    std::string getDescription() const override;

    // Varsayılan severity seviyesini döndürür
    DiagnosticSeverity getDefaultSeverity() const override;

    // AST üzerinde başlatılmamış değişken analizi yapar
    std::vector<Diagnostic> check(const ASTNode& ast) override;
};

#endif