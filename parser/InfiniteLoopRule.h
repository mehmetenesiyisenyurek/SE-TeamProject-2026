#ifndef INFINITELOOPRULE_H
#define INFINITELOOPRULE_H
#include "IRule.h"
#include "ASTNode.h"
#include "Diagnostic.h"
#include <vector>
#include <string>
// InfiniteLoopRule
// Potansiyel sonsuz döngü yapılarını tespit eder.
//
// Kontrol edilen durumlar:
//   1) while(1), while(true)  → koşul sabit, döngü hiç bitmez
//   2) for(;;)                → koşul boş, sonsuz döngü
//
// Seviye: WARNING
// Mesaj : "Satır X: Potansiyel sonsuz döngü tespit edildi."
class InfiniteLoopRule : public IRule {
private:
  std::string ruleId = "R006";
  // AST'yi özyinelemeli gezerek WHILE_LOOP ve FOR_LOOP düğümlerini kontrol eder.
  // Bulunan ihlalleri results vektörüne ekler.
  void traverse(ASTNode* node, std::vector<Diagnostic>& results) const;
  // WHILE_LOOP düğümündeki koşulun sabit olup olmadığını kontrol eder.
  // while(1), while(true) gibi kalıpları yakalar.
  bool isWhileConditionConstant(ASTNode* whileNode) const;
  // FOR_LOOP düğümündeki koşulun boş olup olmadığını kontrol eder.
  // for(;;) gibi kalıpları yakalar.
  bool isForConditionEmpty(ASTNode* forNode) const;
  // Bir ifade düğümünün sabit "truthy" değer olup olmadığını kontrol eder.
  // "1", "true", sıfır olmayan tam sayılar → true döner.
  bool isConstantTruthy(ASTNode* expr) const;
public:
  InfiniteLoopRule() = default;
  // IRule arayüzü implementasyonları
  std::string getId() const override;
  std::string getName() const override;
  DiagnosticSeverity getDefaultSeverity() const override;
  // AST üzerinde sonsuz döngü kontrolü yapar.
  // Tüm WHILE_LOOP ve FOR_LOOP düğümlerini gezer,
  // ihlal bulunanları Diagnostic listesi olarak döndürür.
  std::vector<Diagnostic> check(ASTNode* ast) const override;
};
#endif // INFINITELOOPRULE_H