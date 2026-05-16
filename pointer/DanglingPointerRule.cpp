#include "DanglingPointerRule.h"

using namespace std;

vector<Diagnostic> DanglingPointerRule::check(ASTNode* ast) {
    return {};
}

vector<pair<string, int>> DanglingPointerRule::collectFreeCalls(ASTNode* node) {
    return {};
}

bool DanglingPointerRule::hasNullAssignmentAfterFree(
        const string& varName,
        int freeLine,
        ASTNode* scope
) {
    return false;
}