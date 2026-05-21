#include "Token.h"

Token::Token(
    TokenType type,
    const std::string& value,
    int line,
    int column
)
    : type(type),
      value(value),
      line(line),
      column(column)
{
}

TokenType Token::getType() const
{
    return type;
}

std::string Token::getValue() const
{
    return value;
}

int Token::getLine() const
{
    return line;
}

int Token::getColumn() const
{
    return column;
}