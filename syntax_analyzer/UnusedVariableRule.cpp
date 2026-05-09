#include "UnusedVariableRule.h"

//#include "../parser/ASTNode.h"
//#include "../parser/ASTNodeType.h"

/*
 * Geçerli değişken ismi olup olmadığını kontrol eder.
 */
static bool isValidVariableName(const std::string& variableName) {

    return !variableName.empty();
}

/*
 * Kural ID'sini döndürür.
 */
std::string UnusedVariableRule::getId() const {
    return "R001";
}

/*
 * Kural adını döndürür.
 */
std::string UnusedVariableRule::getName() const {
    return "Unused Variable Rule";
}

/*
 * Varsayılan severity seviyesini döndürür.
 */
DiagnosticSeverity UnusedVariableRule::getDefaultSeverity() const {
    return DiagnosticSeverity::WARNING;
}

/*
 * Kullanılmayan değişken analizini yapar.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UnusedVariableRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    std::vector<std::string> declaredVars;

    std::vector<std::string> usedVars;

    collectDeclaredVars(ast, declaredVars);

    collectUsedVars(ast, usedVars);

    for (const std::string& declaredVar : declaredVars) {

        if (!isValidVariableName(declaredVar)) {
            continue;
        }

        if (!isVariableUsed(declaredVar, usedVars)) {

            Diagnostic diagnostic(
                1,
                1,
                "Değişken '" + declaredVar + "' tanımlanmış ancak hiç kullanılmamış.",
                getDefaultSeverity(),
                "rule",
                getId(),
                declaredVar
            );

            diagnostics.push_back(diagnostic);
        }
    }

    return diagnostics;
}

/*
 * Tanımlanan değişkenleri toplar.
 */
void UnusedVariableRule::collectDeclaredVars(
    const ASTNode& node,
    std::vector<std::string>& declaredVars
) const {

    /*
    if (node.getType() == ASTNodeType::VARIABLE_DECL) {

        std::string variableName = node.getValue();

        if (!variableName.empty()) {
            declaredVars.push_back(variableName);
        }
    }

    for (const ASTNode* child : node.getChildren()) {
    if (child != nullptr) {
        collectDeclaredVars(*child, declaredVars);
    }
}
    */
}

/*
 * Kullanılan değişkenleri toplar.
 */
void UnusedVariableRule::collectUsedVars(
    const ASTNode& node,
    std::vector<std::string>& usedVars
) const {

    /*
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
    */
}

/*
 * Değişkenin kullanılan değişkenler listesinde olup olmadığını kontrol eder.
 */
bool UnusedVariableRule::isVariableUsed(
    const std::string& variableName,
    const std::vector<std::string>& usedVars
) const {

    for (const std::string& usedVar : usedVars) {

        if (usedVar == variableName) {
            return true;
        }
    }

    return false;
}