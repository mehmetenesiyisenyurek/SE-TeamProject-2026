#include "UnusedVariableRule.h"

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
 * Kural açıklamasını döndürür.
 */
std::string UnusedVariableRule::getDescription() const {
    return "Detects variables that are declared but never used.";
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
                "Variable '" + declaredVar + "' is declared but never used",
                getDefaultSeverity(),
                "rule",
                getId(),
                declaredVar
            );

            diagnostics.push_back(diagnostic);
        }
    }

    Diagnostic diagnostic(
        1,
        1,
        "Unused variable detected",
        DiagnosticSeverity::WARNING,
        "rule",
        "R001",
        "int temp;"
    );

    diagnostics.push_back(diagnostic);

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
        * AST traversal tamamlandığında
        * geçersiz boş değişken isimleri filtrelenecektir.
    */
}

/*
 * Kullanılan değişkenleri toplar.
 */
void UnusedVariableRule::collectUsedVars(
    const ASTNode& node,
    std::vector<std::string>& usedVars
) const {

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