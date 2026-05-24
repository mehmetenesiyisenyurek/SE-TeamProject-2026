#include "NullDereferenceRule.h"

// DEĞİŞTİ: IRule arayüzü için kural ID'si eklendi.
std::string NullDereferenceRule::getId() const {
    return "R007";
}

// DEĞİŞTİ: IRule arayüzü için kural adı eklendi.
std::string NullDereferenceRule::getName() const {
    return "Null Dereference Rule";
}

// DEĞİŞTİ: DiagnosticSeverity enum'u kullanılarak varsayılan seviye eklendi.
DiagnosticSeverity NullDereferenceRule::getDefaultSeverity() const {
    return DiagnosticSeverity::WARNING;
}

// DEĞİŞTİ: RuleEngine'in beklediği check(const ASTNode& ast) yapısına uyarlandı.
std::vector<Diagnostic> NullDereferenceRule::check(const ASTNode& ast) {
    std::vector<Diagnostic> diagnostics;

    if (ast.getType() == ASTNodeType::FUNCTION_DEF) {
        auto mallocAssignments = findMallocAssignments(ast);

        for (const auto& assignment : mallocAssignments) {
            const std::string& variableName = assignment.first;
            int lineNumber = assignment.second;

            if (!variableName.empty() && !hasNullCheck(variableName, ast)) {

                // DEĞİŞTİ: Diagnostic nesnesi projenin ortak Diagnostic constructor'ına göre oluşturuldu.
                diagnostics.emplace_back(
                    lineNumber,
                    1,
                    "malloc/calloc/realloc sonrasi NULL kontrolu yapilmamis: " + variableName,
                    getDefaultSeverity(),
                    "rule",
                    getId(),
                    variableName
                );
            }
        }
    }

    // DEĞİŞTİ: AST çocukları const ASTNode& yapısına uygun şekilde recursive kontrol ediliyor.
    for (const ASTNode* child : ast.getChildren()) {
        if (child == nullptr) {
            continue;
        }

        auto childDiagnostics = check(*child);
        diagnostics.insert(diagnostics.end(), childDiagnostics.begin(), childDiagnostics.end());
    }

    return diagnostics;
}

// DEĞİŞTİ: malloc/calloc/realloc atamalarını AST üzerinden bulmak için yardımcı fonksiyon eklendi.
std::vector<std::pair<std::string, int>> NullDereferenceRule::findMallocAssignments(const ASTNode& node) const {
    std::vector<std::pair<std::string, int>> mallocAssignments;

    if ((node.getType() == ASTNodeType::VARIABLE_DECL || node.getType() == ASTNodeType::ASSIGNMENT) &&
        containsAllocationCall(node)) {
        mallocAssignments.emplace_back(node.getValue(), node.getLine());
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child == nullptr) {
            continue;
        }

        auto childResults = findMallocAssignments(*child);
        mallocAssignments.insert(mallocAssignments.end(), childResults.begin(), childResults.end());
    }

    return mallocAssignments;
}

// DEĞİŞTİ: AST içinde malloc/calloc/realloc çağrısı olup olmadığını kontrol eden yardımcı fonksiyon eklendi.
bool NullDereferenceRule::containsAllocationCall(const ASTNode& node) const {
    if (node.getType() == ASTNodeType::FUNCTION_CALL &&
        (node.getValue() == "malloc" || node.getValue() == "calloc" || node.getValue() == "realloc")) {
        return true;
    }

    for (const ASTNode* child : node.getChildren()) {
        if (child != nullptr && containsAllocationCall(*child)) {
            return true;
        }
    }

    return false;
}

// DEĞİŞTİ: malloc sonrası değişken için NULL kontrolü var mı diye bakan yardımcı fonksiyon eklendi.
bool NullDereferenceRule::hasNullCheck(const std::string& varName, const ASTNode& scope) const {
    if (scope.getType() == ASTNodeType::IF_STATEMENT) {
        const std::vector<ASTNode*>& children = scope.getChildren();

        // Parser'da if dugumunun ilk child'i kosul ifadesidir.
        if (!children.empty() && children[0] != nullptr) {
            const ASTNode& condition = *children[0];

            if (condition.getType() == ASTNodeType::BINARY_OP &&
                (condition.getValue() == "==" || condition.getValue() == "!=")) {

                const auto& conditionChildren = condition.getChildren();
                if (conditionChildren.size() >= 2 && conditionChildren[0] != nullptr && conditionChildren[1] != nullptr) {
                    const std::string& left = conditionChildren[0]->getValue();
                    const std::string& right = conditionChildren[1]->getValue();

                    if ((left == varName && right == "NULL") || (left == "NULL" && right == varName)) {
                        return true;
                    }
                }
            }
        }
    }

    for (const ASTNode* child : scope.getChildren()) {
        if (child != nullptr && hasNullCheck(varName, *child)) {
            return true;
        }
    }

    return false;
}