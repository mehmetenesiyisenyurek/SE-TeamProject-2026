#ifndef FUNCTION_LENGTH_RULE_H
#define FUNCTION_LENGTH_RULE_H

#include <string>
#include <vector>
#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"

class FunctionLengthRule : public IRule {
private:
    int maxLines;

public:
    explicit FunctionLengthRule(int maxLines = 50);
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& root) override;
    void setMaxLines(int max);

private:
    void traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const;
    int findMaxLine(const ASTNode& node) const;
};

#endif
