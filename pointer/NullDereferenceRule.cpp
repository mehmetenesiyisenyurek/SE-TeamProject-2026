#include "NullDereferenceRule.h"

vector<Diagnostic>
NullDereferenceRule::check(ASTNode* ast) {

    // Üretilecek warning mesajları
    vector<Diagnostic> diagnostics;

    // AST boşsa çık
    if (ast == nullptr) {
        return diagnostics;
    }

    // Sadece fonksiyon içinde analiz yap
    if (ast->type == "FUNCTION_DEF") {

        // malloc yapılan değişkenleri bul
        vector<pair<string, int>>
                mallocAssignments =
                findMallocAssignments(ast);

        // Her malloc için NULL kontrolü var mı bak
        for (const auto& assignment : mallocAssignments) {

            string variableName = assignment.first;

            int lineNumber = assignment.second;

            // NULL kontrolü yoksa warning üret
            if (!hasNullCheck(variableName, ast)) {

                diagnostics.emplace_back(
                        "R007",
                        "malloc/calloc/realloc sonrasi NULL kontrolu yapilmamis: "
                        + variableName,
                        lineNumber
                );
            }
        }
    }

    // Alt AST düğümlerini recursive tara
    for (ASTNode* child : ast->children) {

        vector<Diagnostic> childDiagnostics =
                check(child);

        diagnostics.insert(
                diagnostics.end(),
                childDiagnostics.begin(),
                childDiagnostics.end()
        );
    }

    return diagnostics;
}

vector<pair<string, int>>
NullDereferenceRule::findMallocAssignments(ASTNode* node) {

    // malloc yapılan değişkenler burada tutulur
    vector<pair<string, int>> mallocAssignments;

    if (node == nullptr) {
        return mallocAssignments;
    }

    // Değişken tanımı veya assignment mı?
    if (node->type == "VARIABLE_DECL" ||
        node->type == "ASSIGNMENT") {

        bool containsMallocCall = false;

        // Alt düğümlerde malloc/calloc/realloc ara
        for (ASTNode* child : node->children) {

            if (child != nullptr &&
                child->type == "CALL_EXPR" &&

                (
                        child->value == "malloc" ||
                        child->value == "calloc" ||
                        child->value == "realloc"
                )) {

                containsMallocCall = true;
                break;
            }
        }

        // Allocation bulunduysa listeye ekle
        if (containsMallocCall) {

            mallocAssignments.emplace_back(
                    node->value,
                    node->line
            );
        }
    }

    // Alt düğümlerde de ara
    for (ASTNode* child : node->children) {

        vector<pair<string, int>> childResults =
                findMallocAssignments(child);

        mallocAssignments.insert(
                mallocAssignments.end(),
                childResults.begin(),
                childResults.end()
        );
    }

    return mallocAssignments;
}

bool NullDereferenceRule::hasNullCheck(
        const string& varName,
        ASTNode* scope
) {

    if (scope == nullptr) {
        return false;
    }

    // IF bloğu mu kontrol et
    if (scope->type == "IF") {

        for (ASTNode* child : scope->children) {

            // ptr == NULL veya ptr != NULL kontrolü
            if (child != nullptr &&
                child->type == "BINARY_EXPR" &&
                child->value == varName &&

                (
                        child->operatorSymbol == "==" ||
                        child->operatorSymbol == "!="
                )) {

                // Sağ tarafta NULL var mı?
                for (ASTNode* exprChild : child->children) {

                    if (exprChild != nullptr &&
                        exprChild->type == "NULL_LITERAL") {

                        return true;
                    }
                }
            }
        }
    }

    // Alt düğümlerde de NULL kontrolü ara
    for (ASTNode* child : scope->children) {

        if (hasNullCheck(varName, child)) {
            return true;
        }
    }

    return false;
}