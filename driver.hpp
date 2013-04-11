#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <map>
#include "parser.hpp"

#define YY_DECL                                 \
    yy::Parser::token_type                      \
    yylex (yy::Parser::semantic_type* yylval,   \
           Driver& driver)
YY_DECL;

class Driver
{
public:
    Driver();
    virtual ~Driver();

    std::map<std::string, double> variables;
    double result;

    void scan_begin();
    void scan_end();
    bool trace_scanning;

    int parse(const std::string& filename);
    std::string filename;
    bool trace_parsing;

    void error(const std::string& m);
};
#endif
