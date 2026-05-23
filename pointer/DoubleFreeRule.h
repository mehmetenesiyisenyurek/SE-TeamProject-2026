#ifndef DOUBLE_FREE_RULE_H
#define DOUBLE_FREE_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

class DoubleFreeRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
    std::vector<std::pair<std::string, int>> collectFreeCalls(const ASTNode& node) const;
    bool hasReallocationBetween(const std::string& varName, int line1, int line2, const ASTNode& scope) const;
};

#endif
