#ifndef DANGLING_POINTER_RULE_H
#define DANGLING_POINTER_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

class DanglingPointerRule {
public:
    std::vector<Diagnostic> check(ASTNode* ast);

private:
    std::vector<std::pair<std::string, int>> collectFreeCalls(ASTNode* node);

    bool hasNullAssignmentAfterFree(
            const std::string& varName,
            int freeLine,
            ASTNode* scope
    );
};

#endif