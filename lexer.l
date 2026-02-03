%{
#include "parser.tab.h"
%}

%%
"*"         {return STAR;}
"+"         {return PLUS;}
"?"         {return QUESTION;}
"("         {return LPAREN;}
")"         {return RPAREN;}
"|"         {return OR;}
"&"         {return AND;}
[a-zA-Z0-9] {yylval = yytext[0}; return CHAR;}
[ \t\n]     ; // ignore whitespace
.           {fprintf(stderr, "Unpexpected character: %s\n", yytext); return ERROR;}

%%