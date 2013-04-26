#include "binaryNode.hpp"

BinaryNode::~BinaryNode() {
    delete left_node;
    delete rigth_node;
}


std::string BinaryNode::toString() 
{
    std::ostringstream stream;
    stream << "( " 
           << left_node->toString()  << " "
           << operation              << " "
           << rigth_node->toString() << " "
           << ")";
    return stream.str();
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
}

