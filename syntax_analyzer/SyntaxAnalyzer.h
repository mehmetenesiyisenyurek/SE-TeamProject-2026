#pragma once

#include "RuleEngine.h"

// Sözdizimi kontrol arayüzü
class SyntaxAnalyzer {
public:
    virtual ~SyntaxAnalyzer() = default;
    virtual void analyze(std::shared_ptr<ASTNode> rootNode, AnalysisResult& result) = 0;
};
