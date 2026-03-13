%{
#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *str;
    struct AST *node;
}

/* Tokens */
%token <str> ID LITERAL RANGE
%token CONST SLASH AMP NOT OR EQUALS
%left AMP
%token STAR PLUS QUESTION
%token LPAREN RPAREN
%token DOT
%token SUBSTART LBRACE RBRACE

/* Non-terminals that return AST nodes */
%type <node> System DefinitionList Definition
%type <node> RootRegex Regex Alt Seq Repeat Term Substitute

%%

System :
      DefinitionList SLASH RootRegex SLASH {
          // $1 is DefinitionList, $3 is RootRegex
          $$ = top_level(NODE_SYSTEM, $1, $3);
          ast_root = $$; // Save the finished tree to a global var
      }
    ;

DefinitionList :
      /* empty */ {
          // An empty list is just a NULL pointer
          $$ = NULL;
      }
    | DefinitionList Definition {
          // Chain them together. Reuse make_binary with NODE_SEQ
          $$ = make_binary(NODE_SEQ, $1, $2);
      }
    ;

Definition :
      CONST ID EQUALS SLASH Regex SLASH {
          // $2 is the ID (char*), $5 is the Regex (AST*)
          $$ = make_def(NODE_DEF, $2, $5);
      }
    ;

/* Boolean and Top Level Operations */
RootRegex :
      RootRegex AMP RootRegex { $$ = make_binary(NODE_AMP, $1, $3); }
    | NOT Regex               { $$ = make_unary(NODE_NOT, $2); }
    | Alt                     { $$ = $1; }
    ;


Regex :
    Alt       { $$ = $1; }
    ;

Alt :
      Seq         { $$ = $1; }
    | Alt OR Seq  { $$ = make_binary(NODE_ALT, $1, $3); }
    ;

Seq :
      Repeat      { $$ = $1; }
    | Seq Repeat  { $$ = make_binary(NODE_SEQ, $1, $2); }
    ;

Repeat :
      Repeat STAR       {$$ = make_unary(NODE_STAR, $1); }
    | Repeat PLUS       { $$ = make_unary(NODE_PLUS, $1); }
    | Repeat QUESTION   { $$ = make_unary(NODE_QUESTION, $1); }
    | Term              { $$ = $1; }
    ;

Term :
      LITERAL                 { $$ = make_string(NODE_LITERAL, $1); }
    | RANGE                   { $$ = make_string(NODE_RANGE, $1); }
    | DOT                     { $$ = make_unary(NODE_DOT, NULL); }
    | Substitute              { $$ = $1; }
    | LPAREN RootRegex RPAREN { $$ = $2; }
    ;

Substitute :
      SUBSTART ID RBRACE { $$ = make_string(NODE_VAR_REF, $2); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
