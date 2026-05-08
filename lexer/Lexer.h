#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <unordered_set>

#include "Token.h"

class Lexer
{
private:
    std::string source;

    int position;
    int line;
    int column;

    static const std::unordered_set<std::string> keywords;

    char advance();

    char peek(int offset = 0) const;

    bool isAtEnd() const;

    void skipWhitespace();

    Token readKeywordOrIdentifier();

    Token readNumericLiteral();

    Token readStringLiteral();

    Token readCharLiteral();

    Token readOperator();

    bool isKeyword(const std::string& word) const;

    bool isOperatorStart(char c) const;

    bool isPunctuation(char c) const;

public:
    Lexer();

    std::vector<Token> tokenize(const std::string& code);
};

#endif