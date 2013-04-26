#include "node.hpp"

std::string VariableNode::getName() 
{
    return varname;
}


std::string VariableNode::toString() 
{
    return varname;
}


Value* VariableNode::codeGeneration()
{
    return variable_scope.getObject(varname);
}


std::string NumberNode::toString()
{
    std::ostringstream stream;
    stream << number;
    return stream.str();
}


Value* NumberNode::codeGeneration() {
    return ConstantFP::get(getGlobalContext(), APFloat(number));
}


VariableDefinitionNode::~VariableDefinitionNode() {
    delete expression;
}


std::string VariableDefinitionNode::toString()
{
    std::ostringstream stream;
    stream << "( " << variable << " = " << expression->toString() << " )";
    return stream.str();
}


Value* VariableDefinitionNode::codeGeneration()
{
    Value *expr = expression->codeGeneration();
    variable_scope.addObject(variable, expr);
    Value *alloc = builder->CreateAlloca(Type::getDoubleTy(getGlobalContext()));
    builder->CreateStore(expr, alloc);

    return alloc;
}


FunctionCallNode::~FunctionCallNode()
{
    delete parametrs;
}


std::string FunctionCallNode::toString()
{
    std::ostringstream stream;
    stream << "( call " << function_name << " " << parametrs->toString() << " )";
    return stream.str();
}


Value* FunctionCallNode::codeGeneration()
{
    return builder->CreateCall( function_scope.getObject(function_name), parametrs->codeGeneration() );
}


CreateFunctionNode::~CreateFunctionNode()
{
    delete arguments;
    delete block;
}


std::string CreateFunctionNode::toString() 
{
    std::ostringstream stream;
    stream << "( create " 
           << function_name << " " 
           << arguments->toString() << " " 
           << block->toString() << " "
           << ")";
    return stream.str();
}


Function* CreateFunctionNode::codeGeneration()
{
    std::vector<Type*> args = arguments->codeGeneration();
    FunctionType *type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), args, false);
    Function *function = Function::Create(type, Function::ExternalLinkage, function_name, module);

    function_scope.addObject(function_name, function);

    return function;
}


IfNode::~IfNode()
{
    delete expression;
    delete true_block;
    delete false_block;
}


std::string IfNode::toString()
{
    std::ostringstream stream;
    stream << "( if " 
           << expression->toString()  << " then " 
           << true_block->toString()  << " else " 
           << false_block->toString() << " ifend "
           << ")";
    return stream.str();
}


Value* IfNode::codeGeneration()
{
    BasicBlock *this_block = builder->GetInsertBlock();

    BasicBlock *tblock = true_block->codeGeneration();
    builder->SetInsertPoint(tblock);
    builder->CreateBr(this_block);

    BasicBlock *fblock = false_block->codeGeneration();
    builder->SetInsertPoint(fblock);
    builder->CreateBr(this_block);

    builder->SetInsertPoint(this_block);
    Value *cmp = expression->codeGeneration();

    builder->CreateCondBr(cmp, tblock, fblock);

    return cmp;
}


ReturnNode::~ReturnNode()
{
    delete expression;
}


std::string ReturnNode::toString()
{
    std::ostringstream stream;
    stream << "( return " << expression->toString() << ")";
    return stream.str();
}


Value*  ReturnNode::codeGeneration()
{
    Value *ret = expression->codeGeneration(); 
    builder->CreateRet(ret);

    return ret;
}

