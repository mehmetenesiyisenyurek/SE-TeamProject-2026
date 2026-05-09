#ifndef USE_AFTER_FREE_RULE_H
#define USE_AFTER_FREE_RULE_H

#include "NullDereferenceRule.h"

#include <string>
#include <vector>
#include <utility>

using namespace std;

// free sonrası pointer tekrar kullanılmış mı kontrol eder
class UseAfterFreeRule {

public:

    // Ana kontrol fonksiyonu
    vector<Diagnostic> check(ASTNode* ast);

private:

    // AST içinde free(ptr) çağrılarını toplar
    vector<pair<string, int>>
    findFreeCallSites(ASTNode* node);

    // free sonrası aynı pointer tekrar kullanılmış mı kontrol eder
    bool isUsedAfterFree(
            const string& varName,
            int freeLine,
            ASTNode* scope
    );

    // ptr = NULL atanmış mı kontrol eder
    bool isNullAssignment(
            const string& varName,
            ASTNode* node
    );
};

#endif