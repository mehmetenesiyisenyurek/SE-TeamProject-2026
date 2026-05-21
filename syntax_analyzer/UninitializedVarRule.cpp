#include "UninitializedVarRule.h"

//#include "../parser/ASTNode.h"
//#include "../parser/ASTNodeType.h"

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
 * Başlatılmadan kullanılan değişkenleri analiz eder.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UninitializedVarRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    std::vector<std::string> uninitializedVars;

    std::vector<std::string> usedVars;

    collectUninitializedVars(ast, uninitializedVars);

    collectUsedVars(ast, usedVars);

    for (const std::string& usedVar : usedVars) {

        if (isVariableInitialized(usedVar, uninitializedVars)) {

            Diagnostic diagnostic(
                1,
                1,
                "Değişken '" + usedVar + "' başlatılmadan kullanılıyor.",
                getDefaultSeverity(),
                "rule",
                getId(),
                usedVar
            );

            diagnostics.push_back(diagnostic);
        }
    }

    return diagnostics;
}

/*
 * Başlatılmamış değişkenleri toplar.
 */
void UninitializedVarRule::collectUninitializedVars(
    const ASTNode& node,
    std::vector<std::string>& uninitializedVars
) const {


    if (node.getType() == ASTNodeType::VARIABLE_DECL) {

        std::string variableName = node.getValue();

        if (!variableName.empty()) {
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


    if (
        node.getType() == ASTNodeType::EXPRESSION ||
        node.getType() == ASTNodeType::ASSIGNMENT ||
        node.getType() == ASTNodeType::BINARY_OP ||
        node.getType() == ASTNodeType::FUNCTION_CALL
    ) {

        std::string variableName = node.getValue();

        if (!variableName.empty()) {
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
    const std::vector<std::string>& initializedVars
) const {

    for (const std::string& initializedVar : initializedVars) {

        if (initializedVar == variableName) {
            return true;
        }
    }

    return false;
}