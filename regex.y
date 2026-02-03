%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
%}

/* Tokens */
%token CONST ID
%token SLASH AMP NOT OR
%token STAR PLUS QUESTION
%token LPAREN RPAREN
%token DOT
%token DOLLAR LBRACE RBRACE
%token CHAR

%%

System :
      DefinitionList SLASH RootRegex SLASH
    ;

DefinitionList :
      /* empty */
    | DefinitionList Definition
    ;

Definition :
      CONST ID '=' SLASH Regex SLASH
    ;

RootRegex :
      RootRegex AMP RootRegex
    | NOT Regex
    | Regex
    ;

Regex :
      Seq
    | Alt
    | Repeat
    | Term
    | LPAREN Regex RPAREN
    ;

Seq :
      Regex
    | Seq Regex
    ;

Alt :
      Regex OR Regex
    ;

Repeat :
      Regex STAR
    | Regex PLUS
    | Regex QUESTION
    ;

Term :
      CHAR
    | DOT
    | Substitute
    ;

Substitute :
      DOLLAR LBRACE ID RBRACE
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
