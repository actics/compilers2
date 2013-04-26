#include "llvmInterface.hpp"

IRBuilder<> *CodeGeneratorBase::builder = 0;
Module *CodeGeneratorBase::module = 0;
CodeScope<Value*> CodeGeneratorBase::variable_scope;
CodeScope<Function*> CodeGeneratorBase::function_scope;

