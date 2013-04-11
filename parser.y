%skeleton "lalr1.cc"   
%require "2.5"
%defines
%define parser_class_name "Parser"

%code requires {
#include <string>
class Driver;
}

%parse-param { Driver& driver }
%lex-param   { Driver& driver }

%debug
%error-verbose

%union {
    double dval;
    std::string* sval;
}

%code {
#include "driver.hpp"
}

%token END 0 "end of file"
%token PLUS
%token MINUS
%token TIMES
%token SLASH
%token LPAREN
%token RPAREN
%token EQUALS
%token SEMICOLON
%token DEF
%token EXTERN
%token <sval> IDENTIFIER "identifier"
%token <dval> NUMBER "number"
%type <dval> expr

%%
%start unit;
unit    : assignments expr  { driver.result = $2; };

assignments : assignments assignment {}
            | /* Nothing.  */        {};

assignment  : IDENTIFIER EQUALS expr
            { driver.variables[*$1] = $3; delete $1; };

%left PLUS MINUS;
%left TIMES SLASH;
expr    : expr PLUS expr    { $$ = $1 + $3; }
        | expr MINUS expr   { $$ = $1 - $3; }
        | expr TIMES expr   { $$ = $1 * $3; }
        | expr SLASH expr   { $$ = $1 / $3; }
        | IDENTIFIER  { $$ = driver.variables[*$1]; delete $1; }
        | NUMBER      { $$ = $1; };
%%

void yy::Parser::error(const yy::location& l, const std::string& m)
{
    driver.error(m);
}
