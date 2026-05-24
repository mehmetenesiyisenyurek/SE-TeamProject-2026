#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include "Token.h"
#include "ASTNode.h"
#include "Diagnostic.h"


class Parser {
private:
    std::vector<Token> tokens;         // Lexer'dan gelen token listesi
    int pos;                           // Şu anki token pozisyonu
    ASTNode* root;                     // AST kök düğümü
    std::vector<Diagnostic> diagnostics; // Ayrıştırma hataları



    // Şu anki token'ı döndürür (pozisyon değişmez)
    Token current() const;

    // Şu anki token'ı döndürür ve pozisyonu 1 ileri alır
    Token advance();

    // Şu anki token beklenen tipteyse döndürür ve ilerler.
    // Değilse diagnostics'e hata ekler.
    Token expect(TokenType type);

    // Şu anki token beklenen tipteyse true döndürür ve ilerler.
    // Değilse false döndürür (hata üretmez).
    bool match(TokenType type);

    // Şu anki token beklenen değere sahipse true döndürür ve ilerler.
    bool matchValue(const std::string& val);

    // Token listesinin sonuna gelindi mi?
    bool isAtEnd() const;

    // Bir sonraki ; veya } karakterine kadar atla
    void synchronize();

    // Hata üretir ve diagnostics listesine ekler
    void reportError(const std::string& message);

    // Verilen string bir C tipi mi?
    bool isTypeName(const std::string& val) const;



    // Üst düzey: fonksiyon tanımları, global değişkenler, struct'lar
    void parseProgram();

    // int func(params) { body }  →  FUNCTION_DEF düğümü
    ASTNode* parseFunctionDef(const std::string& typeName);

    // { statement; statement; ... }  →  BLOCK düğümü
    ASTNode* parseBlock();

    // Tek bir ifade: if, while, for, return, değişken tanımı, atama veya ifade
    ASTNode* parseStatement();

    // Değişken tanımı: int x; veya int x = expr;  veya  int *p;
    ASTNode* parseVarDecl(const std::string& typeName, bool isPointer = false);

    // if (koşul) { ... } else { ... }
    ASTNode* parseIfStatement();

    // while (koşul) { ... }
    ASTNode* parseWhileLoop();

    // for (init; koşul; güncelleme) { ... }
    ASTNode* parseForLoop();

    // return expr;
    ASTNode* parseReturnStmt();

    // struct İsim { üyeler };
    ASTNode* parseStructDef();




    // En düşük öncelik → En yüksek öncelik sırasıyla:
    ASTNode* parseExpression();      // Giriş noktası
    ASTNode* parseAssignmentExpr();  // = (sağdan sola)
    ASTNode* parseComparison();      // ==, !=, <, >, <=, >=
    ASTNode* parseAddition();        // +, -
    ASTNode* parseMultiplication();  // *, /, %
    ASTNode* parseUnary();           // !, -, *, & (tekli operatörler)
    ASTNode* parsePrimary();         // Literal, identifier, fonksiyon çağrısı, (expr)
public:
    Parser();
    // Ana metot. Token listesini alır, AST ağacı döndürür.
    // Hatalarda bile kısmi AST üretir.
    ASTNode* parse(const std::vector<Token>& tokenList);
    // Ayrıştırma sırasında bulunan hataları döndürür.
    const std::vector<Diagnostic>& getDiagnostics() const;
};
#endif // PARSER_H