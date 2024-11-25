#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

// Function to trim leading and trailing whitespace
void trimWhitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    end = str + strlen(str);
    while (end > str && isspace((unsigned char)*(end - 1))) end--;
    *end = '\0';
}

// Function to process MDT file and generate output
void processMDT(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    
    // Add starting address to the output
    fprintf(output, "START 100\n\n");

    while (fgets(line, sizeof(line), input)) {
        trimWhitespace(line);

        // Skip lines that contain "MACRO" or "MEND"
        if (strstr(line, "MACRO") != NULL || strstr(line, "MEND") != NULL) {
            continue;
        }

        // Remove "END1", "END2", "END3" keywords from the line
        char *ptr = line;
        while ((ptr = strstr(ptr, "END1")) != NULL) {
            memmove(ptr, ptr + 4, strlen(ptr + 4) + 1);  // Remove "END1"
        }
        ptr = line;
        while ((ptr = strstr(ptr, "END2")) != NULL) {
            memmove(ptr, ptr + 4, strlen(ptr + 4) + 1);  // Remove "END2"
        }
        ptr = line;
        while ((ptr = strstr(ptr, "END3")) != NULL) {
            memmove(ptr, ptr + 4, strlen(ptr + 4) + 1);  // Remove "END3"
        }

        // Write the cleaned line to the output
        fprintf(output, "%s\n", line);
    }

    // Add END at the last line
    fprintf(output, "\nEND\n");
}

int main() {
    FILE *input = fopen("MDT.txt", "r");
    FILE *output = fopen("output.txt", "w");

    if (!input || !output) {
        printf("Error opening files.\n");
        return 1;
    }

    processMDT(input, output);

    fclose(input);
    fclose(output);
    
    printf("MDT processed and output written to 'output.txt'.\n");

    return 0;
}
