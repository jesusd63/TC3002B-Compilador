// make run

#include <stdio.h>

void main(){
    // Read from file
    FILE *fp = fopen("in.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }
    // Print the contents of the file
    char c;
    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    fclose(fp);
}