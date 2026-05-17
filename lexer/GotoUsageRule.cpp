#include "GotoUsageRule.h"

std::vector<Diagnostic> GotoUsageRule::check(ASTNode* root)
{
    std::vector<Diagnostic> diagnostics;

    if (root == nullptr)
    {
        return diagnostics;
    }

    traverseAST(root, diagnostics);

    return diagnostics;
}

void GotoUsageRule::traverseAST(
    ASTNode* node,
    std::vector<Diagnostic>& diagnostics
)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->getType() == "GOTO_STATEMENT")
    {
        diagnostics.push_back(
            Diagnostic(
                "Usage of goto statement detected",
                DiagnosticSeverity::WARNING,
                node->getLine(),
                node->getColumn()
            )
        );
    }

    for (ASTNode* child : node->getChildren())
    {
        traverseAST(child, diagnostics);
    }
}