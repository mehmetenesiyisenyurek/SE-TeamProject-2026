#ifndef MAGIC_NUMBER_RULE_H
#define MAGIC_NUMBER_RULE_H

#include <string>
#include <vector>
#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../infrastructure/Diagnostic.h"

class MagicNumberRule : public IRule {
public:
    std::string getId() const override;
    std::string getName() const override;
    DiagnosticSeverity getDefaultSeverity() const override;
    std::vector<Diagnostic> check(const ASTNode& root) override;

private:
    void traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const;
    bool isNumber(const std::string& value) const;
    bool isIgnoredNumber(const std::string& value) const;
};

#endif
