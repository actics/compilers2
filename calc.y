%{
    #include <iostream>
    #include <cstdio>
    int yylex(void);
    void yyerror(char const *);
%}

%defines "./generated/calc_defines.h"

%union {
    double dval;
}

%token PLUS
%token MINUS
%token MULT
%token DIV
%token NL
%token <dval> NUM

%type  <dval> exp

%%

input : /* empty */
      | input line
      ;

line  : NL
      | exp NL { std::cout << "result = " << $1 <<std::endl; }
      ;

exp   : NUM           { $$ = $1 }
      | exp exp PLUS  { $$ = $1 + $2; }
      | exp exp MINUS { $$ = $1 + $2; }
      | exp exp MULT  { $$ = $1 * $2; }
      | exp exp DIV   { $$ = $1 / $2; }
      ;

%%

void yyerror(char const *s) {
    fprintf (stderr, "%s\n", s);
}

int main(void) {
    return yyparse();
}

