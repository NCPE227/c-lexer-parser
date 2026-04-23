#include <stdio.h>
#include <stdlib.h>
#include "AST.h"
#include "parser.tab.h"
#include "generators.h"

extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
    }

    // Exit immediately if a syntax error occurs
    if (yyparse() != 0) {
        return 1;
    }

    // If we survived parsing, check semantics
    if (ast_root != NULL) {
        // splitting semantic check into two groups, check push_symbol and NODE_DEF first, then validate VAR_REFs and escapes
        collect_definitions(ast_root);
        check_semantics(ast_root);

        // Open the output file
        FILE *out = fopen("rexec.c", "w");
        if (!out) { perror("rexec.c"); return 1; }

        // Use fancy new generator system to build the file
        generate_system(ast_root, out);

        fclose(out);
    }



    //printf("accepts\n");
    return 0;
}