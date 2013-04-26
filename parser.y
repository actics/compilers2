%skeleton "lalr1.cc"   
%require "2.5"
%defines
%define parser_class_name "Parser"

%code requires {
    #include <string>
    class Driver;
    class Node;
    class BlockNode;
    class FunctionArgumentsNode;
    class FunctionParametrsNode;
    class ValueNode;
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
    double                    double_value;
    std::string               *string_value;
    ValueNode                 *value_node;
    BlockNode                 *block_node;
    FunctionArgumentsNode *arguments_node;
    FunctionParametrsNode      *parametrs_node;
}

%token END 0 "end of file"
%token PLUS
%token MINUS
%token MULTIPLICATION
%token DIVISION
%token LEFT_PARENTHESIS
%token RIGTH_PARENTHESIS
%token ASSIGN
%token SEMICOLON
%token COMMA
%token LEFT_CURLY_BRACE
%token RIGTH_CURLY_BRACE
%token GT
%token LS
%token GTE
%token LSE
%token EQ
%token NEQ
%token IF
%token ELSE
%token RETURN
%token CREATE_FUNCTION

%token <string_value> IDENTIFIER
%token <double_value> NUMBER
%type <block_node> block
%type <block_node> actions
%type <parametrs_node> call_arguments
%type <parametrs_node> call_arguments_eps
%type <arguments_node> definition_arguments
%type <arguments_node> definition_arguments_eps
%type <value_node> action
%type <value_node> function_call
%type <value_node> if_expression
%type <function_node> create_function
%type <value_node> variable_definition
%type <value_node> return_statement
%type <block_node> else_expression
%type <value_node> logical_expression
%type <value_node> expression

%start axiom


%%


axiom
    : block { driver.nodes.push_back($1); }
    ;

block
    : LEFT_CURLY_BRACE actions RIGTH_CURLY_BRACE { $$ = $2; }
    ;

actions
    : actions action { $1->append($2); $$ = $1; }
    | /* epsilon */  { $$ = new BlockNode(); }
    ;

action
    : function_call
    | if_expression
/*    | create_function */
    | variable_definition
    | return_statement
    ;

function_call
    : IDENTIFIER LEFT_PARENTHESIS call_arguments_eps RIGTH_PARENTHESIS { $$ = new FunctionCallNode(*$1, $3); }
    ;

call_arguments_eps
    : call_arguments { $$ = $1; }
    | /* epsilon */  { $$ = new FunctionParametrsNode(); }
    ;

call_arguments
    : call_arguments COMMA expression { $1->append($3); }
    | expression { $$ = new FunctionParametrsNode(); $$->append($1); }
    ;

if_expression
    : IF LEFT_PARENTHESIS logical_expression RIGTH_PARENTHESIS block else_expression { $$ = new IfNode($3, $5, $6); }
    ;

else_expression
    : ELSE block { $$ = $2; }
    | /* epsilon */ { $$ = new BlockNode(); }
    ;

logical_expression
    : expression  GT   expression  { $$ = new BinaryNode(">", $1, $3); }
    | expression  LS   expression  { $$ = new BinaryNode("<", $1, $3); }
    | expression  GTE  expression  { $$ = new BinaryNode(">=", $1, $3); }
    | expression  LSE  expression  { $$ = new BinaryNode("<=", $1, $3); }
    | expression  EQ   expression  { $$ = new BinaryNode("==", $1, $3); }
    | expression  NEQ  expression  { $$ = new BinaryNode("!=", $1, $3); }
    ;

create_function
    : CREATE_FUNCTION IDENTIFIER LEFT_PARENTHESIS definition_arguments_eps RIGTH_PARENTHESIS block { $$ = new CreateFunctionNode(*$2, $4, $6); }
    ;

definition_arguments_eps
    : definition_arguments { $$ = $1; }
    | /* epsilon */ { $$ = new DefinitionArgumentsNode(); }
    ;

definition_arguments
    : definition_arguments COMMA IDENTIFIER { $1->append($3); $$ = $1; }
    | IDENTIFIER { $$ = new DefinitionArgumentsNode($1); }
    ;

variable_definition
    : IDENTIFIER ASSIGN expression { $$ = new VariableDefinitionNode(*$1, $3); }
    ;

return_statement
    : RETURN expression { $$ = new ReturnNode($2); }
    ;

%left PLUS MINUS;
%left MULTIPLICATION DIVISION;

expression
    : LEFT_PARENTHESIS expression RIGTH_PARENTHESIS { $$ = $2; }
    | expression PLUS expression { $$ = new BinaryNode("", $1, $3); }
    | expression MINUS expression { $$ = new BinaryNode("", $1, $3); }
    | expression MULTIPLICATION expression { $$ = new BinaryNode("", $1, $3); }
    | expression DIVISION expression { $$ = new BinaryNode("", $1, $3); }
    | function_call { $$ = $1; }
    | IDENTIFIER { $$ = new VariableNode(*$1); }
    | NUMBER { $$ = new NumberNode($1); }
    ;


%%


void yy::Parser::error(const yy::location& l, const std::string& m)
{
    driver.error(m);
}

