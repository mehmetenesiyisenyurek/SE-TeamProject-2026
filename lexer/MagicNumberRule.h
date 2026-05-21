#ifndef MAGIC_NUMBER_RULE_H
#define MAGIC_NUMBER_RULE_H

#include <vector>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class MagicNumberRule : public IRule
{
private:
    void traverseAST(
        ASTNode* node,
        std::vector<Diagnostic>& diagnostics
    );

    bool isIgnoredNumber(const std::string& value) const;

public:
    std::vector<Diagnostic> check(ASTNode* root) override;
};

#endif