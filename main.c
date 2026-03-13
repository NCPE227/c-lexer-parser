#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
#include "parser.tab.h"

extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
    }

    // CRITICAL: Exit immediately if a syntax error occurs!
    if (yyparse() != 0) {
        return 1;
    }

    // If we survived parsing, check semantics
    if (ast_root != NULL) {
        check_semantics(ast_root);
    }

    printf("accepts\n");
    return 0;
}