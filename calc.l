%{
    #include <cstdlib>
    #include <cstdio>
    #include "calc_defines.h"
%}

%option noyywrap
/* %option debug */

digit [0-9]
char  [a-zA-Z_]
nchar [a-zA-Z_0-9]
nl    [\r\n]

%%

"("     return L_BR;
")"     return R_BR;
"+"     return OPR_ADD;
"-"     return OPR_SUB;
"*"     return OPR_MUL;
"/"     return OPR_DIV;
"let"   return KEYW_LET;
"read"  return KEYW_READ;
"write" return KEYW_WRITE;
{nl}    return NL; 
{digit}*\.?{digit}+ {
                       yylval.dval = std::atof(yytext);
                       return NUM;
                    }
{char}{nchar}*      {
                       yylval.sval = new std::string(yytext);
                       return VAR;
                    }
.    /* ignore */

