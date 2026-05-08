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
    return false;
}

bool UseAfterFreeRule::isNullAssignment(
        const string& varName,
        ASTNode* node
) {
    return false;
}