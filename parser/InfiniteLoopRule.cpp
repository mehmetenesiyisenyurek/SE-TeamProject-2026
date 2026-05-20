#include "InfiniteLoopRule.h"

std::string InfiniteLoopRule::getId() const {
    return ruleId;
}
std::string InfiniteLoopRule::getName() const {
    return "Sonsuz Döngü Tespiti";
}
DiagnosticSeverity InfiniteLoopRule::getDefaultSeverity() const {
    return DiagnosticSeverity::WARNING;
}

//  ANA KONTROL FONKSİYONU

// AST'nin tamamını gezer ve sonsuz döngü ihlallerini toplar.
std::vector<Diagnostic> InfiniteLoopRule::check(ASTNode* ast) const {
    std::vector<Diagnostic> results;
    if (ast == nullptr) {
        return results;
    }
    traverse(ast, results);
    return results;
}

//  ÖZYINELEMELI GEZİNME

// AST ağacını derinlik öncelikli (DFS) olarak gezer.
// Her düğümde:
//   - WHILE_LOOP bulursa → koşulun sabit olup olmadığını kontrol eder
//   - FOR_LOOP bulursa  → koşulun boş olup olmadığını kontrol eder
// Sonra tüm alt düğümlere özyinelemeli olarak iner.
void InfiniteLoopRule::traverse(ASTNode* node, std::vector<Diagnostic>& results) const {
    if (node == nullptr) {
        return;
    }
    // WHILE DÖNGÜSÜ KONTROLÜ
    // while(1) veya while(true) gibi sabit koşullu döngüler
    if (node->getType() == ASTNodeType::WHILE_LOOP) {
        if (isWhileConditionConstant(node)) {
            std::string msg = "Satir " + std::to_string(node->getLine())
                + ": Potansiyel sonsuz dongu tespit edildi. "
                + "'while' kosulu sabit bir deger iceriyor.";
            results.push_back(Diagnostic(
                node->getLine(),                // satır
                0,                              // sütun (konum bilgisi mevcut değil)
                msg,                            // mesaj
                DiagnosticSeverity::WARNING,     // seviye
                "rule",                         // kaynak
                ruleId,                         // kural ID
                ""                              // kod parçası
            ));
        }
    }
    // FOR DÖNGÜSÜ KONTROLÜ
    // for(;;) gibi koşulsuz döngüler
    if (node->getType() == ASTNodeType::FOR_LOOP) {
        if (isForConditionEmpty(node)) {
            std::string msg = "Satir " + std::to_string(node->getLine())
                + ": Potansiyel sonsuz dongu tespit edildi. "
                + "'for' dongusunde kosul ifadesi bulunmuyor.";
            results.push_back(Diagnostic(
                node->getLine(),                // satır
                0,                              // sütun
                msg,                            // mesaj
                DiagnosticSeverity::WARNING,     // seviye
                "rule",                         // kaynak
                ruleId,                         // kural ID
                ""                              // kod parçası
            ));
        }
    }
    // Alt düğümlere özyinelemeli olarak in
    for (ASTNode* child : node->getChildren()) {
        traverse(child, results);
    }
}

//  WHILE KOŞUL KONTROLÜ

// WHILE_LOOP düğümünün ilk child'ı koşul ifadesidir.
// Parser'ın ürettiği AST yapısı:
//   WHILE_LOOP (line: X)
//     ── EXPRESSION (value: "1", line: X)   ← koşul
//     ── BLOCK (line: X)                    ← gövde

// Koşul şu durumlarda "sabit" kabul edilir:
//   - EXPRESSION tipinde ve değeri "1", "true" veya sıfır olmayan tam sayı
//   - BINARY_OP değilse (karşılaştırma içermiyorsa) ve sabit bir değer ise
bool InfiniteLoopRule::isWhileConditionConstant(ASTNode* whileNode) const {
    const auto& children = whileNode->getChildren();
    // While düğümünün en az 1 child'ı olmalı (koşul)
    // İlk child koşuldur, son child genelde BLOCK (gövde)
    if (children.empty()) {
        return false;
    }
    ASTNode* condition = children[0];
    // Koşul BLOCK ise → parser koşulu atlayıp direkt gövdeye geçmiş olabilir
    // Bu durumda koşul yok demektir → sonsuz döngü potansiyeli
    if (condition->getType() == ASTNodeType::BLOCK) {
        return false; // Koşul yok ama bu while için normal değil, false döndür
    }
    // Koşulun sabit bir "truthy" değer olup olmadığını kontrol et
    return isConstantTruthy(condition);
}

//  FOR KOŞUL KONTROLÜ
// FOR_LOOP düğümünün child'ları Parser tarafından şu sırayla eklenir:
//   [init], [koşul], [güncelleme], BLOCK

// for(;;) durumunda → yalnızca BLOCK child'ı var
// for(int i=0; ; i++) → VARIABLE_DECL, EXPRESSION(i++), BLOCK
//                        (koşul atlandı, init ve update var)

