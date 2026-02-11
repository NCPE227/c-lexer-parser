%{
#include "parser.tab.h"
%}

%%
"const"     {return CONST;}
"//"        {int c; while ((c=input()) !='\n' && c!=EOF);} // handle comments: skip until newline

"="         {return EQUALS;}
"*"         {return STAR;}
"+"         {return PLUS;}
"?"         {return QUESTION;}
"!"         {return NOT;}
"|"         {return OR;}
"&"         {return AMP;}
"/"         {return SLASH;}

"("         {return LPAREN;}
")"         {return RPAREN;}

"${"        {return SUBSTART;}
"{"         {return LBRACE;}
"}"         {return RBRACE;}

"."         {return DOT;}

\"([^\"%]|%x[0-9]+;)*\" { return LITERAL; } // defines a literal as any characters or escapes bounded by double quotations
"["[^\]]*"]"        { return RANGE; } // defines range as escaped chars and ^ bounded by brackets
[a-zA-Z_][a-zA-Z0-9_]*  { return ID; }

[ \t\r\n]+     ; // skip extra whitespace

. {
    fprintf(stderr, "Unexpected character: %s\n", yytext);
    return 0;
}

%%
int yywrap(void) {return 1;}