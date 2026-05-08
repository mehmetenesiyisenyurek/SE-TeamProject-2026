#ifndef SE_TEAMPROJECT_2026_SYNTAXANALYZER_H
#define SE_TEAMPROJECT_2026_SYNTAXANALYZER_H

#include <vector>

#include "../infrastructure/Diagnostic.h"

class ASTNode;
class Token;

/*
 * SyntaxAnalyzer:
 * C kaynak kodundaki temel sözdizimi hatalarını tespit eder.
 */
class SyntaxAnalyzer {
private:

    // Token listesi boşsa true döndürür
    bool isTokenListEmpty(const std::vector<Token>& tokens) const;

    // Syntax diagnostic nesnesi oluşturur
    Diagnostic createSyntaxDiagnostic(
        int line,
        int column,
        const std::string& message,
        DiagnosticSeverity severity
    ) const;

public:

    // AST ve token listesi üzerinden syntax analizi yapar
    std::vector<Diagnostic> analyze(
        const ASTNode& ast,
        const std::vector<Token>& tokens
    );
};

#endif