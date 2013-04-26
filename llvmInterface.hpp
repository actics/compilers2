#pragma once

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/IRBuilder.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

#include "codeScope.hpp"

using namespace llvm;

class CodeGeneratorBase {
public:
    static IRBuilder<> *builder;
    static Module *module;
    static CodeScope<Value*> variable_scope;
    static CodeScope<Function*> function_scope;

public:
    static void setBuilder(IRBuilder<> *bldr) {
        builder = bldr;
    }

    static void setModule(Module *mod) {
        module = mod;
    }
};


template <typename LlvmCode>
class CodeGenerator: public CodeGeneratorBase {
public:
    virtual ~CodeGenerator() {}
    
    virtual LlvmCode codeGeneration() = 0;
};

