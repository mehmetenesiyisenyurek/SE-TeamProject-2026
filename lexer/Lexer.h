#pragma once

#include "Token.h"
#include <vector>
#include <string>

// Sözcüksel analiz arayüzü
class Lexer {
public:
    virtual ~Lexer() = default;
    virtual std::vector<Token> tokenize(const std::string& sourceCode) = 0;
};
