#ifndef NULL_DEREFERENCE_RULE_H
#define NULL_DEREFERENCE_RULE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

// AST düğüm yapısı
struct ASTNode {

    // Düğüm tipi
    // Örn: FUNCTION_DEF, VARIABLE_DECL, CALL_EXPR
    string type;

    // Düğüm değeri
    // Örn: değişken adı veya fonksiyon adı
    string value;

    // Operatör bilgisi
    // Örn: == , !=
    string operatorSymbol;

    // Kod satır numarası
    int line = 0;

    // Alt AST düğümleri
    vector<ASTNode*> children;
};

// Diagnostic mesaj yapısı
struct Diagnostic {

    // Rule kodu
    // Örn: R007
    string ruleCode;

    // Kullanıcıya gösterilecek mesaj
    string message;

    // Hatanın bulunduğu satır
    int line;

    Diagnostic(
            string ruleCode,
            string message,
            int line
    )
            : ruleCode(ruleCode),
              message(message),
              line(line) {}
};

// malloc sonrası NULL kontrolü yapılmış mı kontrol eder
class NullDereferenceRule {

public:

    // Ana kontrol fonksiyonu
    vector<Diagnostic> check(ASTNode* ast);

private:

    // malloc/calloc/realloc atamalarını bulur
    vector<pair<string, int>>
    findMallocAssignments(ASTNode* node);

    // if(ptr == NULL) benzeri kontrol var mı bakar
    bool hasNullCheck(
            const string& varName,
            ASTNode* scope
    );
};

#endif