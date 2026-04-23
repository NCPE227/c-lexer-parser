//
// Created by nperry on 3/11/26.
//

#ifndef LEXERPARSER_AST_H
#define LEXERPARSER_AST_H

typedef struct AST AST;

struct AST {
    enum {
        // Top Level Structure
        NODE_SYSTEM, // top of tree
        NODE_DEF, // represents const ID = /regex/ definition

        // Boolean Ops (Top Level Ops)
        NODE_AMP, // regex & regex
        NODE_NOT, // !regex

        // Regex Ops
        NODE_ALT, // regex | regex
        NODE_SEQ, // regex regex (concat)
        NODE_STAR, // regex*
        NODE_PLUS, // regex+
        NODE_QUESTION, // regex?

        // Terminals and Leaves
        NODE_LITERAL, // "a" and/or "abc"
        NODE_RANGE, // ["a"-"z"]
        NODE_DOT, // .
        NODE_VAR_REF // substitution for ${ID}
    } tag;

    union {
        // Operations
        struct {AST *left; AST *right;} binary;
        struct {AST *child;} unary;

        // Terminals and Leaves (literal, range, and var_ref)
        struct {char *value;} string;

        // Definition
        struct {char *name; AST *regex;} def;

        // Top Level System (list of defs and the final root regex)
        struct {AST *def_list; AST *root_regex;} system;
    } data;
};

// Function Prototypes
AST *make_binary(int tag, AST *left, AST *right);
AST *make_unary(int tag, AST *child);
AST *make_string(int tag, char *val);
AST *make_def(int tag, char *name, AST *regex);
AST *top_level(int tag, AST *def_list, AST *root_regex);

void check_semantics(AST *node);
void collect_definitions(AST *node);
int contains_var(AST *node, char *name);

extern AST *ast_root; // Tells everyone this variable exists somewhere!

#endif //LEXERPARSER_AST_H