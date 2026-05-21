#include "DoubleFreeRule.h"

using namespace std;

vector<Diagnostic> DoubleFreeRule::check(ASTNode* ast) {
    vector<Diagnostic> diagnostics;

    if (ast == nullptr) {
        return diagnostics;
    }

    if (ast->getType() == ASTNodeType::FUNCTION_DEF) {
        vector<pair<string, int>> freeCalls =
                collectFreeCalls(ast);

        for (size_t i = 0; i < freeCalls.size(); i++) {
            for (size_t j = i + 1; j < freeCalls.size(); j++) {
                string firstVar = freeCalls[i].first;
                string secondVar = freeCalls[j].first;

                int firstLine = freeCalls[i].second;
                int secondLine = freeCalls[j].second;

                // Ayni pointer ikinci kez free ediliyor mu?
                if (firstVar == secondVar) {
                    bool reallocated =
                            hasReallocationBetween(
                                    firstVar,
                                    firstLine,
                                    secondLine,
                                    ast
                            );

                    // Arada yeniden malloc/calloc/realloc yoksa double free
                    if (!reallocated) {
                        diagnostics.emplace_back(
                                secondLine,
                                0,
                                "Ayni pointer iki kez free edilmis: " + firstVar,
                                DiagnosticSeverity::CRITICAL,
                                "rule",
                                "R010",
                                ""
                        );
                    }
                }
            }
        }
    }

    // Alt fonksiyon veya bloklari da tara
    for (ASTNode* child : ast->getChildren()) {
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

vector<pair<string, int>> DoubleFreeRule::collectFreeCalls(ASTNode* func) {
    vector<pair<string, int>> freeCalls;

    if (func == nullptr) {
        return freeCalls;
    }

    // free(...) fonksiyon cagrisi mi kontrol et
    if (func->getType() == ASTNodeType::FUNCTION_CALL &&
        func->getValue() == "free") {

        const vector<ASTNode*>& children = func->getChildren();

        // free(ptr) parametresini al
        if (!children.empty() && children[0] != nullptr) {
            string variableName = children[0]->getValue();
            int lineNumber = func->getLine();

            freeCalls.emplace_back(variableName, lineNumber);
        }
        }

    // Alt dugumlerde de free cagrisi ara
    for (ASTNode* child : func->getChildren()) {
        vector<pair<string, int>> childResults =
                collectFreeCalls(child);

        freeCalls.insert(
                freeCalls.end(),
                childResults.begin(),
                childResults.end()
        );
    }

    return freeCalls;
}

bool DoubleFreeRule::hasReallocationBetween(
        const string& varName,
        int line1,
        int line2,
        ASTNode* scope
) {
    if (scope == nullptr) {
        return false;
    }

    // Sadece iki free arasindaki satirlara bak
    if (scope->getLine() > line1 &&
        scope->getLine() < line2) {

        // ptr = malloc(...)
        if ((scope->getType() == ASTNodeType::ASSIGNMENT ||
             scope->getType() == ASTNodeType::VARIABLE_DECL) &&
            scope->getValue() == varName) {

            for (ASTNode* child : scope->getChildren()) {
                if (child != nullptr &&
                    child->getType() == ASTNodeType::FUNCTION_CALL &&
                    (
                            child->getValue() == "malloc" ||
                            child->getValue() == "calloc" ||
                            child->getValue() == "realloc"
                    )) {
                    return true;
                    }
            }
            }
        }

    // Alt dugumlerde de yeniden allocation ara
    for (ASTNode* child : scope->getChildren()) {
        if (hasReallocationBetween(
                varName,
                line1,
                line2,
                child
        )) {
            return true;
        }
    }

    return false;
}