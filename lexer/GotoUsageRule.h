#ifndef GOTO_USAGE_RULE_H
#define GOTO_USAGE_RULE_H

#include <vector>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class GotoUsageRule : public IRule
{
private:
    void traverseAST(
        ASTNode* node,
        std::vector<Diagnostic>& diagnostics
    );

public:
    std::vector<Diagnostic> check(ASTNode* root) override;
};

#endif