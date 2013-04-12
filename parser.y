%skeleton "lalr1.cc"   
%require "2.5"
%defines
%define parser_class_name "Parser"

%code requires {
    #include <string>
    class Driver;
    class Node;
}

%parse-param { Driver& driver }
%lex-param   { Driver& driver }

%debug
%error-verbose

%code {
    #include "../driver.hpp"
    #include "../node.hpp"
}

%union {
    double dval;
    std::string* sval;
    Node * nval;
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
%token <dval> NUMBER     "number"

%type  <nval> expr

%start unit


%%


unit
    : assignments expr  { driver.nodes.push_back($2); }
    ;

assignments 
    : assignments assignment {}
    | /* Nothing.  */        {}
    ;

assignment
    : IDENTIFIER EQUALS expr 
    { 
        Node* varnode = new VarNode(*$1);
        Node* binnode = new BinNode(varnode, $3, "=");
        driver.nodes.push_back(binnode); 
        delete $1; 
    }
    ;

%left PLUS MINUS;
%left TIMES SLASH;

expr 
    : expr PLUS  expr   { $$ = new BinNode($1, $3, "+"); }
    | expr MINUS expr   { $$ = new BinNode($1, $3, "-"); }
    | expr TIMES expr   { $$ = new BinNode($1, $3, "*"); }
    | expr SLASH expr   { $$ = new BinNode($1, $3, "/"); }
    | IDENTIFIER        { $$ = new VarNode(*$1); delete $1; }
    | NUMBER            { $$ = new NumNode($1); }
    ;


%%


void yy::Parser::error(const yy::location& l, const std::string& m)
{
    driver.error(m);
}

