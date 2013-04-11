%code requires{
    #include <cstdio>
    #include <cstdarg>
    #include <string>
    #include <queue>
    #include <map>
}

%code { 
    int yylex(void);
    void yyerror(const char * format, ...);
    std::map<std::string, double> vars;
    std::queue<double> operands;

    double opr_func(double (*)(double, double));
    double opr_add(double, double);
    double opr_mul(double, double);

    double get_variable(const std::string);

}

%defines "./generated/calc_defines.h"

%union {
    double dval;
    std::string * sval;
}


%token L_BR
%token R_BR
%token OPR_ADD
%token OPR_SUB
%token OPR_MUL
%token OPR_DIV
%token KEYW_LET
%token KEYW_READ
%token KEYW_WRITE
%token NL
%token <dval> NUM
%token <sval> VAR

%type  <dval> expression
%type  <dval> num_or_expr
%type  <dval> command
%type  <dval> expr1
%type  <dval> expr_ext

%start input


%%


input 
    : 
    | input line
    ;

line  
    : NL 
    | expression NL
    ;

expression  
    : L_BR expr1 R_BR { $$ = $2; }
    ;

expr1  
    : command { $$ = $1; }
    | expr_ext { $$ = $1; }
    ;

expr_ext  
    : expression expr_ext { $$ = $2; }
    | expression { $$ = $1; }
    ;

command  
    : NUM                             { $$ = $1; }
    | VAR                             { $$ = get_variable(*$1); delete $1; }
    | KEYW_READ                       { scanf("%lf", &$$); }
    | KEYW_WRITE expression           { printf("%lf\n", $2); $$ = 0; }
    | KEYW_LET   vardef expr_ext      { $$ = $3; }
    | OPR_ADD    opr_expr             { $$ = opr_func(opr_add); }
    | OPR_MUL    opr_expr             { $$ = opr_func(opr_mul); }
    | OPR_SUB num_or_expr num_or_expr { $$ = $2 - $3; }
    | OPR_DIV num_or_expr num_or_expr { $$ = $2 / $3; }
    ;

vardef  
    : L_BR vardef1 R_BR
    ;

vardef1  
    : VAR num_or_expr
    {
        if ( vars.find(*$1) == vars.end() ) {
            vars[*$1] = $2;
        }
        else {
            yyerror("conflicting declaration «%s»\n", $1->c_str());
        }
        delete $1;
    }
    | vardef_ext
    ;

vardef_ext 
    : vardef vardef_ext 
    | vardef
    ;

opr_expr  
    : num_or_expr opr_expr { operands.push($1); }
    | num_or_expr { operands.push($1); }
    ;

num_or_expr  
    : NUM { $$ = $1; }
    | VAR { $$ = get_variable(*$1); delete $1; }
    | expression { $$ = $1; }
    ;


%%


double opr_func(double (*oper)(double, double)) {
    double target = operands.front();
    operands.pop();
    while ( !operands.empty() ) {
        target = oper(target, operands.front());
        operands.pop();
    }
    return target;
}


double opr_add(double x, double y) {
    return x + y;
}


double opr_mul(double x, double y) {
    return x * y;
}


double get_variable(const std::string var) {
    if ( vars.find(var) == vars.end() ) {
        yyerror("undefined variable «%s»\n", var.c_str());
    }
    else {
        return vars[var];
    }
}


void yyerror(const char * format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}


int main(void) {
    return yyparse();
}

