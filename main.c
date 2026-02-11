#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// From Bison
int yyparse(void);

// From Flex
extern FILE *yyin;

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    }

    // Open the input file given by user if it exists
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("fopen");
        return 1;
    }

    // Run the parser
    int result = yyparse();

    // Check the result. Print "accepts" if its regex, or outputs errors if it isn't regex.
    if (result == 0) {
        printf("accepts\n");
        return 0;
    }
    else {
        return 1;
    }
}