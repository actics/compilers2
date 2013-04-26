#pragma once

#include "nodeInterface.hpp"


class BinaryNode: public ValueNode {
private:
    std::string operation;
    ValueNode *left_node;
    ValueNode *rigth_node;

public:
    virtual ~BinaryNode();
    BinaryNode(std::string oper, ValueNode* lnode, ValueNode* rnode) 
        : operation(oper)
        , left_node(lnode)
        , rigth_node(rnode) 
    {}

    std::string toString();
    Value* codeGeneration();
};

