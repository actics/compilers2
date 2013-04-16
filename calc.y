%code requires{
    #include <cstdarg>
    #include <cstdlib>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <map>
    #include <vector>
}

%code { 
    int yylex(void);
    void yyerror(const char * format, ...);

    std::map<std::string, std::string> vars;

    void check_var(std::string);
    void add_var(std::string, std::string);

    std::ostringstream code_head;
    int let_count = 0;
    int expr_count = 0;

}

%defines "./generated/calc_defines.h"

%union {
    std::string * sval;
    std::vector<std::string*> * vval;
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
%token <sval> NUM
%token <sval> VAR

%type  <sval> expression
%type  <vval> opr_expr
%type  <sval> num_or_expr
%type  <sval> command
%type  <sval> expr1
%type  <sval> expressions

%start input


%%


input 
    : 
    | input line
    ;

line  
    : NL 
    | expression NL 
    { 
        std::cout << "#include <stdio.h>\n";
        std::cout << "double calc_read() {\n";
        std::cout << "    double x;\n";
        std::cout << "    scanf(\"%lf\", &x);\n";
        std::cout << "    return x;\n";
        std::cout << "}\n";
        std::cout << "double calc_write(double x) {\n";
        std::cout << "    printf(\"%lf\\n\", x);\n";
        std::cout << "    return 0;\n";
        std::cout << "}\n";
        std::cout << code_head.str();
        std::cout << "int main(void) {\n";
        for (auto it=vars.begin(); it != vars.end(); it++) {
            std::cout << it->first << "=" << it->second <<";\n";
        }
        std::cout << *$1 << ";\n";
        std::cout << "}\n";
        code_head.clear();
        vars.clear();
        let_count = 0;
        expr_count = 0;
        code_head.str( std::string() );
    }
    ;

expression  
    : L_BR expr1 R_BR { $$ = $2; }
    ;

expr1  
    : command 
    { 
        std::ostringstream stream;
        stream << "(" << *$1 << ")"; 
        $$ = new std::string(stream.str());
        delete $1;
    }
    | expressions 
    { 
        std::ostringstream expr_func;
        expr_func << "__expr_block_" << expr_count << "__()";
        $$ = new std::string(expr_func.str());
        code_head << "double " << *$$ << "{\n";
        code_head << *$1 << "}\n";
        expr_count++;
        delete $1;
    }
    ;

expressions  
    : expression 
    { 
        $$ = new std::string();
        $$->append("return ");
        $$->append(*$1);
        $$->append(";\n");
        delete $1;
    }
    | expression expressions 
    { 
        $1->append(";\n");
        $1->append(*$2);
        $$ = $1;
        delete $2;
    }
    ;

command  
    : NUM    { $$ = $1; }
    | VAR    { check_var(*$1); $$ = $1; }
    | KEYW_READ 
    { 
        $$ = new std::string("calc_read()"); 
    }
    | KEYW_WRITE expression 
    { 
        $$ = new std::string("calc_write(" + *$2 + ")"); 
        delete $2; 
    }
    | KEYW_LET   vardef_expr expressions
    { 
        std::ostringstream let_func;
        let_func << "__let_block_" << let_count << "__()";
        $$ = new std::string(let_func.str());
        code_head << "double " << *$$ << "{\n";
        code_head << *$3 << "}\n";
        let_count++;
        delete $3;
    }
    | OPR_ADD    opr_expr 
    {
        std::ostringstream stream;
        stream << *(*($2->begin()));
        for (auto it=$2->begin()+1; it != $2->end(); it++)  {
            stream << "+";
            stream << **it;
            delete *it;
        }
        delete $2;
        $$ = new std::string(stream.str());
    }
    | OPR_MUL    opr_expr
    {
        std::ostringstream stream;
        stream << *(*($2->begin()));
        for (auto it=$2->begin()+1; it != $2->end(); it++)  {
            stream << "*";
            stream << **it;
            delete *it;
        }
        delete $2;
        $$ = new std::string(stream.str());
    }
    | OPR_SUB num_or_expr num_or_expr 
    {
        auto str = *$2 + "-" + *$3;
        $$ = new std::string(str);
        delete $2;
        delete $3;
    }
    | OPR_DIV num_or_expr num_or_expr 
    {
        auto str = *$2 + "/" + *$3;
        $$ = new std::string(str);
        delete $2;
        delete $3;
    }
    ;

vardef_expr  
    : vardef 
    | L_BR vardefs R_BR 
    ;

vardefs 
    : vardef 
    | vardef vardefs
    ;

vardef  
    : L_BR VAR num_or_expr R_BR
    {
        add_var(*$2, *$3);
        code_head << "double " << *$2 << ";\n";
        delete $2;
        delete $3;
    }
    ;

opr_expr  
    : num_or_expr opr_expr 
    { 
        $2->push_back($1); 
        $$ = $2; 
    }
    | num_or_expr 
    { 
        $$ = new std::vector<std::string*>; 
        $$->push_back($1);
    }
    ;

num_or_expr  
    : NUM        { $$ = $1; }
    | VAR        { check_var(*$1); $$ = $1; }
    | expression { $$ = $1; }
    ;


%%


void check_var(std::string var) 
{
    if (vars.find(var) == vars.end()) {
        yyerror("undefined variable «%s»\n", var.c_str());
    }
}


void add_var(std::string var, std::string value)
{
    if (vars.find(var) != vars.end()) {
        yyerror("conflicting declaration «%s»\n", var.c_str());
    }
    vars[var] = value;
}


void yyerror(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}


int main(void) 
{
    return yyparse();
}

