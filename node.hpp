#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdio>

#include "codeGenerator.hpp"

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


class ValueNodeCollection: public Node {
protected:
    std::vector<ValueNode*> collection;

public:
    virtual  ~ValueNodeCollection() {}
    ValueNodeCollection() {}

    void append(ValueNode *element)
    {
        collection.push_back(element);
    }

    std::string toString();
};


class FunctionArgumentsNode: public ValueNodeCollection, public CodeGenerator<std::vector<Type*> > {
public:
    void createArgumentScope(Function *func);
    std::vector<Type*> codeGeneration();
};


class FunctionParametrsNode: public ValueNodeCollection, public CodeGenerator<std::vector<Value*> > {
public:
    std::vector<Value*> codeGeneration();
};


class BlockNode: public ValueNodeCollection, public CodeGenerator<BasicBlock*> {
public:
    BasicBlock* codeGeneration();
};


class VariableNode: public ValueNode {
private:
    std::string varname;

public:
    ~VariableNode() {}
    VariableNode(std::string name) : varname(name) {}

    std::string toString();
    Value* codeGeneration();
};


class NumberNode: public ValueNode {
private:
    double number;

public:
    ~NumberNode() {}
    NumberNode(double number) : number(number) {};

    std::string toString();
    Value* codeGeneration();
};


class VariableDefinitionNode: public ValueNode {
private:
    std::string varname;
    ValueNode *expression;

public:
    ~VariableDefinitionNode() { delete expression; }

    std::string toString();
    Value* codeGeneration();
};


class BinaryNode: public ValueNode {
private:
    std::string operation;
    ValueNode *left_node;
    ValueNode *rigth_node;

public:
    ~BinaryNode() { delete left_node, rigth_node; }
    BinaryNode(std::string oper, ValueNode* lnode, ValueNode* rnode) 
        : operation(oper)
        , left_node(lnode)
        , rigth_node(rnode) 
    {}

    std::string toString();
    Value* codeGeneration();
};


class FunctionCallNode: public ValueNode {
private:
    std::string function_name;
    FunctionParametrsNode *parametrs;

public:
    ~FunctionCallNode() { delete parametrs; }
    FunctionCallNode(std::string fname, FunctionParametrsNode *params): function_name(fname), parametrs(params) {}

    std::string toString();
    Value* codeGeneration();
};


class CreateFunctionNode: public ValueNode {
private:
    std::string function_name;
    FunctionArgumentsNode *arguments;
    BlockNode *block;

public:
    ~CreateFunctionNode() { delete arguments, block; }
    CreateFunctionNode(std::string fname, FunctionArgumentsNode *args, BlockNode *block) 
        : function_name(fname)
        , arguments(args)
        , block(block) 
    {}

    std::string toString();
    Value* codeGeneration();
};


class IfNode: public ValueNode {
private:
    ValueNode *expression;
    BlockNode *true_block;
    BlockNode *false_block;

public:
    ~IfNode() { delete expression, true_block, false_block; }
    IfNode(ValueNode *expr, BlockNode *tblock, BlockNode *fblock)
        : expression(expr)
        , true_block(tblock)
        , false_block(fblock)
    {}

    std::string toString();
    Value* codeGeneration();
};


class ReturnNode: public ValueNode {
private:
    ValueNode *expression;

public:
    ~ReturnNode() { delete expression; }
    ReturnNode(ValueNode *val): expression(val) {}

    std::string toString();
    Value* codeGeneration();
};

