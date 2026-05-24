#ifndef DANGLING_POINTER_RULE_H
#define DANGLING_POINTER_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

class DanglingPointerRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
    std::vector<std::pair<std::string, int>> collectFreeCalls(const ASTNode& node) const;
    bool hasNullAssignmentAfterFree(const std::string& varName, int freeLine, const ASTNode& scope) const;
};

#endif
