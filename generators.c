//
// Created by nperry on 4/22/26.
//

#include "generators.h"

#include <string.h>

// Helper function to forward declare match functions so they can call each other
void forward_declare_defs(AST *node, FILE *out) {
    if (node == NULL) return;
    if (node->tag == NODE_SEQ) {
        forward_declare_defs(node->data.binary.left, out);
        forward_declare_defs(node->data.binary.right, out);
    } else if (node->tag == NODE_DEF) {
        fprintf(out, "int match_%s(const char *input, int pos);\n", node->data.def.name);
    }
}

// Generate our includes and start traversal
void generate_system(AST *node, FILE *out) {
    if (node == NULL || node->tag != NODE_SYSTEM) return;

    // C headers
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <string.h>\n");
    fprintf(out, "#include <stdlib.h>\n\n");

    // Matching functions for each const definition
    fprintf(out, "\n// Regex Definitions\n");
    generate_definitions(node->data.system.def_list, out);

    // Main RootRegex function
    fprintf(out, "\n// Root Regex Matcher\n");
    fprintf(out, "int match_root(const char *input, int pos) {\n");
    int count = 0;
    generate_regex(node->data.system.root_regex, out, &count);
    fprintf(out, "\treturn pos;\n");
    fprintf(out, "}\n\n");

    //main() function for rexec.c
    fprintf(out, "int main(int argc, char **argv) {\n");
    fprintf(out, "    if (argc < 2) { return 1; }\n");
    fprintf(out, "    FILE *f = fopen(argv[1], \"r\");\n");
    fprintf(out, "    if (!f) { return 1; }\n");
    fprintf(out, "    fseek(f, 0, SEEK_END);\n");
    fprintf(out, "    long length = ftell(f);\n");
    fprintf(out, "    fseek(f, 0, SEEK_SET);\n");
    fprintf(out, "    char *buffer = malloc(length + 1);\n");
    fprintf(out, "    fread(buffer, 1, length, f);\n");
    fprintf(out, "    buffer[length] = '\\0';\n");
    fprintf(out, "    fclose(f);\n\n");
    fprintf(out, "    if (match_root(buffer, 0) != -1) { printf(\"ACCEPTS\\n\"); }\n");
    fprintf(out, "    else { printf(\"REJECTS\\n\"); }\n");
    fprintf(out, "    free(buffer);\n");
    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n");
}

// Walk definition list and create a C function for each one
void generate_definitions(AST *node, FILE *out) {
    if (node == NULL) return;

    if (node->tag == NODE_SEQ) {
        generate_definitions(node->data.binary.left, out);
        generate_definitions(node->data.binary.right, out);
    }
    else if (node->tag == NODE_DEF) {
        // Create a function signature for this variable
        fprintf(out, "int match_%s(const char *input, int pos) {\n", node->data.def.name);

        int id_counter = 0;
        // Generate the internal logic for the regex
        generate_regex(node->data.def.regex, out, &id_counter);

        // If it makes it to the end without returning -1, it's a match
        fprintf(out, "    return pos;\n");
        fprintf(out, "}\n\n");
    }
}

