#include "UseAfterFreeRule.h"

std::string UseAfterFreeRule::getId() const {
    return "R008";
}

std::string UseAfterFreeRule::getName() const {
    return "Use After Free Rule";
}

DiagnosticSeverity UseAfterFreeRule::getDefaultSeverity() const {
    return DiagnosticSeverity::CRITICAL;
}

std::vector<Diagnostic> UseAfterFreeRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;

    if (ast.getType() == ASTNodeType::FUNCTION_DEF) {
        std::vector<std::pair<std::string, int>> freeCalls = findFreeCallSites(ast);

        for (const auto& freeCall : freeCalls) {
            const std::string& variableName = freeCall.first;
            int freeLine = freeCall.second;

            if (!variableName.empty() && isUsedAfterFree(variableName, freeLine, ast)) {
                diagnostics.emplace_back(
                    freeLine,
                    1,
                    "free edildikten sonra pointer tekrar kullaniliyor: " + variableName,
                    getDefaultSeverity(),
                    "rule",
                    getId(),
                    variableName
                );
            }
        }
    }

    for (const ASTNode* child : ast.getChildren()) {
        if (child != nullptr) {
            std::vector<Diagnostic> childDiagnostics = check(*child);
            diagnostics.insert(diagnostics.end(), childDiagnostics.begin(), childDiagnostics.end());
        }
    }

    return diagnostics;
}

std::vector<std::pair<std::string, int>> UseAfterFreeRule::findFreeCallSites(const ASTNode& node) const {
    std::vector<std::pair<std::string, int>> freeCalls;

    if (node.getType() == ASTNodeType::FUNCTION_CALL && node.getValue() == "free") {
        const std::vector<ASTNode*>& children = node.getChildren();

        if (!children.empty() && children[0] != nullptr) {
            freeCalls.emplace_back(children[0]->getValue(), node.getLine());
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            std::vector<std::pair<std::string, int>> childResults = findFreeCallSites(*child);
            freeCalls.insert(freeCalls.end(), childResults.begin(), childResults.end());
        }
    }

    return freeCalls;
}

bool UseAfterFreeRule::isUsedAfterFree(
    const std::string& varName,
    int freeLine,
    const ASTNode& scope
) const {
    if (scope.getLine() > freeLine) {
        if (isNullAssignment(varName, scope)) {
            return false;
        }

        if (scope.getValue() == varName) {
            return true;
        }
    }

    for (const ASTNode* child : scope.getChildren()) {
        if (child != nullptr && isUsedAfterFree(varName, freeLine, *child)) {
            return true;
        }
    }

    return false;
}

bool UseAfterFreeRule::isNullAssignment(const std::string& varName, const ASTNode& node) const {
    if (node.getType() == ASTNodeType::ASSIGNMENT && node.getValue() == varName) {
        for (const ASTNode* child : node.getChildren()) {
            if (child != nullptr && child->getValue() == "NULL") {
                return true;
            }
        }
    }

    return false;
}