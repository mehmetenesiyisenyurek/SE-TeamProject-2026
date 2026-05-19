#pragma once

#include "ASTNode.h"
#include "../lexer/Token.h"
#include <vector>
#include <memory>

// Sözdizimi analiz arayüzü
class Parser {
public:
    virtual ~Parser() = default;
    virtual std::shared_ptr<ASTNode> parse(const std::vector<Token>& tokens) = 0;
};