// Generates the actual matching logic (the body of the matching functions)
void generate_regex(AST *node, FILE *out, int *id_counter) {
    if (node == NULL) return;

    switch (node->tag) {
        case NODE_LITERAL: {
            char *val = node->data.string.value;
            int len = strlen(val) - 2; // Subtract 2 for the quotes

            fprintf(out, "    // Matching Literal %s\n", val);
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        if (strncmp(input + pos, %s, %d) == 0) { pos += %d; }\n", val, len, len); // removed \" from %s as it already has it in the generated code
            fprintf(out, "        else { pos = -1; }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_VAR_REF: {
            fprintf(out, "    // Calling variable ${%s}\n", node->data.string.value);
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        pos = match_%s(input, pos);\n", node->data.string.value);
            fprintf(out, "    }\n");
            break;
        }


        case NODE_ALT: {
            int cid = (*id_counter)++;
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        int saved_pos_%d = pos;\n", cid);

            // Try Left Side
            generate_regex(node->data.binary.left, out, id_counter);

            // If Left Side Failed, Try Right Side
            fprintf(out, "        if (pos == -1) {\n");
            fprintf(out, "            pos = saved_pos_%d;\n", cid);
            generate_regex(node->data.binary.right, out, id_counter);
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_SEQ: {
            generate_regex(node->data.binary.left, out, id_counter);
            generate_regex(node->data.binary.right, out, id_counter);
            break;
        }

        case NODE_STAR: {
            int cid = (*id_counter)++;
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        while (1) {\n");
            fprintf(out, "            int saved_pos_%d = pos;\n", cid);

            generate_regex(node->data.unary.child, out, id_counter);

            fprintf(out, "            if (pos == -1) {\n");
            fprintf(out, "                pos = saved_pos_%d;\n", cid); // Restore pos to last successful match
            fprintf(out, "                break;\n"); // Exit greedy loop
            fprintf(out, "            }\n");
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_PLUS: {
            int cid = (*id_counter)++;
            // Match at least once (A+)
            generate_regex(node->data.unary.child, out, id_counter);

            // Then loop for the rest (A*)
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        while (1) {\n");
            fprintf(out, "            int saved_pos_%d = pos;\n", cid);

            generate_regex(node->data.unary.child, out, id_counter);

            fprintf(out, "            if (pos == -1) {\n");
            fprintf(out, "                pos = saved_pos_%d;\n", cid);
            fprintf(out, "                break;\n");
            fprintf(out, "            }\n");
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_QUESTION: {
            int cid = (*id_counter)++;
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        int saved_pos_%d = pos;\n", cid);

            generate_regex(node->data.unary.child, out, id_counter);

            fprintf(out, "        if (pos == -1) {\n");
            fprintf(out, "            pos = saved_pos_%d; // Ignore failure (0 matches is fine)\n", cid);
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_DOT: {
            fprintf(out, "    if (pos != -1) {\n");
            // Match anything except end of string
            fprintf(out, "        if (input[pos] != '\\0' && input[pos] != '\\n') { pos += 1; }\n");
            fprintf(out, "        else { pos = -1; }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_RANGE: {
            char *val = node->data.string.value; // e.g., "[a-z]"
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        char c = input[pos];\n");
            fprintf(out, "        int match_range = 0;\n");
            fprintf(out, "        if (c != '\\0') {\n");

            // Checking if it's explicitly a formatted range [x-y]
            if (strlen(val) == 5 && val[2] == '-') {
                fprintf(out, "            if (c >= '%c' && c <= '%c') match_range = 1;\n", val[1], val[3]);
            } else {
                // Fallback for character classes like "[aeiou]"
                fprintf(out, "            if (strchr(\"%s\", c) != NULL && c != '[' && c != ']') match_range = 1;\n", val);
            }

            fprintf(out, "        }\n");
            fprintf(out, "        if (match_range) { pos += 1; }\n");
            fprintf(out, "        else { pos = -1; }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_NOT: {
            int cid = (*id_counter)++;
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        int saved_pos_%d = pos;\n", cid);

            generate_regex(node->data.unary.child, out, id_counter);

            fprintf(out, "        if (pos == -1) {\n");
            fprintf(out, "            pos = saved_pos_%d; // NOT condition fulfilled\n", cid);
            fprintf(out, "        } else {\n");
            fprintf(out, "            pos = -1; // NOT condition failed (it matched)\n");
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }

        case NODE_AMP: {
            int cid = (*id_counter)++;
            fprintf(out, "    if (pos != -1) {\n");
            fprintf(out, "        int saved_pos_%d = pos;\n", cid);

            // Check left side
            generate_regex(node->data.binary.left, out, id_counter);

            fprintf(out, "        int end_pos_%d = pos;\n", cid);
            fprintf(out, "        if (pos != -1) {\n");
            fprintf(out, "            pos = saved_pos_%d; // Reset to check right side\n", cid);

            // Check right side
            generate_regex(node->data.binary.right, out, id_counter);

            fprintf(out, "            if (pos != -1 && pos != end_pos_%d) {\n", cid);
            fprintf(out, "                pos = -1; // Intersection must match same length\n");
            fprintf(out, "            }\n");
            fprintf(out, "        }\n");
            fprintf(out, "    }\n");
            break;
        }
    }
}
