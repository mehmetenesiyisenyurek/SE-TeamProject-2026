#include "ASTNode.h"

ASTNode::ASTNode(const std::string& nodeType) : type(nodeType) {
}

ASTNode::~ASTNode() {
}

void ASTNode::addChild(std::shared_ptr<ASTNode> child) {
    children.push_back(child);
}

std::string ASTNode::getType() const {
    return type;
}
