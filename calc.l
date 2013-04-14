%{
    #include <cstdlib>
    #include "calc_defines.h"
%}


%option noyywrap

digit [0-9]
nl    [\r\n]

%%

"+"  return PLUS;
"-"  return MINUS;
"*"  return MULT;
"/"  return DIV;
"("  return L_BR;
")"  return R_BR;
{nl} return NL;
{digit}*\.?{digit}+ {
                       yylval.dval = std::atof(yytext);
                       return NUM;
                    }
.                   /* ignore */

