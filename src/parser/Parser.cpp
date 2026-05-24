#include "Parser.h"
#include <iostream>


Parser::Parser() : pos(0), root(nullptr) {}


ASTNode* Parser::parse(const std::vector<Token>& tokenList) {
    tokens = tokenList;
    pos = 0;
    diagnostics.clear();
    root = new ASTNode(ASTNodeType::PROGRAM, "", 1);
    parseProgram();
    return root;
}
const std::vector<Diagnostic>& Parser::getDiagnostics() const {
    return diagnostics;
}


// Şu anki token'ı döndürür, pozisyon değişmez
Token Parser::current() const {
    if (pos < (int)tokens.size()) {
        return tokens[pos];
    }
    // Dosya sonu — boş bir token döndür
    return Token(TokenType::UNKNOWN, "", -1, -1);
}

// Şu anki token'ı döndürür ve 1 ilerler
Token Parser::advance() {
    Token tok = current();
    if (pos < (int)tokens.size()) {
        pos++;
    }
    return tok;
}

// Beklenen tipteki token'ı döndürür ve ilerler.
// Eşleşmezse hata üretir, yine de ilerler.
Token Parser::expect(TokenType type) {
    Token tok = current();
    if (tok.getType() == type) {
        return advance();
    }
    // Hata: beklenen token yok
    std::string msg = "Satir " + std::to_string(tok.getLine())
        + ": Beklenen token bulunamadi, bulunan: '" + tok.getValue() + "'";
    reportError(msg);
    return tok; // Hatalı token'ı döndür, pozisyon ilerlemez
}

// Eşleşirse true + ilerle, eşleşmezse false (hata yok)
bool Parser::match(TokenType type) {
    if (current().getType() == type) {
        advance();
        return true;
    }
    return false;
}

// Değere göre eşleştir
bool Parser::matchValue(const std::string& val) {
    if (current().getValue() == val) {
        advance();
        return true;
    }
    return false;
}

// Token listesi bitti mi?
bool Parser::isAtEnd() const {
    return pos >= (int)tokens.size();
}
// Hata kurtarma: ; veya } bulana kadar atla
// Bu sayede bir hata sonrası parser tamamen çökmez,
// bir sonraki ifadeden devam eder.
void Parser::synchronize() {
    while (!isAtEnd()) {

        // Noktalı virgül bulduysa, sonrasından devam et
        if (current().getValue() == ";") {
            advance();
            return;
        }

        // Kapanış süslü parantez bulduysa dur (ama tüketme)
        if (current().getValue() == "}") {
            return;
        }
        advance();
    }
}

// Hata mesajını diagnostics listesine ekler
void Parser::reportError(const std::string& message) {
    int line = (pos < (int)tokens.size()) ? current().getLine() : -1;
    int col = (pos < (int)tokens.size()) ? current().getColumn() : -1;
    diagnostics.push_back(
        Diagnostic(line, col, message, DiagnosticSeverity::CRITICAL, "syntax", "", "")
    );
}

// Verilen string C veri tipi mi?
bool Parser::isTypeName(const std::string& val) const {
    return val == "int" || val == "char" || val == "float" || val == "double"
        || val == "void" || val == "long" || val == "short" || val == "unsigned"
        || val == "signed" || val == "struct";
}



