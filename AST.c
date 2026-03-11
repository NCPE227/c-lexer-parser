//
// Created by nperry on 3/11/26.
//
#include <stdlib.h>
#include <stdio.h>

#include "AST.h"

#include <string.h>

AST *make_binary(int tag, AST *left, AST *right) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Ran out of memory!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

AST *make_unary(int tag, AST *child) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Ran out of memory!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.unary.child = child;
    return node;
}

AST *make_string(int tag, char *val) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Ran out of memory!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.string.value = strdup(val);
    return node;
}

AST *make_def(int tag, char *name, AST *regex) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Ran out of memory!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.def.name = strdup(name);
    node->data.def.regex = regex;
    return node;
}

AST *top_level(int tag, AST *def_list, AST *root_regex) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Ran out of memory!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.system.def_list = def_list;
    node->data.system.root_regex = root_regex;
    return node;
}