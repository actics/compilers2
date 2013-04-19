#include <iostream>
#include <vector>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/IRBuilder.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Bitcode/ReaderWriter.h>

#define EPS 0.0000000001

using namespace llvm;


Function * createPrintFunction(IRBuilder<> & builder, Module* module)
{
    std::vector<Type*> print_params(1, Type::getDoubleTy(getGlobalContext()));
    FunctionType * print_type = FunctionType::get(Type::getVoidTy(getGlobalContext()), print_params, false);
    Function * print_func = Function::Create(print_type, Function::ExternalLinkage, "_Z5printd", module);

    verifyFunction(*print_func);
    
    return print_func;
}


Function * createFabsFunction(IRBuilder<> & builder, Module* module)
{
    std::vector<Type*> fabs_params(1, Type::getDoubleTy(getGlobalContext()));
    FunctionType * fabs_type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), fabs_params, false);
    Function * fabs_func = Function::Create(fabs_type, Function::ExternalLinkage, "llvm.fabs.f64", module);

    verifyFunction(*fabs_func);
    
    return fabs_func;
}


Function * createSqrtFunction(IRBuilder<> & builder, Module* module, Function * fabs_func)
{
    std::vector<Type*> sqrt_params(1, Type::getDoubleTy(getGlobalContext()));
    FunctionType* sqrt_type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), sqrt_params, false);
    Function * sqrt_func = Function::Create(sqrt_type, Function::ExternalLinkage, "msqrt", module);

    Value* x = sqrt_func->arg_begin()++;

    Value * eps_const = ConstantFP::get(getGlobalContext(), APFloat((double) EPS));
    Value * _0_const  = ConstantFP::get(getGlobalContext(), APFloat((double) 0.0));
    Value * _2_const  = ConstantFP::get(getGlobalContext(), APFloat((double) 2.0));

    BasicBlock* entry_bb   = BasicBlock::Create(getGlobalContext(), "entry", sqrt_func);
    BasicBlock* loop_bb    = BasicBlock::Create(getGlobalContext(), "loop", sqrt_func);
    BasicBlock* return_bb  = BasicBlock::Create(getGlobalContext(), "return", sqrt_func);

    /********************************/

    builder.SetInsertPoint(entry_bb);

    Value * l_val = builder.CreateAlloca(Type::getDoubleTy(getGlobalContext()));
    Value * r_val = builder.CreateAlloca(Type::getDoubleTy(getGlobalContext()));

    builder.CreateStore(_0_const, l_val);
    builder.CreateStore(x, r_val);

    builder.CreateBr(loop_bb);

    /********************************/

    builder.SetInsertPoint(loop_bb);

    Value * _1 = builder.CreateLoad(l_val);
    Value * _2 = builder.CreateLoad(r_val);
    
    Value * _3 = builder.CreateFAdd(_1, _2);
    Value * _4 = builder.CreateFDiv(_3, _2_const);
    Value * _5 = builder.CreateFMul(_4, _4);

    Value * _6 = builder.CreateFSub(_5, x);
    Value * _1_cmp = builder.CreateFCmpOGT(_6, eps_const);

    Value * _7 = builder.CreateSelect(_1_cmp, _1, _4);
    Value * _8 = builder.CreateSelect(_1_cmp, _4, _2);

    builder.CreateStore(_7, l_val);
    builder.CreateStore(_8, r_val);

    std::vector<Value*> fabs_call_args;
    fabs_call_args.push_back(_6);
    Value * _9 = builder.CreateCall(fabs_func, fabs_call_args);
    Value * _2_cmp = builder.CreateFCmpOGT(_9, eps_const);
    
    builder.CreateCondBr(_2_cmp, loop_bb, return_bb);

    /********************************/

    builder.SetInsertPoint(return_bb);

    builder.CreateRet(_4);

    verifyFunction(*sqrt_func);

    return sqrt_func;
}


Function *createMainFunction(IRBuilder<> & builder, Module* module, Function * sqrt_func, Function * print_func)
{
    FunctionType *main_type = FunctionType::get(Type::getDoubleTy(getGlobalContext()), false);
    Function     *main_func = Function::Create(main_type, Function::ExternalLinkage, "main", module);
    
    BasicBlock* entry_bb   = BasicBlock::Create(getGlobalContext(), "", main_func);

    Value * _2_const  = ConstantFP::get(getGlobalContext(), APFloat((double) 2.0));

    builder.SetInsertPoint(entry_bb);
    std::vector<Value*> params;
    params.push_back(_2_const);
    Value * _2_sqrt = builder.CreateCall(sqrt_func, params);
    params.clear();
    params.push_back(_2_sqrt);
    builder.CreateCall(print_func, params);

    builder.CreateRet(_2_const);

    verifyFunction(*main_func);

    return main_func;
}


int main(int argc, char** argv) 
{
    IRBuilder<> builder(getGlobalContext());

    Module* module = new Module("sqrt_module", getGlobalContext());
    
    Function * fabs  = createFabsFunction(builder, module);
    Function * print = createPrintFunction(builder, module);
    Function * sqrt  = createSqrtFunction(builder, module, fabs);
    Function * main  = createMainFunction(builder, module, sqrt, print);

    module->dump();

    std::string errorString;
    raw_fd_ostream bitcode("llvmsqrt.bc", errorString, 0);
    WriteBitcodeToFile(module, bitcode);
    bitcode.close();

    return 0;
}

