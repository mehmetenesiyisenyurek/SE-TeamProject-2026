#ifndef SE_TEAMPROJECT_2026_IRULE_H
#define SE_TEAMPROJECT_2026_IRULE_H

#include <string>
#include <vector>

#include "../infrastructure/Diagnostic.h"

class ASTNode;

/*
 * IRule:
 * Tüm analiz kuralları için temel arayüzdür.
 * Her kural bu sınıftan türetilir.
 */
class IRule {
public:

    // Sanal destructor
    virtual ~IRule() = default;

    // Kural ID'sini döndürür (Örn: R001)
    virtual std::string getId() const = 0;

    // Kural adını döndürür
    virtual std::string getName() const = 0;

    // AST üzerinde analiz yapar ve bulguları döndürür
    virtual std::vector<Diagnostic> check(const ASTNode& ast) = 0;
};

#endif