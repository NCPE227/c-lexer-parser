//
// Created by nperry on 3/11/26.
//
#include <stdlib.h>
#include <stdio.h>

#include "AST.h"

#include <string.h>

AST *ast_root = NULL;

typedef struct StackTop {
    char *name;
    struct StackTop *next;
} StackTop;

// Tracker for the top of the stack
StackTop *stack_top = NULL;

AST *make_binary(int tag, AST *left, AST *right) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Error!\n");
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
        fprintf(stderr, "Error!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.unary.child = child;
    return node;
}

AST *make_string(int tag, char *val) {
    AST *node = (AST *) malloc(sizeof(AST));
    if (node == NULL) { exit(1); }

    node->tag = tag;

    // Safety check: if val is NULL, duplicate an empty string
    node->data.string.value = (val != NULL) ? strdup(val) : strdup("");
    return node;
}

AST *make_def(int tag, char *name, AST *regex) {
    AST *node = (AST *) malloc(sizeof(AST));
    if (node == NULL) { exit(1); }

    node->tag = tag;

    // Safety check: if val is NULL, duplicate an empty string
    node->data.def.name = (name != NULL) ? strdup(name) : strdup("");
    node->data.def.regex = regex;
    return node;
}

AST *top_level(int tag, AST *def_list, AST *root_regex) {
    AST *node = (AST *) malloc(sizeof(AST));

    if (node == NULL) {
        fprintf(stderr, "Error!\n");
        exit(1);
    }

    node->tag = tag;
    node->data.system.def_list = def_list;
    node->data.system.root_regex = root_regex;
    return node;
}

void push_symbol(char *name) {
    StackTop *top = (StackTop *)malloc(sizeof(StackTop));

    if (top == NULL) {
        fprintf(stderr, "Error!\n");
        exit(1);
    }

    top->name = strdup(name);
    top->next = stack_top;
    stack_top = top;
}

int find_symbol(char *name) {
    StackTop *current = stack_top;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1; // found case
        }
        current = current->next;
    }
    return 0; // not found case
}

void validate_escapes(char *str) {
    if (str == NULL) return;

    char *current = str;

    // Keep searching so long as we find another "%x"
    while ((current = strstr(current, "%x")) != NULL) {
        current += 2;

        char *end_pointer;

        // convert string to long int. 16 for hex.
        long l_int = strtol(current, &end_pointer, 16);

        // check if it exceeds the maximum unicode endpoint
        if (l_int > 0x10FFFF) {
            fprintf(stderr, "Semantics Error: Hex escape %%x%lX is out of bounds.\n", l_int);
            exit(1);
        }

        current = end_pointer;
    }
}

void check_semantics(AST *node) {
    if (node == NULL) return;

    switch (node->tag) {
        // Top Level
        case NODE_SYSTEM:
            check_semantics(node->data.system.def_list);
            // Protect against files that only have definitions!
            if (node->data.system.root_regex != NULL) {
                check_semantics(node->data.system.root_regex);
            }
            break;

        // Definitions
        case NODE_DEF:
            // 1. Check the right side (the regex) for any variable usage/hex errors
            check_semantics(node->data.def.regex);
            // 2. Add the left side (the new variable name) to our stack
            push_symbol(node->data.def.name);
            break;

        // Semantic Checks
        case NODE_VAR_REF:
            if (find_symbol(node->data.string.value) == 0) {
                fprintf(stderr, "Semantic Error: Unbound variable '${%s}'\n", node->data.string.value);
                exit(1);
            }
            break;

        // Strings
        case NODE_LITERAL:
        case NODE_RANGE:
            validate_escapes(node->data.string.value);
            break;

        // Recursive tree walking for operations
        case NODE_SEQ:
        case NODE_ALT:
        case NODE_AMP:
            check_semantics(node->data.binary.left);
            check_semantics(node->data.binary.right);
            break;

        // Operations
        case NODE_STAR:
        case NODE_PLUS:
        case NODE_QUESTION:
        case NODE_NOT:
            check_semantics(node->data.unary.child);
            break;

        // Don't do anything
        case NODE_DOT:
            break;

    }
}