// Program: fonksiyon tanımları, global değişkenler, struct'lar
void Parser::parseProgram() {
    while (!isAtEnd()) {

        // Preprocessor direktiflerini atla (#include, #define)
        if (current().getType() == TokenType::PREPROCESSOR) {
            advance();
            continue;
        }
        // struct anahtar kelimesi → struct tanımı
        if (current().getValue() == "struct") {
            ASTNode* structNode = parseStructDef();
            if (structNode) root->addChild(structNode);
            continue;
        }

        // Tip ismi bekliyoruz (int, void, char, ...)
        if (current().getType() == TokenType::KEYWORD && isTypeName(current().getValue())) {
            std::string typeName = advance().getValue();
            // Pointer mı? (int *p gibi)
            bool isPointer = false;
            if (current().getValue() == "*") {
                advance();
                isPointer = true;
            }
            // İsim al
            if (current().getType() != TokenType::IDENTIFIER) {
                reportError("Satir " + std::to_string(current().getLine())
                    + ": Tip isminden sonra tanimlayici bekleniyor.");
                synchronize();
                continue;
            }
            std::string name = current().getValue();
            // Fonksiyon mu değişken mi? İsimden sonra ( varsa fonksiyon
            // Bir sonraki token'a bakmak için pos+1 kontrol et
            if (pos + 1 < (int)tokens.size() && tokens[pos + 1].getValue() == "(") {
                ASTNode* funcNode = parseFunctionDef(typeName);
                if (funcNode) root->addChild(funcNode);
            } else {
                ASTNode* varNode = parseVarDecl(typeName, isPointer);
                if (varNode) root->addChild(varNode);
            }
        } else {
            // Tanınmayan üst düzey yapı
            reportError("Satir " + std::to_string(current().getLine())
                + ": Beklenmeyen token: '" + current().getValue() + "'");
            advance();
        }
    }
}





// typeName zaten okunmuş durumda ("int", "void", vb.)
ASTNode* Parser::parseFunctionDef(const std::string& typeName) {
    // Fonksiyon adını oku
    Token nameToken = expect(TokenType::IDENTIFIER);
    ASTNode* funcNode = new ASTNode(ASTNodeType::FUNCTION_DEF, nameToken.getValue(), nameToken.getLine());
    // Parametre listesi: ( ... )
    expect(TokenType::PUNCTUATION); // (
    // Parametreleri VARIABLE_DECL olarak ekle
    while (!isAtEnd() && current().getValue() != ")") {
        if (current().getType() == TokenType::KEYWORD && isTypeName(current().getValue())) {
            std::string paramType = advance().getValue();
            bool isPtr = false;
            if (current().getValue() == "*") {
                advance();
                isPtr = true;
            }
            if (current().getType() == TokenType::IDENTIFIER) {
                Token paramName = advance();
                ASTNode* paramNode = new ASTNode(ASTNodeType::VARIABLE_DECL, paramName.getValue(), paramName.getLine());
                funcNode->addChild(paramNode);
            }
        }
        // Virgül varsa atla
        if (current().getValue() == ",") {
            advance();
        } else if (current().getValue() != ")") {
            break; // Beklenmeyen token
        }
    }
    expect(TokenType::PUNCTUATION); // )
    // Fonksiyon gövdesi
    ASTNode* body = parseBlock();
    if (body) funcNode->addChild(body);
    return funcNode;
}




// { statement; statement; ... }  →  BLOCK
ASTNode* Parser::parseBlock() {
    int blockLine = current().getLine();
    if (!match(TokenType::PUNCTUATION)) { // {
        if (current().getValue() != "{") {
            reportError("Satir " + std::to_string(current().getLine()) + ": '{' bekleniyor.");
            return nullptr;
        }
        advance();
    }
    ASTNode* block = new ASTNode(ASTNodeType::BLOCK, "", blockLine);
    while (!isAtEnd() && current().getValue() != "}") {
        ASTNode* stmt = parseStatement();
        if (stmt) {
            block->addChild(stmt);
        } else {
            // Hata kurtarma
            synchronize();
        }
    }
    if (!isAtEnd() && current().getValue() == "}") {
        advance(); // } tüket
    } else {
        reportError("Satir " + std::to_string(blockLine) + ": Eslesmeyen '{' — kapatan '}' bulunamadi.");
    }
    return block;
}


