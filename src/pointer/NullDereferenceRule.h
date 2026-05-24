#ifndef NULL_DEREFERENCE_RULE_H
#define NULL_DEREFERENCE_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

class NullDereferenceRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
    std::vector<std::pair<std::string, int>> findMallocAssignments(const ASTNode& node) const;
    bool containsAllocationCall(const ASTNode& node) const;
    bool hasNullCheck(const std::string& varName, const ASTNode& scope) const;
};

#endif
