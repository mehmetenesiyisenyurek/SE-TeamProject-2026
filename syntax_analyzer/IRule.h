#ifndef SE_TEAMPROJECT_2026_IRULE_H
#define SE_TEAMPROJECT_2026_IRULE_H

#include <string>
#include <vector>

#include "ASTNode.h"
#include "Diagnostic.h"

class IRule {
public:
    virtual ~IRule() = default;

    virtual std::string getId() const = 0;

    virtual std::string getName() const = 0;

    virtual std::vector<Diagnostic> check(const ASTNode& ast) = 0;
};

#endif