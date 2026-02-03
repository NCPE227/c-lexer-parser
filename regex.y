%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int  yylex(void);
%}

%%

System     := Definition* '/' RootRegex '/'
Definition :=  'const' ID '=' '/' Regex '/'
RootRegex  :=  RootRegex '&' RootRegex | '!' Regex | Regex
Regex      :=  Seq | Alt | Repeat | Term | '(' Regex ')'
Seq        :=  Regex+
Alt        :=  Regex '|' Regex
Repeat     :=  Regex'*' | Regex'+'  | Regex'?'
Term       :=  Literal | Range | Wild | Substitute
Literal    :=  '"' escaped unicode '"'
Range      :=  '[' '^'? unicode char ranges ']' // range is C1-C2 & may be escaped
Wild       :=  '.'
Substitute :=  '${' ID '}'
ID         :=  [a-zA-Z0-9_]+

%%