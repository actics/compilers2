#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "./generated/parser.hpp"
#include "node.hpp"

#define YY_DECL                                 \
    yy::Parser::token_type                      \
    yylex (yy::Parser::semantic_type* yylval,   \
           Driver& driver)
YY_DECL;

extern FILE * yyin;
extern int yy_flex_debug;

class Driver
{
public:
    bool trace_scanning = false;
    bool trace_parsing  = false;

    BlockNode *node;
    Module* codeGeneration();

    int parse(FILE *);
    std::string getAstString();
    void deleteAst();
    void error(const std::string& m);
};

