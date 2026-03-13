%{
#include <string.h> // will this inclusion fix segfaults (139)?
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

\"[^\"]*\" {
    yylval.str = malloc(strlen(yytext) + 1);
    strcpy(yylval.str, yytext);
    return LITERAL;
}

"["[^\]]*"]" {
    yylval.str = malloc(strlen(yytext) + 1);
    strcpy(yylval.str, yytext);
    return RANGE;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = malloc(strlen(yytext) + 1);
    strcpy(yylval.str, yytext);
    return ID;
}

[ \t\r\n]+     ; // skip extra whitespace

. {
    fprintf(stderr, "Unexpected character: %s\n", yytext);
    return 0;
}
%%
int yywrap(void) {return 1;}