ASTNode* Parser::parseStatement() {
    // if
    if (current().getValue() == "if") {
        return parseIfStatement();
    }
    // while
    if (current().getValue() == "while") {
        return parseWhileLoop();
    }
    // for
    if (current().getValue() == "for") {
        return parseForLoop();
    }
    // return
    if (current().getValue() == "return") {
        return parseReturnStmt();
    }
    // struct
    if (current().getValue() == "struct") {
        return parseStructDef();
    }
    // Değişken tanımı: tip isim ...
    if (current().getType() == TokenType::KEYWORD && isTypeName(current().getValue())) {
        std::string typeName = advance().getValue();
        bool isPointer = false;
        if (current().getValue() == "*") {
            advance();
            isPointer = true;
        }
        return parseVarDecl(typeName, isPointer);
    }
    // Atama veya ifade (x = 5; veya func();)
    ASTNode* expr = parseExpression();
    // Noktalı virgül bekliyoruz
    if (current().getValue() == ";") {
        advance();
    }
    return expr;
}
// int x;  veya  int x = 5;  veya  int *ptr = malloc(...);
// typeName zaten okunmuş durumda
ASTNode* Parser::parseVarDecl(const std::string& typeName, bool isPointer) {
    Token nameToken = expect(TokenType::IDENTIFIER);
    ASTNode* varNode = new ASTNode(ASTNodeType::VARIABLE_DECL, nameToken.getValue(), nameToken.getLine());
    // Başlangıç değeri var mı? (= expr)
    if (current().getValue() == "=") {
        advance(); // = tüket
        ASTNode* initExpr = parseExpression();
        if (initExpr) varNode->addChild(initExpr);
    }
    // Noktalı virgül
    if (current().getValue() == ";") {
        advance();
    } else {
        reportError("Satir " + std::to_string(nameToken.getLine())
            + ": Degisken tanimi sonunda ';' bekleniyor.");
    }
    return varNode;
}

// if (koşul) { ... } else { ... }
ASTNode* Parser::parseIfStatement() {
    Token ifToken = advance(); // "if" tüket
    ASTNode* ifNode = new ASTNode(ASTNodeType::IF_STATEMENT, "", ifToken.getLine());
    // ( koşul )
    if (current().getValue() == "(") advance();
    ASTNode* condition = parseExpression();
    if (condition) ifNode->addChild(condition);
    if (current().getValue() == ")") advance();
    // if gövdesi
    ASTNode* thenBlock = parseBlock();
    if (thenBlock) ifNode->addChild(thenBlock);
    // else varsa
    if (current().getValue() == "else") {
        advance(); // "else" tüket
        ASTNode* elseBlock = parseBlock();
        if (elseBlock) ifNode->addChild(elseBlock);
    }
    return ifNode;
}

// while (koşul) { ... }
ASTNode* Parser::parseWhileLoop() {
    Token whileToken = advance(); // "while" tüket
    ASTNode* whileNode = new ASTNode(ASTNodeType::WHILE_LOOP, "", whileToken.getLine());
    // ( koşul )
    if (current().getValue() == "(") advance();
    ASTNode* condition = parseExpression();
    if (condition) whileNode->addChild(condition);
    if (current().getValue() == ")") advance();
    // Gövde
    ASTNode* body = parseBlock();
    if (body) whileNode->addChild(body);
    return whileNode;
}

// for (init; koşul; güncelleme) { ... }
ASTNode* Parser::parseForLoop() {
    Token forToken = advance(); // "for" tüket
    ASTNode* forNode = new ASTNode(ASTNodeType::FOR_LOOP, "", forToken.getLine());
    if (current().getValue() == "(") advance();
    // init kısmı (değişken tanımı veya atama)
    if (current().getValue() != ";") {
        if (current().getType() == TokenType::KEYWORD && isTypeName(current().getValue())) {
            std::string typeName = advance().getValue();
            ASTNode* initVar = parseVarDecl(typeName); // ; içeride tüketilir
            if (initVar) forNode->addChild(initVar);
        } else {
            ASTNode* initExpr = parseExpression();
            if (initExpr) forNode->addChild(initExpr);
            if (current().getValue() == ";") advance();
        }
    } else {
        advance(); // boş init, ; atla
    }
    // Koşul kısmı
    if (current().getValue() != ";") {
        ASTNode* cond = parseExpression();
        if (cond) forNode->addChild(cond);
    }
    if (current().getValue() == ";") advance();
    // Güncelleme kısmı
    if (current().getValue() != ")") {
        ASTNode* update = parseExpression();
        if (update) forNode->addChild(update);
    }
    if (current().getValue() == ")") advance();
    // Gövde
    ASTNode* body = parseBlock();
    if (body) forNode->addChild(body);
    return forNode;
}

