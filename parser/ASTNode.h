#ifndef ASTNODE_H
#define ASTNODE_H
#include <string>
#include <vector>
#include "ASTNodeType.h"
// Soyut Sözdizim Ağacı'ndaki tek bir düğümü temsil eder.
// Kaynak kodun tam hiyerarşik modelini tutar.
// Parser tarafından üretilir; SyntaxAnalyzer, tüm IRule kuralları ve
// MetricCalculator tarafından kullanılır.
class ASTNode {
private:
    ASTNodeType type;
    std::string value;             // Düğümün değeri
    int line;                      // Kaynak koddaki satır numarası
    std::vector<ASTNode*> children; // Alt düğüm listesi
    ASTNode* parent;               // Üst düğüm işaretçisi
public:

    ASTNode(ASTNodeType type, const std::string& value = "", int line = 0);
    // Destructor — tüm child düğümleri özyinelemeli olarak siler
    ~ASTNode();

    // Alt düğüm ekler. Child'ın parent işaretçisini otomatik olarak this yapar.
    void addChild(ASTNode* child);

    // Alt düğüm listesini döndürür.
    const std::vector<ASTNode*>& getChildren() const;
    // Düğüm türünü döndürür (FUNCTION_DEF, VARIABLE_DECL, IF_STATEMENT, vb.)
    ASTNodeType getType() const;

    // Düğümün değerini döndürür.
    const std::string& getValue() const;


    // Kaynak koddaki satır numarasını döndürür.
    int getLine() const;

    // Üst düğümü döndürür. Kök düğümde nullptr döner.
    ASTNode* getParent() const;

    // Debug/test amaçlı: Ağacı girintili formatta string olarak döndürür.
    // indent: Girinti derinliği (varsayılan: 0, her seviyede 2 artar)
    std::string toString(int indent = 0) const;
};
#endif // ASTNODE_H