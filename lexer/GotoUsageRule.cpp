#include "GotoUsageRule.h"

std::string GotoUsageRule::getId() const { return "R005"; }
std::string GotoUsageRule::getName() const { return "Goto Usage Rule"; }
DiagnosticSeverity GotoUsageRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }

std::vector<Diagnostic> GotoUsageRule::check(const ASTNode& root) {
    std::vector<Diagnostic> diagnostics;
    traverseAST(root, diagnostics);
    return diagnostics;
}

void GotoUsageRule::traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const {
    if (node.getValue() == "goto") {
        diagnostics.emplace_back(node.getLine(), 1,
            "goto kullanimi tespit edildi. Yapisal kontrol akisi tercih edin.",
            getDefaultSeverity(), "rule", getId(), node.getValue());
    }
    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) traverseAST(*child, diagnostics);
    }
}
