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


#endif //LEXERPARSER_AST_H