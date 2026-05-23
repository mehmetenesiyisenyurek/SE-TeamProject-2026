#include "FunctionLengthRule.h"
#include <algorithm>

FunctionLengthRule::FunctionLengthRule(int maxLines) : maxLines(maxLines) {}
std::string FunctionLengthRule::getId() const { return "R004"; }
std::string FunctionLengthRule::getName() const { return "Function Length Rule"; }
DiagnosticSeverity FunctionLengthRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }
void FunctionLengthRule::setMaxLines(int max) { maxLines = max; }

std::vector<Diagnostic> FunctionLengthRule::check(const ASTNode& root) {
    std::vector<Diagnostic> diagnostics;
    traverseAST(root, diagnostics);
    return diagnostics;
}

void FunctionLengthRule::traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const {
    if (node.getType() == ASTNodeType::FUNCTION_DEF) {
        int length = std::max(1, findMaxLine(node) - node.getLine() + 1);
        if (length > maxLines) {
            diagnostics.emplace_back(node.getLine(), 1,
                "Fonksiyon '" + node.getValue() + "' " + std::to_string(length) +
                " satir. Daha kucuk fonksiyonlara bolun.",
                getDefaultSeverity(), "rule", getId(), node.getValue());
        }
    }
    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) traverseAST(*child, diagnostics);
    }
}

int FunctionLengthRule::findMaxLine(const ASTNode& node) const {
    int maxLine = node.getLine();
    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) maxLine = std::max(maxLine, findMaxLine(*child));
    }
    return maxLine;
}
