#include "InfiniteLoopRule.h"
#include <cstdlib>

std::string InfiniteLoopRule::getId() const { return "R006"; }
std::string InfiniteLoopRule::getName() const { return "Infinite Loop Rule"; }
DiagnosticSeverity InfiniteLoopRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }

std::vector<Diagnostic> InfiniteLoopRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> results;
    traverse(ast, results);
    return results;
}

void InfiniteLoopRule::traverse(const ASTNode& node, std::vector<Diagnostic>& results) const {
    if (node.getType() == ASTNodeType::WHILE_LOOP && isWhileConditionConstant(node)) {
        results.emplace_back(node.getLine(), 1,
            "Potansiyel sonsuz dongu tespit edildi: while kosulu sabit.",
            getDefaultSeverity(), "rule", getId(), node.getValue());
    }
    if (node.getType() == ASTNodeType::FOR_LOOP && isForConditionEmpty(node)) {
        results.emplace_back(node.getLine(), 1,
            "Potansiyel sonsuz dongu tespit edildi: for dongusunde kosul yok.",
            getDefaultSeverity(), "rule", getId(), node.getValue());
    }
    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) traverse(*child, results);
    }
}

bool InfiniteLoopRule::isWhileConditionConstant(const ASTNode& whileNode) const {
    const auto& children = whileNode.getChildren();
    if (children.empty() || children[0] == nullptr) return false;
    return isConstantTruthy(*children[0]);
}

bool InfiniteLoopRule::isForConditionEmpty(const ASTNode& forNode) const {
    return forNode.getValue().empty();
}

bool InfiniteLoopRule::isConstantTruthy(const ASTNode& expr) const {
    const std::string v = expr.getValue();
    if (v == "true" || v == "1") return true;
    if (v.empty()) return false;
    char* end = nullptr;
    long n = std::strtol(v.c_str(), &end, 10);
    return end != v.c_str() && *end == '\0' && n != 0;
}
