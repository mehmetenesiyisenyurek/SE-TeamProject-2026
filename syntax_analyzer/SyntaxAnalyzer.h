#ifndef SE_TEAMPROJECT_2026_SYNTAXANALYZER_H
#define SE_TEAMPROJECT_2026_SYNTAXANALYZER_H

#include <vector>
#include <string>

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

    // Syntax hata mesajına göre severity belirler
    DiagnosticSeverity determineSeverity(
        const std::string& message
    ) const;

    // Syntax diagnostic kaynağını döndürür
    std::string getSyntaxSource() const;

    // Eksik noktalı virgül kontrolü yapar
    void checkMissingSemicolon(
        const ASTNode& node,
        std::vector<Diagnostic>& diagnostics
    );

    // Eşleşmeyen süslü parantezleri kontrol eder
    void checkUnmatchedBrackets(
        const std::vector<Token>& tokens,
        std::vector<Diagnostic>& diagnostics
    );

    // Eşleşmeyen parantezleri kontrol eder
    void checkUnmatchedParentheses(
        const std::vector<Token>& tokens,
        std::vector<Diagnostic>& diagnostics
    );

    // Geçersiz değişken tanımlarını kontrol eder
    void checkInvalidDeclarations(
        const ASTNode& node,
        std::vector<Diagnostic>& diagnostics
    );

public:

    // AST ve token listesi üzerinden syntax analizi yapar
    std::vector<Diagnostic> analyze(
        const ASTNode& ast,
        const std::vector<Token>& tokens
    );
};

#endif