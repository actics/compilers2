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
"*"                 return token::TIMES;    
"/"                 return token::SLASH;    
"("                 return token::LPAREN;   
")"                 return token::RPAREN;   
"="                 return token::EQUALS;
";"                 return token::SEMICOLON;
"def"               return token::DEF;
"extern"            return token::EXTERN;
{char}{nchar}*      {
                        yylval->sval = new std::string(yytext);
                        return token::IDENTIFIER;
                    }
{digit}*\.?{digit}+ {
                        yylval->dval = std::atof(yytext);
                        return token::NUMBER;
                    }
({nl}|{ws})+        /* ignore */
.                   { driver.error("invalid character"); }

%%