// return;  veya  return expr;
ASTNode* Parser::parseReturnStmt() {
    Token retToken = advance(); // "return" tüket
    ASTNode* retNode = new ASTNode(ASTNodeType::RETURN_STMT, "", retToken.getLine());
    // return; (değersiz) mi?
    if (current().getValue() != ";") {
        ASTNode* expr = parseExpression();
        if (expr) retNode->addChild(expr);
    }
    if (current().getValue() == ";") advance();
    return retNode;
}

// struct Point { int x; int y; };
ASTNode* Parser::parseStructDef() {
    Token structToken = advance(); // "struct" tüket
    Token nameToken = expect(TokenType::IDENTIFIER);
    ASTNode* structNode = new ASTNode(ASTNodeType::STRUCT_DEF, nameToken.getValue(), structToken.getLine());
    // { üyeler }
    if (current().getValue() == "{") {
        advance();
        while (!isAtEnd() && current().getValue() != "}") {
            if (current().getType() == TokenType::KEYWORD && isTypeName(current().getValue())) {
                std::string memberType = advance().getValue();
                bool isPtr = (current().getValue() == "*");
                if (isPtr) advance();
                ASTNode* member = parseVarDecl(memberType, isPtr);
                if (member) structNode->addChild(member);
            } else {
                advance(); // Tanınmayan, atla
            }
        }
        if (current().getValue() == "}") advance();
    }
    if (current().getValue() == ";") advance();
    return structNode;
}

