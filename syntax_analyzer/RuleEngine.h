#ifndef SE_TEAMPROJECT_2026_RULEENGINE_H
#define SE_TEAMPROJECT_2026_RULEENGINE_H

#include <vector>
#include <memory>

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
};

#endif