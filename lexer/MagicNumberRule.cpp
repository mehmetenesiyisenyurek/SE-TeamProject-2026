#include "MagicNumberRule.h"

std::vector<Diagnostic> MagicNumberRule::check(ASTNode* root)
{
    std::vector<Diagnostic> diagnostics;

    if (root == nullptr)
    {
        return diagnostics;
    }

    traverseAST(root, diagnostics);

    return diagnostics;
}

void MagicNumberRule::traverseAST(
    ASTNode* node,
    std::vector<Diagnostic>& diagnostics
)
{
    if (node == nullptr)
    {
        return;
    }

    if (
        node->getType() == "INT_LITERAL" ||
        node->getType() == "FLOAT_LITERAL"
    )
    {
        std::string value = node->getValue();

        if (!isIgnoredNumber(value))
        {
            diagnostics.push_back(
                Diagnostic(
                    "Magic number detected: " + value,
                    DiagnosticSeverity::WARNING,
                    node->getLine(),
                    node->getColumn()
                )
            );
        }
    }

    for (ASTNode* child : node->getChildren())
    {
        traverseAST(child, diagnostics);
    }
}

bool MagicNumberRule::isIgnoredNumber(const std::string& value) const
{
    return
        value == "0" ||
        value == "1" ||
        value == "-1";
}