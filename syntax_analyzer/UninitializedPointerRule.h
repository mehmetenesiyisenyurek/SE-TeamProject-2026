#ifndef SE_TEAMPROJECT_2026_UNINITIALIZEDPOINTERRULE_H
#define SE_TEAMPROJECT_2026_UNINITIALIZEDPOINTERRULE_H

#include <string>
#include <vector>

#include "IRule.h"
#include "../infrastructure/Diagnostic.h"

class ASTNode;

/*
 * UninitializedPointerRule:
 * Başlatılmamış pointer kullanımını tespit eder.
 * Kural ID: R012
 */
class UninitializedPointerRule : public IRule {
public:

    // Kural ID'sini döndürür
    std::string getId() const override;

    // Kural adını döndürür
    std::string getName() const override;

    // Kural açıklamasını döndürür
    std::string getDescription() const override;

    // Varsayılan severity seviyesini döndürür
    DiagnosticSeverity getDefaultSeverity() const override;

    // AST üzerinde pointer analizi yapar
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:

    // Başlatılmamış pointerları toplar
    void collectUninitializedPointers(
        const ASTNode& node,
        std::vector<std::string>& pointers
    ) const;

    // Pointer dereference kullanımını toplar
    void collectDereferencedPointers(
        const ASTNode& node,
        std::vector<std::string>& dereferencedPointers
    ) const;

    // Pointer'ın başlatılıp başlatılmadığını kontrol eder
    bool isPointerInitialized(
        const std::string& pointerName,
        const std::vector<std::string>& initializedPointers
    ) const;
};

#endif