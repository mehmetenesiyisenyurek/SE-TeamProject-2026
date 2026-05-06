#ifndef SE_TEAMPROJECT_2026_RULEENGINE_H
#define SE_TEAMPROJECT_2026_RULEENGINE_H

#include <vector>

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

public:

    // Yeni bir kural ekler
    void addRule(IRule* rule);

    // Tüm kuralları çalıştırır
    std::vector<Diagnostic> analyzeAll(const ASTNode& ast);

    // Eklenen kural sayısını döndürür
    int getRuleCount() const;

    // Sistemdeki tüm kuralları temizler
    void clearRules();
};

#endif