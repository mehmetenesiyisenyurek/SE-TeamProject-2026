#ifndef DOUBLE_FREE_RULE_H
#define DOUBLE_FREE_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

class DoubleFreeRule {
public:
    std::vector<Diagnostic> check(ASTNode* ast);

private:
    std::vector<std::pair<std::string, int>> collectFreeCalls(ASTNode* func);

    bool hasReallocationBetween(
            const std::string& varName,
            int line1,
            int line2,
            ASTNode* scope
    );
};

#endif