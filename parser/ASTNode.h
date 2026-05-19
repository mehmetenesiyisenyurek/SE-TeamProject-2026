#pragma once

#include <vector>
#include <string>
#include <memory>

// Soyut sözdizimi ağacı düğümü
class ASTNode {
public:
    ASTNode(const std::string& nodeType);
    ~ASTNode();

    void addChild(std::shared_ptr<ASTNode> child);
    std::string getType() const;

private:
    std::string type;
    std::vector<std::shared_ptr<ASTNode>> children;
};
