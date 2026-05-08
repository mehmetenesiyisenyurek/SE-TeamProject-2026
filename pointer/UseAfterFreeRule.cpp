#include "UseAfterFreeRule.h"

vector<Diagnostic>
UseAfterFreeRule::check(ASTNode* ast) {
    return {};
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
