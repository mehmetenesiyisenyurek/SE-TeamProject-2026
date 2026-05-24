#include "MemoryLeakRule.h"

std::string MemoryLeakRule::getId() const { return "R009"; }
std::string MemoryLeakRule::getName() const { return "Memory Leak Rule"; }
DiagnosticSeverity MemoryLeakRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }

std::vector<Diagnostic> MemoryLeakRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;
    if (ast.getType() == ASTNodeType::FUNCTION_DEF) {
        std::vector<std::pair<std::string, int>> allocations;
        collectAllocations(ast, allocations);
        for (const auto& allocation : allocations) {
            if (!hasFreeCall(allocation.first, ast)) {
                diagnostics.emplace_back(allocation.second, 1,
                    "Bellek ayrilmis ancak free edilmemis olabilir: " + allocation.first,
                    getDefaultSeverity(), "rule", getId(), allocation.first);
            }
        }
    }
    for (const ASTNode* child : ast.getChildren()) {
        if (child != nullptr) {
            auto sub = check(*child);
            diagnostics.insert(diagnostics.end(), sub.begin(), sub.end());
        }
    }
    return diagnostics;
}

void MemoryLeakRule::collectAllocations(const ASTNode& node, std::vector<std::pair<std::string, int>>& allocations) const {
    if ((node.getType() == ASTNodeType::VARIABLE_DECL || node.getType() == ASTNodeType::ASSIGNMENT) && containsAllocationCall(node)) {
        allocations.emplace_back(node.getValue(), node.getLine());
    }
    for (const ASTNode* child : node.getChildren()) if (child) collectAllocations(*child, allocations);
}

bool MemoryLeakRule::containsAllocationCall(const ASTNode& node) const {
    if (node.getType() == ASTNodeType::FUNCTION_CALL &&
        (node.getValue() == "malloc" || node.getValue() == "calloc" || node.getValue() == "realloc")) return true;
    for (const ASTNode* child : node.getChildren()) if (child && containsAllocationCall(*child)) return true;
    return false;
}

bool MemoryLeakRule::hasFreeCall(const std::string& varName, const ASTNode& node) const {
    if (node.getType() == ASTNodeType::FUNCTION_CALL && node.getValue() == "free") {
        const auto& ch = node.getChildren();
        if (!ch.empty() && ch[0] != nullptr && ch[0]->getValue() == varName) return true;
    }
    for (const ASTNode* child : node.getChildren()) if (child && hasFreeCall(varName, *child)) return true;
    return false;
}
