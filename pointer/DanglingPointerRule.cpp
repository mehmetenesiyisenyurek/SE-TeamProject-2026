#include "DanglingPointerRule.h"

using namespace std;

vector<Diagnostic> DanglingPointerRule::check(ASTNode* ast) {
    return {};
}

vector<pair<string, int>> DanglingPointerRule::collectFreeCalls(ASTNode* node) {
    vector<pair<string, int>> freeCalls;

    if (node == nullptr) {
        return freeCalls;
    }

    // free(...) fonksiyon cagrisi mi?
    if (node->getType() == ASTNodeType::FUNCTION_CALL &&
        node->getValue() == "free") {

        const vector<ASTNode*>& children = node->getChildren();

        // free(ptr) parametresini al
        if (!children.empty() && children[0] != nullptr) {
            string variableName = children[0]->getValue();
            int lineNumber = node->getLine();

            freeCalls.emplace_back(variableName, lineNumber);
        }
        }

    // Alt dugumlerde de free ara
    for (ASTNode* child : node->getChildren()) {
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

bool DanglingPointerRule::hasNullAssignmentAfterFree(
        const string& varName,
        int freeLine,
        ASTNode* scope
) {
    return false;
}