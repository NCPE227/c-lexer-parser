//
// Created by nperry on 4/22/26.
//

#ifndef LEXERPARSER_GENERATORS_H
#define LEXERPARSER_GENERATORS_H
#include <stdio.h>
#include "AST.h"

// Prototypes for the C code generator
void generate_system(AST *node, FILE *out);
void generate_definitions(AST *node, FILE *out);
void generate_regex(AST *node, FILE *out, int *id_counter);

#endif //LEXERPARSER_GENERATORS_H