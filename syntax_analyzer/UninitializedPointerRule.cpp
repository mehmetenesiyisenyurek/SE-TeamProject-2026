#include "UninitializedPointerRule.h"

#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"

static bool isValidPointerName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    if (name == "NULL" || name == "nullptr" || name == "*") {
        return false;
    }

    return true;
}

std::string UninitializedPointerRule::getId() const {
    return "R012";
}

std::string UninitializedPointerRule::getName() const {
    return "Uninitialized Pointer Rule";
}

DiagnosticSeverity UninitializedPointerRule::getDefaultSeverity() const {
    return DiagnosticSeverity::CRITICAL;
}

std::vector<Diagnostic> UninitializedPointerRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;
    std::vector<std::string> pointers;
    std::vector<std::string> dereferencedPointers;

    collectUninitializedPointers(ast, pointers);
    collectDereferencedPointers(ast, dereferencedPointers);

    for (const std::string& pointer : dereferencedPointers) {
        if (isPointerInitialized(pointer, pointers)) {
            diagnostics.emplace_back(
                1,
                1,
                "Pointer '" + pointer + "' baslatilmadan dereference ediliyor.",
                getDefaultSeverity(),
                "rule",
                getId(),
                pointer
            );
        }
    }

    return diagnostics;
}

void UninitializedPointerRule::collectUninitializedPointers(
    const ASTNode& node,
    std::vector<std::string>& pointers
) const {
    if (node.getType() == ASTNodeType::VARIABLE_DECL && node.getChildren().empty()) {
        std::string pointerName = node.getValue();

        if (isValidPointerName(pointerName)) {
            pointers.push_back(pointerName);
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            collectUninitializedPointers(*child, pointers);
        }
    }
}

void UninitializedPointerRule::collectDereferencedPointers(
    const ASTNode& node,
    std::vector<std::string>& dereferencedPointers
) const {
    if (node.getType() == ASTNodeType::EXPRESSION && node.getValue() == "*") {
        const std::vector<ASTNode*>& children = node.getChildren();

        if (!children.empty() && children[0] != nullptr) {
            std::string pointerName = children[0]->getValue();

            if (isValidPointerName(pointerName)) {
                dereferencedPointers.push_back(pointerName);
            }
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            collectDereferencedPointers(*child, dereferencedPointers);
        }
    }
}

bool UninitializedPointerRule::isPointerInitialized(
    const std::string& pointerName,
    const std::vector<std::string>& initializedPointers
) const {
    for (const std::string& pointer : initializedPointers) {
        if (pointer == pointerName) {
            return true;
        }
    }

    return false;
}
