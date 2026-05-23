#ifndef MEMORY_LEAK_RULE_H
#define MEMORY_LEAK_RULE_H

#include <string>
#include <vector>
#include <utility>
#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"

class MemoryLeakRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
    void collectAllocations(const ASTNode& node, std::vector<std::pair<std::string, int>>& allocations) const;
    bool hasFreeCall(const std::string& varName, const ASTNode& node) const;
    bool containsAllocationCall(const ASTNode& node) const;
};

#endif
