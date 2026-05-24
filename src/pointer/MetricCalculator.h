#ifndef METRIC_CALCULATOR_H
#define METRIC_CALCULATOR_H

#include <string>
#include <vector>

#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/CodeMetric.h"

class MetricCalculator {
public:
    std::vector<CodeMetric> calculate(
            ASTNode* ast,
            const std::string& rawCode,
            const std::string& cleanCode
    );

private:
    int countTotalLines(const std::string& code);
    int countCodeLines(const std::string& code);
    int countEmptyLines(const std::string& code);
    int countCommentLines(
            const std::string& rawCode,
            const std::string& cleanCode
    );

    int countFunctions(ASTNode* ast);
    int countStructs(ASTNode* ast);
    int countVariables(ASTNode* ast);

    double averageFunctionLength(ASTNode* ast);

    void collectFunctionLengths(
            ASTNode* node,
            std::vector<int>& lengths
    );

    int findMaxLine(ASTNode* node);
};

#endif