// Sonsuz döngü tespiti için iki ana durum:
//   1) Tek child BLOCK ise → for(;;) → kesinlikle koşul yok
//   2) BLOCK öncesindeki child'lar arasında koşul olabilecek bir
//      EXPRESSION/BINARY_OP var mı kontrol et
bool InfiniteLoopRule::isForConditionEmpty(ASTNode* forNode) const {
    const auto& children = forNode->getChildren();
    if (children.empty()) {
        // Child yok → anormal durum, ama koşul da yok
        return true;
    }
    // Durum 1: Tek child BLOCK ise → for(;;) — init, koşul, güncelleme hepsi boş
    if (children.size() == 1 && children[0]->getType() == ASTNodeType::BLOCK) {
        return true;
    }
    // Durum 2: BLOCK öncesi child sayısına bak
    // Parser'ın for döngüsü ayrıştırma mantığı:
    //   - init varsa: VARIABLE_DECL veya EXPRESSION (atama) eklenir
    //   - koşul varsa: EXPRESSION veya BINARY_OP eklenir
    //   - güncelleme varsa: EXPRESSION eklenir (genelde i++ gibi)
    //   - gövde: BLOCK eklenir
    //
    // Tam olarak hangi child'ın koşul olduğunu belirlemek için
    // BLOCK'tan önceki child sayısını kontrol ederiz.
    // 0 non-BLOCK child → for(;;)          → sonsuz
    // 1 non-BLOCK child → init VEYA update → koşul yok → sonsuz potansiyeli
    //                      VEYA sadece koşul var → o zaman sabit mi kontrol et
    // 2 non-BLOCK child → init + koşul VEYA init + update VEYA koşul + update
    // 3 non-BLOCK child → init + koşul + update → koşul var
    int nonBlockCount = 0;
    for (ASTNode* child : children) {
        if (child->getType() != ASTNodeType::BLOCK) {
            nonBlockCount++;
        }
    }
    // Hiç non-BLOCK child yoksa → koşul kesinlikle yok
    if (nonBlockCount == 0) {
        return true;
    }
    // 3 non-BLOCK child varsa → init, koşul, güncelleme hepsi var
    // Bu durumda koşulun sabit olup olmadığını kontrol et
    // (2. child koşuldur)
    if (nonBlockCount == 3) {
        // Koşul 2. non-BLOCK child (index 1 veya BLOCK olmayan 2. eleman)
        int idx = 0;
        for (ASTNode* child : children) {
            if (child->getType() != ASTNodeType::BLOCK) {
                if (idx == 1) {
                    // Bu koşul sabit mi?
                    return isConstantTruthy(child);
                }
                idx++;
            }
        }
    }
    // 1 non-BLOCK child → Bu child'ın rolünü kesin bilemeyiz.
    // Ancak init (VARIABLE_DECL) ise koşul yok → sonsuz potansiyeli
    if (nonBlockCount == 1) {
        for (ASTNode* child : children) {
            if (child->getType() != ASTNodeType::BLOCK) {
                // Init ise (VARIABLE_DECL) → koşul yok
                if (child->getType() == ASTNodeType::VARIABLE_DECL) {
                    return true;
                }
                // ASSIGNMENT ise → büyük olasılıkla init → koşul yok
                if (child->getType() == ASTNodeType::ASSIGNMENT) {
                    return true;
                }
                // Diğer durumlar → belirsiz, false döndür (yanlış pozitif önlenir)
                break;
            }
        }
    }
    return false;
}

//  SABİT DEĞER KONTROLÜ
// Bir ifade düğümünün "her zaman true" olan sabit bir değer
// içerip içermediğini kontrol eder.

// Sabit truthy kabul edilen durumlar:
//   - EXPRESSION tipinde, değeri "1" → while(1)
//   - EXPRESSION tipinde, değeri "true" → while(true)
//   - EXPRESSION tipinde, değeri sıfır olmayan tam sayı → while(42)

// Sabit truthy kabul EDİLMEYEN durumlar:
//   - EXPRESSION tipinde, değeri "0" → while(0) sonsuz değil
//   - BINARY_OP tipinde → while(x > 0) gibi karşılaştırmalar dinamik
//   - FUNCTION_CALL tipinde → while(func()) dinamik
//   - EXPRESSION tipinde, değeri bir değişken adı → while(x) dinamik
bool InfiniteLoopRule::isConstantTruthy(ASTNode* expr) const {
    if (expr == nullptr) {
        return false;
    }
    // Sadece basit EXPRESSION düğümlerini kontrol et
    // BINARY_OP, FUNCTION_CALL gibi karmaşık ifadeler dinamik kabul edilir
    if (expr->getType() != ASTNodeType::EXPRESSION) {
        return false;
    }
    const std::string& val = expr->getValue();
    // "true" anahtar kelimesi
    if (val == "true" || val == "TRUE") {
        return true;
    }
    // Sayısal değer kontrolü: sıfır olmayan tam sayı ise sabit truthy
    // Boş string veya değişken adı ise false
    if (val.empty()) {
        return false;
    }
    // İlk karakter rakam mı kontrol et (sayısal literal)
    bool isNumeric = true;
    for (char c : val) {
        if (!std::isdigit(c)) {
            isNumeric = false;
            break;
        }
    }
    if (isNumeric && !val.empty()) {
        // "0" → false (while(0) sonsuz değil)
        // Diğer sayılar ("1", "42", "100") → true (while(1) sonsuz)
        try {
            int numVal = std::stoi(val);
            return numVal != 0;
        } catch (...) {
            return false;
        }
    }
    return false;
}