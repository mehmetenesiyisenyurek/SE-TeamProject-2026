#ifndef SE_TEAMPROJECT_2026_SYNTAXANALYZER_H
#define SE_TEAMPROJECT_2026_SYNTAXANALYZER_H

#include <vector>

#include "../infrastructure/Diagnostic.h"

class ASTNode;
class Token;

/*
 * SyntaxAnalyzer:
 * C kaynak kodundaki temel sözdizimi hatalarını tespit eder.
 * Eksik noktalı virgül, eşleşmeyen parantez ve benzeri
 * syntax problemlerini Diagnostic listesi olarak döndürür.
 */
class SyntaxAnalyzer {
public:

    // AST ve token listesi üzerinden syntax analizi yapar
    std::vector<Diagnostic> analyze(
        const ASTNode& ast,
        const std::vector<Token>& tokens
    );
};

#endif