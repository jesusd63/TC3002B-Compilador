// make run {filename}

#include <stdio.h>

void main(int argc, char *argv[]){
    // Check if the filename is provided
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
    }

    // Read from file
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Error opening file\n");
        return;
    }
    // Print the contents of the file
    char c;
    while ((c = fgetc(input)) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            printf("%c", c);
        }
    }
    fclose(input);
}