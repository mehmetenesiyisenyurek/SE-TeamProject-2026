#include "UninitializedVarRule.h"

#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"

/*
 * Geçerli değişken ismi olup olmadığını kontrol eder.
 */
static bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    if (name == "NULL" || name == "nullptr") {
        return false;
    }

    return true;
}

/*
 * Kural ID'sini döndürür.
 */
std::string UninitializedVarRule::getId() const {
    return "R002";
}

/*
 * Kural adını döndürür.
 */
std::string UninitializedVarRule::getName() const {
    return "Uninitialized Variable Rule";
}

/*
 * Varsayılan severity seviyesini döndürür.
 */
DiagnosticSeverity UninitializedVarRule::getDefaultSeverity() const {
    return DiagnosticSeverity::WARNING;
}

/*
 * AST üzerinde başlatılmamış değişken analizi yapar.
 */
std::vector<Diagnostic> UninitializedVarRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;
    std::vector<std::string> uninitializedVars;
    std::vector<std::string> usedVars;

    collectUninitializedVars(ast, uninitializedVars);
    collectUsedVars(ast, usedVars);

    for (const std::string& usedVar : usedVars) {
        if (isVariableInitialized(usedVar, uninitializedVars)) {
            diagnostics.emplace_back(
                1,
                1,
                "Degisken '" + usedVar + "' baslatilmadan kullaniliyor.",
                getDefaultSeverity(),
                "rule",
                getId(),
                usedVar
            );
        }
    }

    return diagnostics;
}

/*
 * Başlatılmadan tanımlanan değişkenleri toplar.
 */
void UninitializedVarRule::collectUninitializedVars(
    const ASTNode& node,
    std::vector<std::string>& uninitializedVars
) const {
    if (node.getType() == ASTNodeType::VARIABLE_DECL && node.getChildren().empty()) {
        std::string variableName = node.getValue();

        if (isValidName(variableName)) {
            uninitializedVars.push_back(variableName);
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            collectUninitializedVars(*child, uninitializedVars);
        }
    }
}

/*
 * Kullanılan değişkenleri toplar.
 */
void UninitializedVarRule::collectUsedVars(
    const ASTNode& node,
    std::vector<std::string>& usedVars
) const {
    if (node.getType() == ASTNodeType::EXPRESSION ||
        node.getType() == ASTNodeType::ASSIGNMENT ||
        node.getType() == ASTNodeType::BINARY_OP ||
        node.getType() == ASTNodeType::FUNCTION_CALL ||
        node.getType() == ASTNodeType::RETURN_STMT) {

        std::string variableName = node.getValue();

        if (isValidName(variableName)) {
            usedVars.push_back(variableName);
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            collectUsedVars(*child, usedVars);
        }
    }
}

/*
 * Değişkenin başlatılmamış değişkenler listesinde olup olmadığını kontrol eder.
 */
bool UninitializedVarRule::isVariableInitialized(
    const std::string& variableName,
    const std::vector<std::string>& uninitializedVars
) const {
    for (const std::string& uninitializedVar : uninitializedVars) {
        if (uninitializedVar == variableName) {
            return true;
        }
    }

    return false;
}