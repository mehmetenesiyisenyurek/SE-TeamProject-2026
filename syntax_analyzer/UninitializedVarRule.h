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

    // Varsayılan severity seviyesini döndürür
    DiagnosticSeverity getDefaultSeverity() const override;

    // AST üzerinde başlatılmamış değişken analizi yapar
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:

    // Başlatılmamış değişkenleri toplar
    void collectUninitializedVars(
        const ASTNode& node,
        std::vector<std::string>& uninitializedVars
    ) const;

    // Kullanılan değişkenleri toplar
    void collectUsedVars(
        const ASTNode& node,
        std::vector<std::string>& usedVars
    ) const;

    // Değişkenin başlatılmamış değişkenler listesinde olup olmadığını kontrol eder
    bool isVariableInitialized(
        const std::string& variableName,
        const std::vector<std::string>& initializedVars
    ) const;
};

#endif