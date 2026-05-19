#pragma once

#include "../parser/ASTNode.h"
#include "../infrastructure/AnalysisResult.h"
#include <memory>

// Kural motoru arayüzü
class RuleEngine {
public:
    virtual ~RuleEngine() = default;
    virtual void applyRules(std::shared_ptr<ASTNode> rootNode, AnalysisResult& result) = 0;
};
