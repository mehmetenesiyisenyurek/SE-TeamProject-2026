#include "DoubleFreeRule.h"

std::string DoubleFreeRule::getId() const { return "R010"; }
std::string DoubleFreeRule::getName() const { return "Double Free Rule"; }
DiagnosticSeverity DoubleFreeRule::getDefaultSeverity() const { return DiagnosticSeverity::CRITICAL; }

std::vector<Diagnostic> DoubleFreeRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;

    if (ast.getType() == ASTNodeType::FUNCTION_DEF) {
        auto freeCalls = collectFreeCalls(ast);
        for (size_t i = 0; i < freeCalls.size(); ++i) {
            for (size_t j = i + 1; j < freeCalls.size(); ++j) {
                if (freeCalls[i].first == freeCalls[j].first &&
                    !hasReallocationBetween(freeCalls[i].first, freeCalls[i].second, freeCalls[j].second, ast)) {
                    diagnostics.emplace_back(
                        freeCalls[j].second,
                        1,
                        "Ayni pointer ikinci kez free ediliyor: " + freeCalls[i].first,
                        getDefaultSeverity(),
                        "rule",
                        getId(),
                        freeCalls[i].first
                    );
                }
            }
        }
    }

    for (const ASTNode* child : ast.getChildren()) {
        if (child != nullptr) {
            auto childDiagnostics = check(*child);
            diagnostics.insert(diagnostics.end(), childDiagnostics.begin(), childDiagnostics.end());
        }
    }
    return diagnostics;
}

std::vector<std::pair<std::string, int>> DoubleFreeRule::collectFreeCalls(const ASTNode& node) const {
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

bool DoubleFreeRule::hasReallocationBetween(const std::string& varName, int line1, int line2, const ASTNode& scope) const {
    if (scope.getLine() > line1 && scope.getLine() < line2 &&
        (scope.getType() == ASTNodeType::ASSIGNMENT || scope.getType() == ASTNodeType::VARIABLE_DECL) &&
        scope.getValue() == varName) {
        for (const ASTNode* child : scope.getChildren()) {
            if (child != nullptr && child->getType() == ASTNodeType::FUNCTION_CALL &&
                (child->getValue() == "malloc" || child->getValue() == "calloc" || child->getValue() == "realloc")) {
                return true;
            }
        }
    }
    for (const ASTNode* child : scope.getChildren()) {
        if (child != nullptr && hasReallocationBetween(varName, line1, line2, *child)) return true;
    }
    return false;
}
