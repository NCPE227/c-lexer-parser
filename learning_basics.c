#include <stdio.h>
#include <errno.h>
#include <string.h>

void single_line_file_read(const char* filepath) {
    // Open file in read mode
    FILE *file_pointer;
    file_pointer = fopen(filepath, "r");

    // Trigger error if file not found.
    /*if (file_pointer == NULL) {
        printf("Could not find specified file.\n");
        return;
    }*/
    if (!file_pointer) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    // Store file content in a string
    char string[100]; // string is an array of chars
    fgets(string, 100, file_pointer); // fgets reads content and then stores it in the file

    // Print the content out
    printf("%s", string);
    printf("\n");

    // Close the file we were reading
    fclose(file_pointer);
}

void multiline_file_read(const char* filepath) {
    FILE *fptr;
    fptr = fopen(filepath, "r");

    // Trigger error if file not found.
    /*if (fptr == NULL) {
        printf("Could not find specified file.\n");
        return;
    }*/
    if (!fptr) {
        printf("Error opening file: %s\n", strerror(errno));
        return;
    }

    char string[100];

    // Read the content and print it, while loop lets it iterate over multiple lines
    while (fgets(string, 100, fptr)) {
        printf("%s", string);
    }

    fclose(fptr);
}

void read_file_from_input() {
    FILE *fptr;
    char filepath[100];
    char string[100];

    // While loop keeps asking if a user input fails, otherwise it breaks the loop
    while (1) {
        printf("Enter path to file: ");
        scanf("%s", filepath);

        // Open file using fptr. If it doesn't exist, end interaction with an error.
        fptr = fopen(filepath, "r");
        if (!fptr) {
            printf("Error opening file: %s\n", strerror(errno));
            printf("Please try again.\n");
        } else {
            break;
        }
    }

    // Read the content and print it, while loop lets it iterate over multiple lines
    while (fgets(string, 100, fptr)) {
        printf("%s", string);
    }
    fclose(fptr);
}

int main() {
    //single_line_file_read("/home/nperry/projects/LexerParser/test.txt");
    //multiline_file_read("/home/nperry/projects/LexerParser/multiline_test.txt");
    read_file_from_input();
    return 0;
}