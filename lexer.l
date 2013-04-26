%{
#include "parser.hpp"
#include "../driver.hpp"

typedef yy::Parser::token token;

#define yyterminate() return token::END
%}

%option noyywrap
%option debug

digit [0-9]
char  [a-zA-Z_]
nchar [a-zA-Z_0-9]
ws    [ \t]
nl    [\r\n]

%%

"+"                 return token::PLUS;     
"-"                 return token::MINUS;    
"*"                 return token::MULTIPLICATION;    
"/"                 return token::DIVISION;
"("                 return token::LEFT_PARENTHESIS;   
")"                 return token::RIGTH_PARENTHESIS;   
"{"                 return token::LEFT_CURLY_BRACE;   
"}"                 return token::RIGTH_CURLY_BRACE;
"="                 return token::ASSIGN;
";"                 return token::SEMICOLON;
">"                 return token::GT;
"<"                 return token::LS;
">="                return token::GTE;
"<="                return token::LSE;
"=="                return token::EQ;
"!="                return token::NEQ;
"return"            return token::RETURN;
"if"                return token::IF;
"else"              return token::ELSE;
"create"            return token::CREATE_FUNCTION;
","                 return token::COMMA;
{char}{nchar}*      {
                        yylval->string_value = new std::string(yytext);
                        return token::IDENTIFIER;
                    }
{digit}*\.?{digit}+ {
                        yylval->double_value = std::atof(yytext);
                        return token::NUMBER;
                    }
({nl}|{ws})+        /* ignore */
.                   { driver.error("invalid character"); }

%%


