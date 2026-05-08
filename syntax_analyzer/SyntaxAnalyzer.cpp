#include "SyntaxAnalyzer.h"
//#include "Token.h"

#include <stack>
/*
 * AST ve token listesi üzerinde syntax analizi yapar.
 * Token listesi boşsa temel syntax hatası üretir.
 */

/*
 * Token listesi boşsa true döndürür.
 */
bool SyntaxAnalyzer::isTokenListEmpty(const std::vector<Token>& tokens) const {
    return tokens.empty();
}

/*
 * Syntax diagnostic nesnesi oluşturur.
 */
Diagnostic SyntaxAnalyzer::createSyntaxDiagnostic(
    int line,
    int column,
    const std::string& message,
    DiagnosticSeverity severity
) const {

    return Diagnostic(
        line,
        column,
        message,
        determineSeverity(message),
        getSyntaxSource(),
        "",
        ""
    );
}

/*
 * Syntax hata mesajına göre severity belirler.
 */
DiagnosticSeverity SyntaxAnalyzer::determineSeverity(
    const std::string& message
) const {

    if (message.empty()) {
        return DiagnosticSeverity::WARNING;
    }

    return DiagnosticSeverity::CRITICAL;
}

std::vector<Diagnostic> SyntaxAnalyzer::analyze(
    const ASTNode& ast,
    const std::vector<Token>& tokens
) {

    std::vector<Diagnostic> diagnostics;

    /*
     * Token listesi boşsa kritik syntax hatası üret.
     */
    if (isTokenListEmpty(tokens)) {

        Diagnostic diagnostic = createSyntaxDiagnostic(
            1,
            1,
            "No tokens found for syntax analysis",
            DiagnosticSeverity::CRITICAL
        );

        diagnostics.push_back(diagnostic);

        return diagnostics;
    }

    checkMissingSemicolon(ast, diagnostics);

    checkUnmatchedBrackets(tokens, diagnostics);

    checkUnmatchedParentheses(tokens, diagnostics);

    checkInvalidDeclarations(ast, diagnostics);

    return diagnostics;
}

/*
 * Syntax diagnostic kaynağını döndürür.
 */
std::string SyntaxAnalyzer::getSyntaxSource() const {
    return "syntax";
}

/*
 * Eksik noktalı virgül kontrolü yapar.
 * TODO: ASTNode tamamlandığında VARIABLE_DECL, RETURN_STMT,
 * ASSIGNMENT ve FUNCTION_CALL düğümleri üzerinde ';' kontrolü yapılacak.
 */
void SyntaxAnalyzer::checkMissingSemicolon(
    const ASTNode& node,
    std::vector<Diagnostic>& diagnostics
) {

    /*
    if (
        node.getType() == ASTNodeType::VARIABLE_DECL ||
        node.getType() == ASTNodeType::RETURN_STMT ||
        node.getType() == ASTNodeType::EXPRESSION ||
        node.getType() == ASTNodeType::FUNCTION_CALL
    ) {

        if (!node.endsWithSemicolon()) {

            Diagnostic diagnostic = createSyntaxDiagnostic(
                node.getLine(),
                1,
                "Missing semicolon",
                DiagnosticSeverity::CRITICAL
            );

            diagnostics.push_back(diagnostic);
        }
    }

    for (const ASTNode& child : node.getChildren()) {
        checkMissingSemicolon(child, diagnostics);
    }
    */
}

/*
 * Eşleşmeyen süslü parantezleri kontrol eder.
 * TODO: Token sınıfı tamamlandığında stack kullanılarak '{' ve '}'
 * eşleşmeleri kontrol edilecek.
 */
void SyntaxAnalyzer::checkUnmatchedBrackets(
    const std::vector<Token>& tokens,
    std::vector<Diagnostic>& diagnostics
) {

    /*
    std::stack<Token> bracketStack;

    for (const Token& token : tokens) {

        if (token.getValue() == "{") {
            bracketStack.push(token);
        }
        else if (token.getValue() == "}") {

            if (bracketStack.empty()) {

                Diagnostic diagnostic = createSyntaxDiagnostic(
                    token.getLine(),
                    token.getColumn(),
                    "Unmatched closing bracket",
                    DiagnosticSeverity::CRITICAL
                );

                diagnostics.push_back(diagnostic);
            }
            else {
                bracketStack.pop();
            }
        }
    }

    while (!bracketStack.empty()) {

        Token token = bracketStack.top();
        bracketStack.pop();

        Diagnostic diagnostic = createSyntaxDiagnostic(
            token.getLine(),
            token.getColumn(),
            "Unmatched opening bracket",
            DiagnosticSeverity::CRITICAL
        );

        diagnostics.push_back(diagnostic);
    }
    */
}

/*
 * Eşleşmeyen parantezleri kontrol eder.
 * TODO: Token sınıfı tamamlandığında stack kullanılarak '(', ')',
 * '[' ve ']' eşleşmeleri kontrol edilecek.
 */
void SyntaxAnalyzer::checkUnmatchedParentheses(
    const std::vector<Token>& tokens,
    std::vector<Diagnostic>& diagnostics
) {

    /*
    std::stack<Token> parenthesisStack;

    for (const Token& token : tokens) {

        if (token.getValue() == "(" || token.getValue() == "[") {
            parenthesisStack.push(token);
        }
        else if (token.getValue() == ")" || token.getValue() == "]") {

            if (parenthesisStack.empty()) {

                Diagnostic diagnostic = createSyntaxDiagnostic(
                    token.getLine(),
                    token.getColumn(),
                    "Unmatched closing parenthesis",
                    DiagnosticSeverity::CRITICAL
                );

                diagnostics.push_back(diagnostic);
            }
            else {
                Token openingToken = parenthesisStack.top();

                bool matchesRound =
                    openingToken.getValue() == "(" &&
                    token.getValue() == ")";

                bool matchesSquare =
                    openingToken.getValue() == "[" &&
                    token.getValue() == "]";

                if (matchesRound || matchesSquare) {
                    parenthesisStack.pop();
                }
                else {
                    Diagnostic diagnostic = createSyntaxDiagnostic(
                        token.getLine(),
                        token.getColumn(),
                        "Mismatched parenthesis type",
                        DiagnosticSeverity::CRITICAL
                    );

                    diagnostics.push_back(diagnostic);
                    parenthesisStack.pop();
                }
            }
        }
    }

    while (!parenthesisStack.empty()) {

        Token token = parenthesisStack.top();
        parenthesisStack.pop();

        Diagnostic diagnostic = createSyntaxDiagnostic(
            token.getLine(),
            token.getColumn(),
            "Unmatched opening parenthesis",
            DiagnosticSeverity::CRITICAL
        );

        diagnostics.push_back(diagnostic);
    }
    */
}

/*
 * Geçersiz değişken tanımlarını kontrol eder.
 * TODO: ASTNode tamamlandığında tip bilgisi olmayan veya hatalı
 * VARIABLE_DECL düğümleri kontrol edilecek.
 */
void SyntaxAnalyzer::checkInvalidDeclarations(
    const ASTNode& node,
    std::vector<Diagnostic>& diagnostics
) {

    /*
    if (node.getType() == ASTNodeType::VARIABLE_DECL) {

        if (node.getValue().empty()) {

            Diagnostic diagnostic = createSyntaxDiagnostic(
                node.getLine(),
                1,
                "Invalid variable declaration",
                DiagnosticSeverity::CRITICAL
            );

            diagnostics.push_back(diagnostic);
        }
    }

    for (const ASTNode& child : node.getChildren()) {
        checkInvalidDeclarations(child, diagnostics);
    }
    */
}