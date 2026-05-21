#include "FunctionLengthRule.h"

std::vector<Diagnostic> FunctionLengthRule::check(ASTNode* root)
{
    std::vector<Diagnostic> diagnostics;

    if (root == nullptr)
    {
        return diagnostics;
    }

    analyzeFunction(root, diagnostics);

    return diagnostics;
}

void FunctionLengthRule::analyzeFunction(
    ASTNode* node,
    std::vector<Diagnostic>& diagnostics
)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->getType() == "FUNCTION_DECLARATION")
    {
        int startLine = node->getLine();
        int endLine = node->getEndLine();

        int functionLength = endLine - startLine + 1;

        if (functionLength > MAX_FUNCTION_LINES)
        {
            diagnostics.push_back(
                Diagnostic(
                    "Function exceeds maximum recommended length",
                    DiagnosticSeverity::WARNING,
                    startLine,
                    node->getColumn()
                )
            );
        }
    }

    for (ASTNode* child : node->getChildren())
    {
        analyzeFunction(child, diagnostics);
    }
}