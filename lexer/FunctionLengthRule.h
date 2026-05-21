#ifndef FUNCTION_LENGTH_RULE_H
#define FUNCTION_LENGTH_RULE_H

#include <vector>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class FunctionLengthRule : public IRule
{
private:
    static const int MAX_FUNCTION_LINES = 50;

    void analyzeFunction(
        ASTNode* node,
        std::vector<Diagnostic>& diagnostics
    );

public:
    std::vector<Diagnostic> check(ASTNode* root) override;
};

#endif