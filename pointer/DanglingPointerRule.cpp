#include "DanglingPointerRule.h"

std::string DanglingPointerRule::getId() const { return "R011"; }
std::string DanglingPointerRule::getName() const { return "Dangling Pointer Rule"; }
DiagnosticSeverity DanglingPointerRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }

std::vector<Diagnostic> DanglingPointerRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;
    if (ast.getType() == ASTNodeType::FUNCTION_DEF) {
        auto calls = collectFreeCalls(ast);
        for (const auto& c : calls) {
            if (!hasNullAssignmentAfterFree(c.first, c.second, ast)) {
                diagnostics.emplace_back(c.second, 1,
                    "free sonrasi pointer NULL yapilmamis: " + c.first,
                    getDefaultSeverity(), "rule", getId(), c.first);
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

std::vector<std::pair<std::string, int>> DanglingPointerRule::collectFreeCalls(const ASTNode& node) const {
    std::vector<std::pair<std::string, int>> calls;
    if (node.getType() == ASTNodeType::FUNCTION_CALL && node.getValue() == "free") {
        const auto& ch = node.getChildren();
        if (!ch.empty() && ch[0] != nullptr) calls.emplace_back(ch[0]->getValue(), node.getLine());
    }
    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            auto sub = collectFreeCalls(*child);
            calls.insert(calls.end(), sub.begin(), sub.end());
        }
    }
    return calls;
}

bool DanglingPointerRule::hasNullAssignmentAfterFree(const std::string& varName, int freeLine, const ASTNode& scope) const {
    if (scope.getLine() > freeLine && scope.getType() == ASTNodeType::ASSIGNMENT && scope.getValue() == varName) {
        for (const ASTNode* child : scope.getChildren()) {
            if (child != nullptr && child->getValue() == "NULL") return true;
        }
    }
    for (const ASTNode* child : scope.getChildren()) {
        if (child != nullptr && hasNullAssignmentAfterFree(varName, freeLine, *child)) return true;
    }
    return false;
}
