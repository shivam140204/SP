#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, DELIMITER, UNKNOWN
} TokenType;

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while"
};
const char operators[] = "+-*/=%<>&|^!";
const char delimiters[] = "(),;{}[]";

int isKeyword(const char *str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    for (int i = 0; i < strlen(operators); i++) {
        if (ch == operators[i])
            return 1;
    }
    return 0;
}

int isDelimiter(char ch) {
    for (int i = 0; i < strlen(delimiters); i++) {
        if (ch == delimiters[i])
            return 1;
    }
    return 0;
}

void printToken(FILE *outputFile, TokenType type, const char *token) {
    const char *typeStr;
    switch (type) {
        case KEYWORD: 
            typeStr = "Keyword"; 
            break;
        case IDENTIFIER: 
            typeStr = "Identifier"; 
            break;
        case NUMBER: 
            typeStr = "Number"; 
            break;
        case OPERATOR: 
            typeStr = "Operator"; 
            break;
        case DELIMITER: 
            typeStr = "Delimiter"; 
            break;
        default: 
            typeStr = "Unknown"; 
            break;
    }
    fprintf(outputFile, "| %-12s | %-15s |\n", typeStr, token);
}

void analyze(const char *src, FILE *outputFile) {
    char token[MAX_TOKEN_LENGTH];
    int i = 0, j = 0;
    
    fprintf(outputFile, "+--------------+-----------------+\n");
    fprintf(outputFile, "| Token Type   | Token Value     |\n");
    fprintf(outputFile, "+--------------+-----------------+\n");
    
    while (src[i] != '\0') {
        if (isspace(src[i])) {
            i++;
            continue;
        }

        if (isalpha(src[i])) {  
            j = 0;
            while (isalnum(src[i])) {
                token[j++] = src[i++];
            }
            token[j] = '\0';
            if (isKeyword(token))
                printToken(outputFile, KEYWORD, token);
            else
                printToken(outputFile, IDENTIFIER, token);
        }
        else if (isdigit(src[i])) {  
            j = 0;
            while (isdigit(src[i])) {
                token[j++] = src[i++];
            }
            token[j] = '\0';
            printToken(outputFile, NUMBER, token);
        }
        else if (isOperator(src[i])) { 
            token[0] = src[i++];
            token[1] = '\0';
            printToken(outputFile, OPERATOR, token);
        }
        else if (isDelimiter(src[i])) { 
            token[0] = src[i++];
            token[1] = '\0';
            printToken(outputFile, DELIMITER, token);
        }
        else {  
            token[0] = src[i++];
            token[1] = '\0';
            printToken(outputFile, UNKNOWN, token);
        }
    }
    fprintf(outputFile, "+--------------+-----------------+\n");
}

int main() {
    FILE *inputFile = fopen("in.c", "r");
    if (!inputFile) {
        printf("Error opening input file.\n");
        return 1;
    }

    FILE *outputFile = fopen("out.txt", "w");
    if (!outputFile) {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return 1;
    }

    char buffer[1024];
    char sourceCode[1024] = ""; 

    while (fgets(buffer, sizeof(buffer), inputFile)) {
        strcat(sourceCode, buffer);
    }
    
    fclose(inputFile);
    
    fprintf(outputFile, "Lexical Analysis:\n\n");
    analyze(sourceCode, outputFile);
    
    fclose(outputFile);

    printf("Lexical analysis complete. Output written to 'out.txt'.\n");
    
    return 0;
}