// Giriş noktası
ASTNode* Parser::parseExpression() {
    return parseAssignmentExpr();
}
// Atama: x = expr
ASTNode* Parser::parseAssignmentExpr() {
    ASTNode* left = parseComparison();
    if (current().getValue() == "=" && current().getType() == TokenType::OPERATOR) {
        Token op = advance();
        ASTNode* right = parseAssignmentExpr(); // Sağdan sola birleştirme
        ASTNode* assignNode = new ASTNode(ASTNodeType::ASSIGNMENT, left->getValue(), op.getLine());
        // Sol tarafı değil, sağ taraftaki ifadeyi child olarak ekle
        if (right) assignNode->addChild(right);
        delete left; // Sol taraf artık assignment'ın value'sunda
        return assignNode;
    }
    return left;
}
// Karşılaştırma: ==, !=, <, >, <=, >=
ASTNode* Parser::parseComparison() {
    ASTNode* left = parseAddition();
    while (current().getValue() == "==" || current().getValue() == "!="
        || current().getValue() == "<"  || current().getValue() == ">"
        || current().getValue() == "<=" || current().getValue() == ">=") {
        Token op = advance();
        ASTNode* right = parseAddition();
        ASTNode* binOp = new ASTNode(ASTNodeType::BINARY_OP, op.getValue(), op.getLine());
        binOp->addChild(left);
        if (right) binOp->addChild(right);
        left = binOp;
    }
    return left;
}
// Toplama/Çıkarma: +, -
ASTNode* Parser::parseAddition() {
    ASTNode* left = parseMultiplication();
    while (current().getValue() == "+" || current().getValue() == "-") {
        Token op = advance();
        ASTNode* right = parseMultiplication();
        ASTNode* binOp = new ASTNode(ASTNodeType::BINARY_OP, op.getValue(), op.getLine());
        binOp->addChild(left);
        if (right) binOp->addChild(right);
        left = binOp;
    }
    return left;
}
// Çarpma/Bölme: *, /, %
ASTNode* Parser::parseMultiplication() {
    ASTNode* left = parseUnary();
    while (current().getValue() == "*" || current().getValue() == "/"
        || current().getValue() == "%") {
        Token op = advance();
        ASTNode* right = parseUnary();
        ASTNode* binOp = new ASTNode(ASTNodeType::BINARY_OP, op.getValue(), op.getLine());
        binOp->addChild(left);
        if (right) binOp->addChild(right);
        left = binOp;
    }
    return left;
}
// Tekli operatörler: !, -, *, & (dereference, address-of)
ASTNode* Parser::parseUnary() {
    if (current().getValue() == "!" || current().getValue() == "-"
        || current().getValue() == "*" || current().getValue() == "&"
        || current().getValue() == "++") {
        Token op = advance();
        ASTNode* operand = parseUnary(); // Özyinelemeli (!!x gibi)
        ASTNode* unaryNode = new ASTNode(ASTNodeType::EXPRESSION, op.getValue(), op.getLine());
        if (operand) unaryNode->addChild(operand);
        return unaryNode;
    }
    return parsePrimary();
}
// En temel birimler: sayı, string, değişken, fonksiyon çağrısı, parantezli ifade
ASTNode* Parser::parsePrimary() {
    Token tok = current();
    // Sayısal literal: 42, 3.14
    if (tok.getType() == TokenType::INT_LITERAL || tok.getType() == TokenType::FLOAT_LITERAL) {
        advance();
        return new ASTNode(ASTNodeType::EXPRESSION, tok.getValue(), tok.getLine());
    }
    // String literal: "hello"
    if (tok.getType() == TokenType::STRING_LITERAL) {
        advance();
        return new ASTNode(ASTNodeType::EXPRESSION, tok.getValue(), tok.getLine());
    }
    // Char literal: 'a'
    if (tok.getType() == TokenType::CHAR_LITERAL) {
        advance();
        return new ASTNode(ASTNodeType::EXPRESSION, tok.getValue(), tok.getLine());
    }
    // NULL anahtar kelimesi
    if (tok.getValue() == "NULL" || tok.getValue() == "nullptr") {
        advance();
        return new ASTNode(ASTNodeType::EXPRESSION, "NULL", tok.getLine());
    }
    // sizeof(...)
    if (tok.getValue() == "sizeof") {
        advance();
        ASTNode* sizeofNode = new ASTNode(ASTNodeType::FUNCTION_CALL, "sizeof", tok.getLine());
        if (current().getValue() == "(") {
            advance();
            ASTNode* arg = parseExpression();
            if (arg) sizeofNode->addChild(arg);
            if (current().getValue() == ")") advance();
        }
        return sizeofNode;
    }
    // Identifier → değişken referansı veya fonksiyon çağrısı
    if (tok.getType() == TokenType::IDENTIFIER) {
        advance();
        // Fonksiyon çağrısı mı? isim( ... )
        if (current().getValue() == "(") {
            advance(); // ( tüket
            ASTNode* callNode = new ASTNode(ASTNodeType::FUNCTION_CALL, tok.getValue(), tok.getLine());
            // Argümanlar
            while (!isAtEnd() && current().getValue() != ")") {
                ASTNode* arg = parseExpression();
                if (arg) callNode->addChild(arg);
                if (current().getValue() == ",") advance();
            }
            if (current().getValue() == ")") advance();
            return callNode;
        }
        // Dizi erişimi? isim[index]
        if (current().getValue() == "[") {
            advance();
            ASTNode* indexExpr = parseExpression();
            if (current().getValue() == "]") advance();
            ASTNode* accessNode = new ASTNode(ASTNodeType::EXPRESSION, tok.getValue(), tok.getLine());
            if (indexExpr) accessNode->addChild(indexExpr);
            return accessNode;
        }
        // ++ veya -- (postfix)
        if (current().getValue() == "++" || current().getValue() == "--") {
            Token postOp = advance();
            return new ASTNode(ASTNodeType::EXPRESSION, tok.getValue() + postOp.getValue(), tok.getLine());
        }
        // Basit değişken referansı
        return new ASTNode(ASTNodeType::EXPRESSION, tok.getValue(), tok.getLine());
    }
    // Parantezli ifade: (expr)
    if (tok.getValue() == "(") {
        advance(); // ( tüket
        ASTNode* inner = parseExpression();
        if (current().getValue() == ")") advance();
        return inner;
    }
    // Tanınmayan token
    reportError("Satir " + std::to_string(tok.getLine())
        + ": Beklenmeyen token: '" + tok.getValue() + "'");
    advance();
    return nullptr;
}