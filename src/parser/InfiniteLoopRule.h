#ifndef INFINITELOOPRULE_H
#define INFINITELOOPRULE_H

#include <string>
#include <vector>
#include "../syntax_analyzer/IRule.h"
#include "ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class InfiniteLoopRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
    void traverse(const ASTNode& node, std::vector<Diagnostic>& results) const;
    bool isWhileConditionConstant(const ASTNode& whileNode) const;
    bool isForConditionEmpty(const ASTNode& forNode) const;
    bool isConstantTruthy(const ASTNode& expr) const;
};

#endif
