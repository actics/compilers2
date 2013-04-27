#include "driver.hpp"

int Driver::parse(FILE * code_file)
{
    yyin = code_file;
    yy_flex_debug = trace_scanning;
    IRBuilder<> builder = IRBuilder<>(getGlobalContext());
    CodeGeneratorBase::setBuilder(&builder);
    yy::Parser parser(*this);
    parser.set_debug_level(trace_parsing);

    return parser.parse();
}


std::string Driver::getAstString() 
{
    return node->toString();
}


Module* Driver::codeGeneration()
{
    IRBuilder<> builder(getGlobalContext());
    Module* module = new Module("module.1", getGlobalContext());

    CodeGeneratorBase::setModule(module);
    CodeGeneratorBase::setBuilder(&builder);
    
    FunctionType *main_type = FunctionType::get(Type::getInt32Ty(getGlobalContext()), false);
    Function     *main_func = Function::Create(main_type, Function::ExternalLinkage, "main", module);
    Value * ret_const  = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0, false);
    
    BasicBlock* entry_bb   = BasicBlock::Create(getGlobalContext(), "", main_func);

    builder.SetInsertPoint(entry_bb);

    BasicBlock *block = node->codeGeneration();
    builder.CreateRet(ret_const);

    builder.SetInsertPoint(entry_bb);
    builder.CreateBr(block);

    verifyFunction(*main_func);

    return module;
}


void Driver::deleteAst() 
{
    delete node;
}

void Driver::error(const std::string& mess)
{
    fprintf(stderr, "%s\n", mess.c_str());
}

