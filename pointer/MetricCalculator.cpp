#include "MetricCalculator.h"

#include <sstream>
#include <cctype>

using namespace std;

vector<CodeMetric> MetricCalculator::calculate(
        ASTNode* ast,
        const string& rawCode,
        const string& cleanCode
) {
    return {};
}

int MetricCalculator::countTotalLines(
        const string& code
) {
    if (code.empty()) {
        return 0;
    }

    int count = 1;

    for (char ch : code) {
        if (ch == '\n') {
            count++;
        }
    }

    return count;
}

int MetricCalculator::countCodeLines(
        const string& code
) {
    stringstream ss(code);
    string line;
    int count = 0;

    while (getline(ss, line)) {
        bool hasContent = false;

        for (char ch : line) {
            if (!isspace(static_cast<unsigned char>(ch))) {
                hasContent = true;
                break;
            }
        }

        if (hasContent) {
            count++;
        }
    }

    return count;
}

int MetricCalculator::countEmptyLines(
        const string& code
) {
    stringstream ss(code);
    string line;
    int count = 0;

    while (getline(ss, line)) {
        bool isEmpty = true;

        for (char ch : line) {
            if (!isspace(static_cast<unsigned char>(ch))) {
                isEmpty = false;
                break;
            }
        }

        if (isEmpty) {
            count++;
        }
    }

    return count;
}

int MetricCalculator::countCommentLines(
        const string& rawCode,
        const string& cleanCode
) {
    int rawCodeLines = countCodeLines(rawCode);
    int cleanCodeLines = countCodeLines(cleanCode);

    int commentLines = rawCodeLines - cleanCodeLines;

    if (commentLines < 0) {
        return 0;
    }

    return commentLines;
}

int MetricCalculator::countFunctions(
        ASTNode* ast
) {
    if (ast == nullptr) {
        return 0;
    }

    int count = 0;

    // FUNCTION_DEF dugumu bulunduysa say
    if (ast->getType() == ASTNodeType::FUNCTION_DEF) {
        count++;
    }

    // Alt dugumleri recursive gez
    for (ASTNode* child : ast->getChildren()) {
        count += countFunctions(child);
    }

    return count;
}

int MetricCalculator::countStructs(
        ASTNode* ast
) {
    if (ast == nullptr) {
        return 0;
    }

    int count = 0;

    // STRUCT_DEF dugumu bulunduysa say
    if (ast->getType() == ASTNodeType::STRUCT_DEF) {
        count++;
    }

    // Alt dugumleri recursive gez
    for (ASTNode* child : ast->getChildren()) {
        count += countStructs(child);
    }

    return count;
}

int MetricCalculator::countVariables(
        ASTNode* ast
) {
    if (ast == nullptr) {
        return 0;
    }

    int count = 0;

    // VARIABLE_DECL dugumu bulunduysa say
    if (ast->getType() == ASTNodeType::VARIABLE_DECL) {
        count++;
    }

    // Alt dugumleri recursive gez
    for (ASTNode* child : ast->getChildren()) {
        count += countVariables(child);
    }

    return count;
}

double MetricCalculator::averageFunctionLength(
        ASTNode* ast
) {
    return 0.0;
}

void MetricCalculator::collectFunctionLengths(
        ASTNode* node,
        vector<int>& lengths
) {

}

int MetricCalculator::findMaxLine(
        ASTNode* node
) {
    return 0;
}