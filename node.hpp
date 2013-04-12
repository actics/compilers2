#pragma once

#include <string>
#include <sstream>
#include <stdio.h>

class Node {
public:
    virtual ~Node() = 0;
    virtual std::string toString() = 0;
};


class VarNode: public Node {
private:
    std::string varname;

public:
    ~VarNode();
    VarNode(std::string name);

    std::string toString();
};


class NumNode: public Node {
private:
    double num;

public:
    ~NumNode();
    NumNode(double num);

    std::string toString();
};


class BinNode: public Node {
private:
    Node * left_node;
    Node * rigth_node;
    std::string operation;

public:
    ~BinNode();
    BinNode(Node* lnode, Node* rnode, std::string opr);

    std::string toString();
};

