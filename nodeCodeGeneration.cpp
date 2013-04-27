#include "node.hpp"

Value* NumberNode::codeGeneration() 
{
    return ConstantFP::get(getGlobalContext(), APFloat(number));
}


Value* VariableNode::codeGeneration()
{
    Value *var = variable_scope.getObject(varname);
    if (!var) {
        CodeGenerator::error(std::string("variable ") + varname + " unfound!");
    }
    return var;
}


Value* VariableDefinitionNode::codeGeneration() {
    Value * expr = expression->codeGeneration();
    variable_scope.addObject(varname, expr);

    Value *alloc = builder->CreateAlloca(Type::getDoubleTy(getGlobalContext()));
    builder->CreateStore(expr, alloc);

    return alloc;
}


Value* BinaryNode::codeGeneration()
{
    Value *left_value  = left_node->codeGeneration();
    Value *rigth_value = rigth_node->codeGeneration();

    if (operation == "+") {
        return builder->CreateFAdd(left_value, rigth_value);
    }
    else if (operation == "-") {
        return builder->CreateFSub(left_value, rigth_value);
    }
    else if (operation == "*") {
        return builder->CreateFMul(left_value, rigth_value);
    }
    else if (operation == "/") {
        return builder->CreateFDiv(left_value, rigth_value);
    }
    else if (operation == ">") {
        return builder->CreateFCmpOGT(left_value, rigth_value);
    }
    else if (operation == ">=") {
        return builder->CreateFCmpOGE(left_value, rigth_value);
    }
    else if (operation == "<") {
        return builder->CreateFCmpOLT(left_value, rigth_value);
    }
    else if (operation == "<=") {
        return builder->CreateFCmpOLE(left_value, rigth_value);
    }
    else if (operation == "==") {
        return builder->CreateFCmpOEQ(left_value, rigth_value);
    }
    else if (operation == "!=") {
        return builder->CreateFCmpONE(left_value, rigth_value);
    }
    else {
        CodeGenerator::error("wtf?!");
    }
}


std::vector<Type*> FunctionArgumentsNode::codeGeneration()
{
    return std::vector<Type*>(collection.size(), Type::getDoubleTy(getGlobalContext()));
}


void FunctionArgumentsNode::createArgumentScope(Function *func) {
    auto args_it = func->arg_begin();
    variable_scope.pushScope();
    for (auto it=collection.begin(); it != collection.end(); it++) {
        variable_scope.addObject((*it)->toString(), args_it++);
    }
}


Value* CreateFunctionNode::codeGeneration()
{
    Value *null_const =  ConstantFP::get(getGlobalContext(), APFloat(0.0));
    BasicBlock *external_block = builder->GetInsertBlock();

    std::vector<Type*> args = arguments->codeGeneration();
    FunctionType *type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), args, false);
    Function *function = Function::Create(type, Function::ExternalLinkage, function_name, module);

    arguments->createArgumentScope(function);

    BasicBlock *in_block = BasicBlock::Create(getGlobalContext(), "", function);
    builder->SetInsertPoint(in_block);
    
    BasicBlock *func_block = block->codeGeneration();
    builder->CreateRet(null_const);

    builder->SetInsertPoint(in_block);
    builder->CreateBr(func_block);

    builder->SetInsertPoint(external_block);

    function_scope.addObject(function_name, function);
    variable_scope.popScope();

    verifyFunction(*function);

    return null_const;
}


std::vector<Value*> FunctionParametrsNode::codeGeneration()
{
    std::vector<Value*> params;
    for (auto it=collection.begin(); it != collection.end(); it++) {
        params.push_back( (*it)->codeGeneration() );
    }
    return params;
}


Value* FunctionCallNode::codeGeneration()
{
    Function *function = function_scope.getObject(function_name);
    if (!function) {
        CodeGenerator::error(std::string("function " + function_name + "unfound"));
    }
    return builder->CreateCall( function, parametrs->codeGeneration() );
}


BasicBlock* BlockNode::codeGeneration()
{
    variable_scope.pushScope();
    function_scope.pushScope();

    BasicBlock *block = BasicBlock::Create(getGlobalContext(), "", builder->GetInsertBlock()->getParent());
    builder->SetInsertPoint(block);

    for (auto it=collection.begin(); it != collection.end(); it++) {
        (*it)->codeGeneration();
    }

    variable_scope.popScope();
    function_scope.popScope();

    return block;
}


Value* IfNode::codeGeneration()
{
    BasicBlock *this_block = builder->GetInsertBlock();
    BasicBlock *end_block = BasicBlock::Create(getGlobalContext(), "", builder->GetInsertBlock()->getParent());

    BasicBlock *tblock = true_block->codeGeneration();
    builder->CreateBr(end_block);

    BasicBlock *fblock = false_block->codeGeneration();
    builder->CreateBr(end_block);

    builder->SetInsertPoint(this_block);
    Value *cmp = expression->codeGeneration();
    builder->CreateCondBr(cmp, tblock, fblock);

    builder->SetInsertPoint(end_block);

    return cmp;
}


Value*  ReturnNode::codeGeneration()
{
    Value *ret = expression->codeGeneration(); 
    builder->CreateRet(ret);

    return ret;
}

