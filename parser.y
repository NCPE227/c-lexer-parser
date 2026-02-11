%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
%}

/* Tokens */
%token CONST ID LITERAL RANGE
%token SLASH AMP NOT OR EQUALS
%token STAR PLUS QUESTION
%token LPAREN RPAREN
%token DOT
%token SUBSTART LBRACE RBRACE

%%

System :
      DefinitionList SLASH RootRegex SLASH
      | DefinitionList // accounts for the case of having only a declaration
    ;

DefinitionList :
      /* empty */
    | DefinitionList Definition
    ;

Definition :
      CONST ID EQUALS SLASH Regex SLASH
    ;

/* Boolean and Top Level Operations */
RootRegex :
      RootRegex AMP RootRegex
    | NOT Regex
    | Alt
    ;

/* Lowest regex precedence */
Regex :
      Seq
    | Alt
    | Repeat
    | Term
    | LPAREN Regex RPAREN
    ;

Alt :
      Seq
      | Alt OR Seq
    ;

Seq :
      Repeat
    | Seq Repeat
    ;

Repeat :
      Term STAR
    | Term PLUS
    | Term QUESTION
    | Term
    ;

Term :
      LITERAL
    | RANGE
    | DOT
    | Substitute
    | LPAREN RootRegex RPAREN
    ;

Substitute :
      SUBSTART ID RBRACE // recall that substart is the combination character of ${
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
