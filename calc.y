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
      | exp PLUS  exp { $$ = $1 + $3; }
      | exp MINUS exp { $$ = $1 + $3; }
      | exp MULT  exp { $$ = $1 * $3; }
      | exp DIV   exp { $$ = $1 / $3; }
      ;

%%

void yyerror(char const *s) {
    fprintf (stderr, "%s\n", s);
}

int main(void) {
    return yyparse();
}

