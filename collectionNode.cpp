#include "collectionNode.hpp"

void CollectionNode::append(ValueNode *element) 
{
    collection.push_back(element);
}


std::string CollectionNode::toString()
{
    std::ostringstream stream;
    stream << "( ";
    for (auto it=collection.begin(); it!=collection.end(); it++) {
        stream << (*it)->toString() << " ";
    }
    stream << ")";
    return stream.str();
}


std::vector<Type*> FunctionArgumentsNode::codeGeneration()
{
    return std::vector<Type*>(collection.size(), Type::getDoubleTy(getGlobalContext()));
}


std::vector<Value*> FunctionParametrsNode::codeGeneration()
{
    std::vector<Value*> params;
    for (auto it=collection.begin(); it != collection.end(); it++) {
        params.push_back( (*it)->codeGeneration() );
    }
    return params;
}


BasicBlock* BlockNode::codeGeneration()
{
    BasicBlock *block = BasicBlock::Create(getGlobalContext(), "", builder->GetInsertBlock()->getParent());

    builder->SetInsertPoint(block);

    for (auto it=collection.begin(); it != collection.end(); it++) {
        (*it)->codeGeneration();
    }

    return block;
}

