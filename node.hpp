#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdio>

#include "llvmInterface.hpp"
#include "collectionNode.hpp"
#include "binaryNode.hpp"

class VariableNode: public ValueNode {
private:
    std::string varname;

public:
    ~VariableNode() {}
    VariableNode(std::string name) : varname(name) {}

    std::string getName();

    std::string toString();
    Value* codeGeneration();
};


class NumberNode: public ValueNode {
private:
    double number;

public:
    NumberNode(double number) : number(number) {};

    std::string toString();
    Value* codeGeneration();
};



class VariableDefinitionNode: public ValueNode {
private:
    std::string variable;
    ValueNode *expression;

public:
    ~VariableDefinitionNode(); 
    VariableDefinitionNode(std::string var, ValueNode *expr) : variable(var), expression(expr) {}
    
    std::string toString();
    Value* codeGeneration();
};


class FunctionCallNode: public ValueNode {
private:
    std::string function_name;
    FunctionParametrsNode *parametrs;

public:
    ~FunctionCallNode();
    FunctionCallNode(std::string fname, FunctionParametrsNode *params): function_name(fname), parametrs(params) {}

    std::string toString();
    Value* codeGeneration();
};


class CreateFunctionNode: public FunctionNode {
private:
    std::string function_name;
    FunctionArgumentsNode *arguments;
    BlockNode *block;

public:
    ~CreateFunctionNode();
    CreateFunctionNode(std::string fname, FunctionArgumentsNode *args, BlockNode *block) 
        : function_name(fname)
        , arguments(args)
        , block(block) 
    {}

    std::string toString();
    Function* codeGeneration();
};


class IfNode: public ValueNode {
private:
    ValueNode *expression;
    BlockNode *true_block;
    BlockNode *false_block;

public:
    ~IfNode();
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
    ~ReturnNode();
    ReturnNode(ValueNode *val): expression(val) {}

    std::string toString();
    Value* codeGeneration();
};

