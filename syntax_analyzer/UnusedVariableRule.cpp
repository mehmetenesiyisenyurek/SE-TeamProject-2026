#include "UnusedVariableRule.h"

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
 * AST üzerinde kullanılmayan değişken analizi yapar.
 */
std::vector<Diagnostic> UnusedVariableRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;
    std::vector<std::string> declaredVars;
    std::vector<std::string> usedVars;

    collectDeclaredVars(ast, declaredVars);
    collectUsedVars(ast, usedVars);

    for (const std::string& declaredVar : declaredVars) {
        if (!isVariableUsed(declaredVar, usedVars)) {
            diagnostics.emplace_back(
                1,
                1,
                "Degisken '" + declaredVar + "' tanimlanmis fakat kullanilmamis.",
                getDefaultSeverity(),
                "rule",
                getId(),
                declaredVar
            );
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
    if (node.getType() == ASTNodeType::VARIABLE_DECL) {
        std::string variableName = node.getValue();

        if (isValidName(variableName)) {
            declaredVars.push_back(variableName);
        }
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr) {
            collectDeclaredVars(*child, declaredVars);
        }
    }
}

/*
 * Kullanılan değişkenleri toplar.
 */
void UnusedVariableRule::collectUsedVars(
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
 * Değişkenin kullanılıp kullanılmadığını kontrol eder.
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