#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "llvmInterface.hpp"

class Node {
public:
    virtual ~Node() {}

    virtual std::string toString() = 0;
};


class ValueNode: public Node, public CodeGenerator<Value*> {
public:
    virtual ~ValueNode() {}

    virtual std::string toString() = 0;
    virtual Value* codeGeneration() = 0;
};


class FunctionNode: public Node, public CodeGenerator<Function*> {
public:
    virtual ~FunctionNode() {}

    virtual std::string toString() = 0;
    virtual Function* codeGeneration() = 0;
};


class CollectionNode: public Node {
protected:
    std::vector<ValueNode*> collection;

public:
    virtual  ~CollectionNode() {}
    CollectionNode() {}

    void append(ValueNode *element);

    std::string toString();
};

