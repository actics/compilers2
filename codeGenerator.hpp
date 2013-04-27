#pragma once

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>
#include <map>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/IRBuilder.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

template <typename LlvmValue>
class CodeScope {
private:
    std::vector<std::map<std::string, LlvmValue> > scope;

public:
    ~CodeScope() {}

    void pushScope();
    void popScope();
    LlvmValue getObject(std::string name);
    void addObject(std::string name, LlvmValue object);
};


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

    static void error(std::string msg) {
        fprintf(stderr, "%s\n", msg.c_str());
        exit(1);
    }
};


template <typename LlvmCode>
class CodeGenerator: public CodeGeneratorBase {
public:
    virtual ~CodeGenerator() {}
    
    virtual LlvmCode codeGeneration() = 0;
};

