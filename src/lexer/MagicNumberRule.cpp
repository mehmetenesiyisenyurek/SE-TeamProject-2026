#include "MagicNumberRule.h"
#include <cctype>

std::string MagicNumberRule::getId() const { return "R003"; }
std::string MagicNumberRule::getName() const { return "Magic Number Rule"; }
DiagnosticSeverity MagicNumberRule::getDefaultSeverity() const { return DiagnosticSeverity::WARNING; }

std::vector<Diagnostic> MagicNumberRule::check(const ASTNode& root) {
    std::vector<Diagnostic> diagnostics;
    traverseAST(root, diagnostics);
    return diagnostics;
}

void MagicNumberRule::traverseAST(const ASTNode& node, std::vector<Diagnostic>& diagnostics) const {
    const std::string value = node.getValue();
    if (isNumber(value) && !isIgnoredNumber(value)) {
        diagnostics.emplace_back(node.getLine(), 1,
            "Magic number tespit edildi: " + value,
            getDefaultSeverity(), "rule", getId(), value);
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) traverseAST(*child, diagnostics);
    }
}

bool MagicNumberRule::isNumber(const std::string& value) const {
    if (value.empty()) return false;
    size_t i = (value[0] == '-') ? 1 : 0;
    bool digit = false;
    bool dot = false;
    for (; i < value.size(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(value[i]))) digit = true;
        else if (value[i] == '.' && !dot) dot = true;
        else return false;
    }
    return digit;
}

bool MagicNumberRule::isIgnoredNumber(const std::string& value) const {
    return value == "0" || value == "1" || value == "-1";
}
