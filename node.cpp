#include "node.hpp"

Node::~Node() {};


VarNode::~VarNode() {}
VarNode::VarNode(std::string name): varname(name) {}


std::string VarNode::toString() 
{
    return varname;
}


NumNode::~NumNode() {}
NumNode::NumNode(double num): num(num) {}


std::string NumNode::toString()
{
    std::ostringstream stream;
    stream << num;
    return stream.str();
}


BinNode::~BinNode() {
    delete left_node;
    delete rigth_node;
}


BinNode::BinNode(Node* lnode, 
                 Node* rnode, 
                 std::string opr): 
    left_node(lnode),
    rigth_node(rnode),
    operation(opr) 
{
}


std::string BinNode::toString() 
{
    std::ostringstream stream;
    stream << "( " << operation << " " << left_node->toString() 
           << " "  << rigth_node->toString() << " )";
    return stream.str();
}

