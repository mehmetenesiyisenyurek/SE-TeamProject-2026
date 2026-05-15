#include "MetricCalculator.h"

#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

vector<CodeMetric> MetricCalculator::calculate(
        ASTNode* ast,
        const string& rawCode,
        const string& cleanCode
) {
    vector<CodeMetric> metrics;

    // Toplam satir sayisi
    metrics.emplace_back(
            "Toplam Satir Sayisi (LOC)",
            countTotalLines(rawCode),
            "satir",
            "Orijinal kaynak dosyadaki toplam satir sayisi"
    );

    // Kod satiri
    metrics.emplace_back(
            "Kod Satiri",
            countCodeLines(cleanCode),
            "satir",
            "Yorumlar temizlendikten sonraki bos olmayan kod satiri sayisi"
    );

    // Bos satir
    metrics.emplace_back(
            "Bos Satir",
            countEmptyLines(rawCode),
            "satir",
            "Tamamen bos olan satir sayisi"
    );

    // Yorum satiri
    metrics.emplace_back(
            "Yorum Satiri",
            countCommentLines(
                    rawCode,
                    cleanCode
            ),
            "satir",
            "Yorum satiri sayisi"
    );

    // Fonksiyon sayisi
    metrics.emplace_back(
            "Fonksiyon Sayisi",
            countFunctions(ast),
            "adet",
            "AST uzerindeki FUNCTION_DEF dugumu sayisi"
    );

    // Struct sayisi
    metrics.emplace_back(
            "Struct Sayisi",
            countStructs(ast),
            "adet",
            "AST uzerindeki STRUCT_DEF dugumu sayisi"
    );

    // Degisken sayisi
    metrics.emplace_back(
            "Degisken Sayisi",
            countVariables(ast),
            "adet",
            "AST uzerindeki VARIABLE_DECL dugumu sayisi"
    );

    // Ortalama fonksiyon uzunlugu
    metrics.emplace_back(
            "Ort. Fonksiyon Uzunlugu",
            averageFunctionLength(ast),
            "satir",
            "Fonksiyonlarin ortalama satir uzunlugu"
    );

    return metrics;
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
    if (ast == nullptr) {
        return 0;
    }

    int count = 0;

    if (ast->getType() ==
        ASTNodeType::FUNCTION_DEF) {

        count++;
    }

    for (ASTNode* child :
         ast->getChildren()) {

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

    if (ast->getType() ==
        ASTNodeType::STRUCT_DEF) {

        count++;
    }

    for (ASTNode* child :
         ast->getChildren()) {

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

    if (ast->getType() ==
        ASTNodeType::VARIABLE_DECL) {

        count++;
    }

    for (ASTNode* child :
         ast->getChildren()) {

        count += countVariables(child);
    }

    return count;
}

double MetricCalculator::averageFunctionLength(
        ASTNode* ast
) {
    if (ast == nullptr) {
        return 0.0;
    }

    vector<int> lengths;

    collectFunctionLengths(
            ast,
            lengths
    );

    if (lengths.empty()) {
        return 0.0;
    }

    int total = 0;

    for (int length : lengths) {
        total += length;
    }

    return static_cast<double>(total)
           / lengths.size();
}

void MetricCalculator::collectFunctionLengths(
        ASTNode* node,
        vector<int>& lengths
) {
    if (node == nullptr) {
        return;
    }

    // FUNCTION_DEF bulunduysa uzunluk hesapla
    if (node->getType() ==
        ASTNodeType::FUNCTION_DEF) {

        int startLine =
                node->getLine();

        int endLine =
                findMaxLine(node);

        // Fonksiyon satir uzunlugu
        if (startLine > 0 &&
            endLine >= startLine) {

            lengths.push_back(
                    endLine - startLine + 1
            );
        }
    }

    // Alt dugumleri recursive gez
    for (ASTNode* child :
         node->getChildren()) {

        collectFunctionLengths(
                child,
                lengths
        );
    }
}

int MetricCalculator::findMaxLine(
        ASTNode* node
) {
    if (node == nullptr) {
        return 0;
    }

    int maxLine =
            node->getLine();

    // Alt dugumlerde en buyuk satiri bul
    for (ASTNode* child :
         node->getChildren()) {

        maxLine = max(
                maxLine,
                findMaxLine(child)
        );
    }

    return maxLine;
}