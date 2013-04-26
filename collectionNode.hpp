#pragma once

#include "nodeInterface.hpp"

class FunctionArgumentsNode: public CollectionNode, public CodeGenerator<std::vector<Type*> > {
public:
    std::vector<Type*> codeGeneration();
};


class FunctionParametrsNode: public CollectionNode, public CodeGenerator<std::vector<Value*> > {
public:
    std::vector<Value*> codeGeneration();
};


class BlockNode: public CollectionNode, public CodeGenerator<BasicBlock*> {
public:
    BasicBlock* codeGeneration();
};

