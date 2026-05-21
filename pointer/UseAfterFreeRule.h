#ifndef USE_AFTER_FREE_RULE_H
#define USE_AFTER_FREE_RULE_H

#include <string>
#include <vector>
#include <utility>

#include "../syntax_analyzer/IRule.h"
#include "../parser/ASTNode.h"
#include "../parser/ASTNodeType.h"
#include "../infrastructure/Diagnostic.h"
#include "../infrastructure/DiagnosticSeverity.h"

// free(ptr) çağrısından sonra aynı pointer tekrar kullanılmış mı kontrol eder.
class UseAfterFreeRule : public IRule {
public:
  std::string getId() const override;
  std::string getName() const override;
  DiagnosticSeverity getDefaultSeverity() const override;
  std::vector<Diagnostic> check(const ASTNode& ast) override;

private:
  std::vector<std::pair<std::string, int>> findFreeCallSites(const ASTNode& node) const;
  bool isUsedAfterFree(const std::string& varName, int freeLine, const ASTNode& scope) const;
  bool isNullAssignment(const std::string& varName, const ASTNode& node) const;
};

#endif