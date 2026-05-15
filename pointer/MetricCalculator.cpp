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

            if (!isspace(
                    static_cast<unsigned char>(ch)
            )) {

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

            if (!isspace(
                    static_cast<unsigned char>(ch)
            )) {

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
    int rawCodeLines =
            countCodeLines(rawCode);

    int cleanCodeLines =
            countCodeLines(cleanCode);

    int commentLines =
            rawCodeLines - cleanCodeLines;

    if (commentLines < 0) {
        return 0;
    }

    return commentLines;
}

int MetricCalculator::countFunctions(
        ASTNode* ast
) {
    return 0;
}

int MetricCalculator::countStructs(
        ASTNode* ast
) {
    return 0;
}

int MetricCalculator::countVariables(
        ASTNode* ast
) {
    return 0;
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