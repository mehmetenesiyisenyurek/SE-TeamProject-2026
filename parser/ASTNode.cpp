#include "ASTNode.h"
#include <sstream>

// parent başlangıçta nullptr — addChild çağrıldığında otomatik atanır.
ASTNode::ASTNode(ASTNodeType type, const std::string& value, int line)
    : type(type), value(value), line(line), parent(nullptr) {
}

// Ağaç yapısı olduğu için her düğüm kendi child'larını silmekle sorumlu.
ASTNode::~ASTNode() {
    for (ASTNode* child : children) {
        delete child;
    }
}


void ASTNode::addChild(ASTNode* child) {
    if (child != nullptr) {
        children.push_back(child);
        child->parent = this;
    }
}

// const reference döndürür — vektörü kopyalamaz, sadece okuma erişimi verir.
const std::vector<ASTNode*>& ASTNode::getChildren() const {
    return children;
}

ASTNodeType ASTNode::getType() const {
    return type;
}


const std::string& ASTNode::getValue() const {
    return value;
}

int ASTNode::getLine() const {
    return line;
}

// Kök düğümde (PROGRAM) nullptr döner.
ASTNode* ASTNode::getParent() const {
    return parent;
}

// Ağacı görsel olarak yazdırır:
//   PROGRAM
//     FUNCTION_DEF (value: "main", line: 1)
//       BLOCK (line: 1)
//         VARIABLE_DECL (value: "x", line: 2)
std::string ASTNode::toString(int indent) const {
    std::ostringstream oss;
    // Girinti ekle (her seviye 2 boşluk)
    for (int i = 0; i < indent; i++) {
        oss << "  ";
    }

    oss << astNodeTypeToString(type);

    if (!value.empty() || line > 0) {
        oss << " (";
        if (!value.empty()) {
            oss << "value: \"" << value << "\"";
            if (line > 0) oss << ", ";
        }
        if (line > 0) {
            oss << "line: " << line;
        }
        oss << ")";
    }
    oss << "\n";
    // Alt düğümleri recursive olarak yaz
    for (const ASTNode* child : children) {
        oss << child->toString(indent + 1);
    }
    return oss.str();
}