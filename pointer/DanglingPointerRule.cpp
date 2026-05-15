#include "DoubleFreeRule.h"

using namespace std;

vector<Diagnostic> DoubleFreeRule::check(ASTNode* ast) {
    return {};
}

vector<pair<string, int>> DoubleFreeRule::collectFreeCalls(ASTNode* func) {
    return {};
}

bool DoubleFreeRule::hasReallocationBetween(
        const string& varName,
        int line1,
        int line2,
        ASTNode* scope
) {
    return false;
}