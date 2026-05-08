#ifndef SE_TEAMPROJECT_2026_RULEENGINE_H
#define SE_TEAMPROJECT_2026_RULEENGINE_H

#include <vector>
#include <memory>
#include <string>

#include "IRule.h"
#include "../infrastructure/Diagnostic.h"

class ASTNode;

/*
 * RuleEngine:
 * Sistemdeki tüm kuralları çalıştıran kontrol sınıfıdır.
 */
class RuleEngine {
private:

    // Sisteme eklenen analiz kuralları
    std::vector<IRule*> rules;

    // RuleEngine'in sahip olduğu varsayılan kurallar
    std::vector<std::unique_ptr<IRule>> ownedRules;

    // Varsayılan kuralları sisteme ekler
    void registerDefaultRules();

    // Kritik diagnostic sayısını döndürür
    int countCriticalDiagnostics(
        const std::vector<Diagnostic>& diagnostics
    ) const;

public:

    // RuleEngine oluşturulduğunda varsayılan kuralları hazırlar
    RuleEngine();

    // Yeni bir kural ekler
    void addRule(IRule* rule);

    // Tüm kuralları çalıştırır ve diagnostikleri birleştirir
    std::vector<Diagnostic> analyzeAll(const ASTNode& ast);

    // Eklenen kural sayısını döndürür
    int getRuleCount() const;

    // Sistemdeki tüm kuralları temizler
    void clearRules();

    // Diagnostic listesinde bulgu olup olmadığını kontrol eder
    bool hasDiagnostics(
        const std::vector<Diagnostic>& diagnostics
    ) const;

    // Warning seviyesindeki diagnostic sayısını döndürür
    int countWarningDiagnostics(
        const std::vector<Diagnostic>& diagnostics
    ) const;

    // Verilen ID'ye sahip kuralın ekli olup olmadığını kontrol eder
    bool hasRule(const std::string& ruleId) const;

    // Verilen ID'ye sahip kuralı listeden kaldırır
    bool removeRuleById(const std::string& ruleId);

    // Sistemde kayıtlı tüm kural ID'lerini döndürür
    std::vector<std::string> getRuleIds() const;

    // Sistemde kayıtlı tüm kural isimlerini döndürür
    std::vector<std::string> getRuleNames() const;
};

#endif