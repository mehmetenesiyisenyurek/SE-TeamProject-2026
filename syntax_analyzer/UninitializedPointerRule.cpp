#include "UninitializedPointerRule.h"

//#include "../parser/ASTNode.h"
//#include "../parser/ASTNodeType.h"

/*
 * Kural ID'sini döndürür.
 */
std::string UninitializedPointerRule::getId() const {
    return "R012";
}

/*
 * Kural adını döndürür.
 */
std::string UninitializedPointerRule::getName() const {
    return "Uninitialized Pointer Rule";
}

/*
 * Kural açıklamasını döndürür.
 */
std::string UninitializedPointerRule::getDescription() const {
    return "Detects uninitialized pointer usage.";
}

/*
 * Varsayılan severity seviyesini döndürür.
 */
DiagnosticSeverity UninitializedPointerRule::getDefaultSeverity() const {
    return DiagnosticSeverity::CRITICAL;
}

/*
 * Başlatılmamış pointer kullanımını analiz eder.
 * Şimdilik örnek diagnostic üretir.
 */
std::vector<Diagnostic> UninitializedPointerRule::check(const ASTNode& ast) {

    std::vector<Diagnostic> diagnostics;

    std::vector<std::string> pointers;

    std::vector<std::string> dereferencedPointers;

    collectUninitializedPointers(ast, pointers);

    collectDereferencedPointers(ast, dereferencedPointers);

    for (const std::string& pointer : dereferencedPointers) {

        if (isPointerInitialized(pointer, pointers)) {

            Diagnostic diagnostic(
                1,
                1,
                "Pointer '" + pointer +
                "' may be dereferenced before initialization",
                getDefaultSeverity(),
                "rule",
                getId(),
                pointer
            );

            diagnostics.push_back(diagnostic);
        }
    }

    return diagnostics;
}

/*
 * Başlatılmamış pointerları toplar.
 */
void UninitializedPointerRule::collectUninitializedPointers(
    const ASTNode& node,
    std::vector<std::string>& pointers
) const {

    /*
    if (node.getType() == ASTNodeType::VARIABLE_DECL) {

        if (node.isPointerDeclaration() && !node.hasInitializer()) {

            std::string pointerName = node.getValue();

            if (!pointerName.empty()) {
                pointers.push_back(pointerName);
            }
        }
    }

    for (const ASTNode& child : node.getChildren()) {
        collectUninitializedPointers(child, pointers);
    }
    */
}

/*
 * Pointer dereference kullanımlarını toplar.
 */
void UninitializedPointerRule::collectDereferencedPointers(
    const ASTNode& node,
    std::vector<std::string>& dereferencedPointers
) const {

    /*
    if (node.getType() == ASTNodeType::EXPRESSION) {

        if (node.isPointerDereference()) {

            std::string pointerName = node.getValue();

            if (!pointerName.empty()) {
                dereferencedPointers.push_back(pointerName);
            }
        }
    }

    for (const ASTNode& child : node.getChildren()) {
        collectDereferencedPointers(child, dereferencedPointers);
    }
    */
}

/*
 * Pointer'ın başlatılıp başlatılmadığını kontrol eder.
 */
bool UninitializedPointerRule::isPointerInitialized(
    const std::string& pointerName,
    const std::vector<std::string>& initializedPointers
) const {

    for (const std::string& pointer : initializedPointers) {

        if (pointer == pointerName) {
            return true;
        }
    }

    return false;
}