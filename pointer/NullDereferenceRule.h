#ifndef NULL_DEREFERENCE_RULE_H
#define NULL_DEREFERENCE_RULE_H

#include <string>
#include <vector>
#include <utility>

// DEĞİŞTİ:
// NullDereferenceRule artık proje içerisindeki ortak IRule arayüzünü kullanıyor.
#include "../syntax_analyzer/IRule.h"

// DEĞİŞTİ:
// Kendi ASTNode tanımı kaldırıldı.
// Ortak parser AST yapısı kullanılmaya başlandı.
#include "../parser/ASTNode.h"

#include "../parser/ASTNodeType.h"

// DEĞİŞTİ:
// Kendi Diagnostic yapısı kaldırıldı.
// Ortak Diagnostic sistemi kullanılmaya başlandı.
#include "../infrastructure/Diagnostic.h"

#include "../infrastructure/DiagnosticSeverity.h"

// malloc/calloc/realloc sonrasinda
// NULL kontrolu yapilip yapilmadigini kontrol eder.

// DEĞİŞTİ:
// Class artık RuleEngine ile uyumlu olması için
// IRule sınıfından türetildi.
class NullDereferenceRule : public IRule {

public:

    // DEĞİŞTİ:
    // Rule ID'si eklendi.
    // (R007 -> NULL dereference kontrolü)
    std::string getId() const override;

    // DEĞİŞTİ:
    // Kural adı eklendi.
    std::string getName() const override;

    // DEĞİŞTİ:
    // Varsayılan diagnostic severity seviyesi tanımlandı.
    DiagnosticSeverity getDefaultSeverity() const override;

    // DEĞİŞTİ:
    // RuleEngine standardına uygun hale getirildi.
    // ASTNode* yerine const ASTNode& kullanılmaya başlandı.
    std::vector<Diagnostic> check(const ASTNode& ast) override;

private:

    // malloc/calloc/realloc atamalarını AST üzerinde bulur.
    std::vector<std::pair<std::string, int>>
    findMallocAssignments(const ASTNode& node) const;

    // İlgili pointer değişkeni için NULL kontrolü yapılıp yapılmadığını kontrol eder.
    bool hasNullCheck(const std::string& varName,
                      const ASTNode& scope) const;

    // AST içerisinde malloc/calloc/realloc çağrısı var mı kontrol eder.
    bool containsAllocationCall(const ASTNode& node) const;
};

#endif
