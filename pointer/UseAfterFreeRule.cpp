#include "UseAfterFreeRule.h"

vector<Diagnostic>
UseAfterFreeRule::check(ASTNode* ast) {

    // Üretilecek diagnostic mesajları
    vector<Diagnostic> diagnostics;

    // AST boşsa çık
    if (ast == nullptr) {
        return diagnostics;
    }

    // Sadece fonksiyon içinde analiz yap
    if (ast->type == "FUNCTION_DEF") {

        // Tüm free(ptr) çağrılarını bul
        vector<pair<string, int>> freeCalls =
                findFreeCallSites(ast);

        // Her free çağrısını kontrol et
        for (const auto& freeCall : freeCalls) {

            string variableName = freeCall.first;

            int freeLine = freeCall.second;

            // free sonrası tekrar kullanım var mı?
            if (isUsedAfterFree(
                    variableName,
                    freeLine,
                    ast
            )) {

                diagnostics.emplace_back(
                        "R008",
                        "free edildikten sonra pointer tekrar kullaniliyor: "
                        + variableName,
                        freeLine
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
UseAfterFreeRule::findFreeCallSites(ASTNode* node) {

        // free çağrıları burada tutulur
        vector<pair<string, int>> freeCalls;

        if (node == nullptr) {
            return freeCalls;
        }

        // free(...) çağrısı mı kontrol et
        if (node->type == "CALL_EXPR" &&
            node->value == "free") {

            // free(ptr) parametresi var mı?
            if (!node->children.empty() &&
                node->children[0] != nullptr) {

                string variableName =
                        node->children[0]->value;

                int lineNumber = node->line;

                // Pointer adı ve satırı kaydet
                freeCalls.emplace_back(
                        variableName,
                        lineNumber
                );
                }
            }

        // Alt düğümlerde de free ara
        for (ASTNode* child : node->children) {

            vector<pair<string, int>> childResults =
                    findFreeCallSites(child);

            freeCalls.insert(
                    freeCalls.end(),
                    childResults.begin(),
                    childResults.end()
            );
        }

        return freeCalls;
    }


bool UseAfterFreeRule::isUsedAfterFree(
        const string& varName,
        int freeLine,
        ASTNode* scope
) {

    if (scope == nullptr) {
        return false;
    }

    // Sadece free sonrası satırlara bak
    if (scope->line > freeLine) {

        // Eğer ptr = NULL yapılmışsa güvenli kabul edilir
        if (isNullAssignment(varName, scope)) {
            return false;
        }

        // Aynı değişken tekrar kullanılmış mı?
        if (scope->value == varName) {
            return true;
        }
    }

    // Alt düğümlerde de ara
    for (ASTNode* child : scope->children) {

        if (isUsedAfterFree(
                varName,
                freeLine,
                child
        )) {

            return true;
        }
    }

    return false;
}

bool UseAfterFreeRule::isNullAssignment(
        const string& varName,
        ASTNode* node
) {

    if (node == nullptr) {
        return false;
    }

    // ptr = ...
    if (node->type == "ASSIGNMENT" &&
        node->value == varName) {

        // Sağ tarafta NULL var mı?
        for (ASTNode* child : node->children) {

            if (child != nullptr &&
                child->type == "NULL_LITERAL") {

                return true;
            }
        }
    }

    return false;
}
