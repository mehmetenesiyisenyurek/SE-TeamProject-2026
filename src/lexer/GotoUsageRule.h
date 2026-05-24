#ifndef GOTO_USAGE_RULE_H
#define GOTO_USAGE_RULE_H

#include <string>
#include <vector>
#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class GotoUsageRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& root) override;

private:
    void traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const;
};

#endif
