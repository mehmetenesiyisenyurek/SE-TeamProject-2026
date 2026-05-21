#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token
{
private:
    TokenType type;
    std::string value;
    int line;
    int column;

public:
    Token(
        TokenType type,
        const std::string& value,
        int line,
        int column
    );

    TokenType getType() const;

    std::string getValue() const;

    int getLine() const;

    int getColumn() const;
};

#endif