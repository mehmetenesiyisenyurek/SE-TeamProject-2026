#include "Lexer.h"

#include <cctype>

const std::unordered_set<std::string> Lexer::keywords =
{
    "int",
    "float",
    "double",
    "char",
    "void",
    "if",
    "else",
    "while",
    "for",
    "return",
    "struct",
    "switch",
    "case",
    "break",
    "continue",
    "typedef",
    "enum",
    "union",
    "sizeof",
    "static",
    "extern",
    "const",
    "goto",
    "default",
    "unsigned",
    "signed",
    "long",
    "short",
    "do"
};

Lexer::Lexer()
    : source(""),
      position(0),
      line(1),
      column(1)
{
}

std::vector<Token> Lexer::tokenize(const std::string& code)
{
    source = code;

    position = 0;
    line = 1;
    column = 1;

    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        skipWhitespace();

        if (isAtEnd())
        {
            break;
        }

        char current = peek();

        if (std::isalpha(current) || current == '_')
        {
            tokens.push_back(readKeywordOrIdentifier());
        }
        else if (std::isdigit(current))
        {
            tokens.push_back(readNumericLiteral());
        }
        else if (current == '"')
        {
            tokens.push_back(readStringLiteral());
        }
        else if (current == '\'')
        {
            tokens.push_back(readCharLiteral());
        }
        else if (isOperatorStart(current))
        {
            tokens.push_back(readOperator());
        }
        else if (isPunctuation(current))
        {
            int tokenLine = line;
            int tokenColumn = column;

            std::string value(1, advance());

            tokens.push_back(
                Token(
                    TokenType::PUNCTUATION,
                    value,
                    tokenLine,
                    tokenColumn
                )
            );
        }
        else if (current == '#')
        {
            int tokenLine = line;
            int tokenColumn = column;

            std::string value;

            while (!isAtEnd() && peek() != '\n')
            {
                value += advance();
            }

            tokens.push_back(
                Token(
                    TokenType::PREPROCESSOR,
                    value,
                    tokenLine,
                    tokenColumn
                )
            );
        }
        else
        {
            int tokenLine = line;
            int tokenColumn = column;

            std::string value(1, advance());

            tokens.push_back(
                Token(
                    TokenType::UNKNOWN,
                    value,
                    tokenLine,
                    tokenColumn
                )
            );
        }
    }

    return tokens;
}

char Lexer::advance()
{
    char current = source[position];

    position++;

    if (current == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    return current;
}

char Lexer::peek(int offset) const
{
    int index = position + offset;

    if (index >= static_cast<int>(source.length()))
    {
        return '\0';
    }

    return source[index];
}

bool Lexer::isAtEnd() const
{
    return position >= static_cast<int>(source.length());
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char current = peek();

        if (
            current == ' '  ||
            current == '\t' ||
            current == '\r' ||
            current == '\n'
        )
        {
            advance();
        }
        else
        {
            break;
        }
    }
}

Token Lexer::readKeywordOrIdentifier()
{
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;

    while (
        !isAtEnd() &&
        (std::isalnum(peek()) || peek() == '_')
    )
    {
        value += advance();
    }

    TokenType type =
        isKeyword(value)
        ? TokenType::KEYWORD
        : TokenType::IDENTIFIER;

    return Token(type, value, tokenLine, tokenColumn);
}

Token Lexer::readNumericLiteral()
{
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;

    bool isFloat = false;

    if (peek() == '0' &&
        (peek(1) == 'x' || peek(1) == 'X'))
    {
        value += advance();
        value += advance();

        while (std::isxdigit(peek()))
        {
            value += advance();
        }

        return Token(
            TokenType::INT_LITERAL,
            value,
            tokenLine,
            tokenColumn
        );
    }

    while (!isAtEnd())
    {
        char current = peek();

        if (std::isdigit(current))
        {
            value += advance();
        }
        else if (current == '.')
        {
            isFloat = true;
            value += advance();
        }
        else
        {
            break;
        }
    }

    return Token(
        isFloat
            ? TokenType::FLOAT_LITERAL
            : TokenType::INT_LITERAL,
        value,
        tokenLine,
        tokenColumn
    );
}

Token Lexer::readStringLiteral()
{
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;

    value += advance();

    while (!isAtEnd())
    {
        char current = advance();

        value += current;

        if (current == '\\')
        {
            if (!isAtEnd())
            {
                value += advance();
            }
        }
        else if (current == '"')
        {
            break;
        }
    }

    return Token(
        TokenType::STRING_LITERAL,
        value,
        tokenLine,
        tokenColumn
    );
}

Token Lexer::readCharLiteral()
{
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;

    value += advance();

    while (!isAtEnd())
    {
        char current = advance();

        value += current;

        if (current == '\\')
        {
            if (!isAtEnd())
            {
                value += advance();
            }
        }
        else if (current == '\'')
        {
            break;
        }
    }

    return Token(
        TokenType::CHAR_LITERAL,
        value,
        tokenLine,
        tokenColumn
    );
}

Token Lexer::readOperator()
{
    int tokenLine = line;
    int tokenColumn = column;

    std::string value;

    char current = advance();

    value += current;

    char next = peek();

    if (
        (current == '=' && next == '=') ||
        (current == '!' && next == '=') ||
        (current == '<' && next == '=') ||
        (current == '>' && next == '=') ||
        (current == '+' && next == '+') ||
        (current == '-' && next == '-') ||
        (current == '&' && next == '&') ||
        (current == '|' && next == '|') ||
        (current == '+' && next == '=') ||
        (current == '-' && next == '=') ||
        (current == '*' && next == '=') ||
        (current == '/' && next == '=') ||
        (current == '-' && next == '>')
    )
    {
        value += advance();
    }

    return Token(
        TokenType::OPERATOR,
        value,
        tokenLine,
        tokenColumn
    );
}

bool Lexer::isKeyword(const std::string& word) const
{
    return keywords.find(word) != keywords.end();
}

bool Lexer::isOperatorStart(char c) const
{
    return
        c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '=' ||
        c == '!' ||
        c == '<' ||
        c == '>' ||
        c == '&' ||
        c == '|' ||
        c == '^' ||
        c == '~';
}

bool Lexer::isPunctuation(char c) const
{
    return
        c == ';' ||
        c == ',' ||
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == '[' ||
        c == ']';
}