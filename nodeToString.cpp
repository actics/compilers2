#include "node.hpp"

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


std::string ReturnNode::toString()
{
    std::ostringstream stream;
    stream << "( return " << expression->toString() << " )";
    return stream.str();
}


std::string VariableDefinitionNode::toString() 
{
    std::ostringstream stream;
    stream << "( " << varname << " = " << expression->toString() << " )";
    return stream.str();
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


std::string FunctionCallNode::toString()
{
    std::ostringstream stream;
    stream << "( call " << function_name << " " << parametrs->toString() << " )";
    return stream.str();
}


std::string VariableNode::toString() 
{
    return varname;
}


std::string NumberNode::toString()
{
    std::ostringstream stream;
    stream << number;
    return stream.str();
}


std::string ValueNodeCollection::toString()
{
    std::ostringstream stream;
    stream << "( ";
    for (auto it=collection.begin(); it!=collection.end(); it++) {
        stream << (*it)->toString() << " ";
    }
    stream << ")";
    return stream.str();
}

