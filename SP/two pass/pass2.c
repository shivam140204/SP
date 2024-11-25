#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char symbol[10];
    int address;
} syymbol;

typedef struct {
    char literal[10];
    int address;
} littab;

typedef struct {
    char mnemonic[10];
    int opcode;
} mot;

typedef struct {
    char reg[10];
    int code;
} registertab;

mot MOT[] = {
    {"MOVER", 1}, {"MOVEM", 2}, {"ADD", 3}, {"SUB", 4}, {"MULT", 5},
    {"DIV", 6}, {"BC", 7}, {"COMP", 8}, {"PRINT", 9}, {"READ", 10} ,{"SHIFT",11},{"BC",12}
};
mot POT[] = {
    {"START", 1}, {"END", 2}, {"EQU", 3}, {"ORIGIN", 4}, {"LTORG", 5}
};
mot DL[] = {
    {"DS", 1}, {"DC", 2}
};
registertab registers[] = {
    {"AREG", 1}, {"BREG", 2}, {"CREG", 3}, {"DREG", 4}
};

syymbol symtab[50];
littab litt[50];
int symcount = 0, litcount = 0, locctr = 0;

int searsys(char *symbol) {
    for (int i = 0; i < symcount; i++) {
        if (strcmp(symtab[i].symbol, symbol) == 0) {
            return i;
        }
    }
    return -1;
}

int searl(char *literal) {
    for (int i = 0; i < litcount; i++) {
        if (strcmp(litt[i].literal, literal) == 0) {
            return i;
        }
    }
    return -1;
}

int gop(char *mnemonic, mot table[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(table[i].mnemonic, mnemonic) == 0) {
            return table[i].opcode;
        }
    }
    return -1;
}

int grp(char *reg) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(registers[i].reg, reg) == 0) {
            return registers[i].code;
        }
    }
    return -1;
}

void readTablesFromFile() {
    FILE *symbolFile = fopen("symbol_table.txt", "r");
    FILE *literalFile = fopen("literal_table.txt", "r");
 
    symcount = 0;
    while (fscanf(symbolFile, "%s %d", symtab[symcount].symbol, &symtab[symcount].address) == 2) {
        symcount++;
    }
    litcount = 0;
    while (fscanf(literalFile, "%s %d", litt[litcount].literal, &litt[litcount].address) == 2) {
        litcount++;
    }
    fclose(symbolFile);
    fclose(literalFile);
    printf("Tables read from files successfully.\n");
}

void generateMachineCode(FILE *intermediate, FILE *machine) {
    char line[50];
    int address, opcode, regCode, operand;
    int litIndex, symIndex;
    char l[50];
    int t=0,fl=0,k=0;

    readTablesFromFile();
    while (fgets(line, sizeof(line), intermediate)) {
  //note: for registersss
        if (sscanf(line, "%d\t(IS, %02d)\t(%d)\t(S, %d)", &address, &opcode, &regCode, &operand) == 4) {
            symIndex = operand - 1;
            if (symIndex >= 0 && symIndex < symcount) {
                if(regCode){
                    printf("MACA %d\n",regCode);
                    fprintf(machine, "%d %02d %d %d\n", address, opcode, regCode, symtab[symIndex].address);
                }else{
                    printf("dadA %d\n",regCode);
                    fprintf(machine, "%d %02d  %d\n", address, opcode, symtab[symIndex].address);
                }
            } else {
                printf("dadA %d\n",regCode);
                fprintf(machine, "%d %02d %d %d\n", address, opcode, regCode, operand); 
            }
        } 
        else if (sscanf(line, "%d\t(IS, %02d)\t(%d)\t(L, %d)", &address, &opcode, &regCode, &operand) == 4) {
            litIndex = operand - 1;
            if (litIndex >= 0 && litIndex < litcount) {
                fprintf(machine, "%d %02d %d %d\n", address, opcode, regCode, litt[litIndex].address);
            } else {
                fprintf(machine, "%d %02d %d %d\n", address, opcode, regCode, operand); 
            }
        } 
//note: without
        else if (sscanf(line, "%d\t(IS, %02d)\t(S, %d)", &address, &opcode, &operand) == 3) {
            symIndex = operand - 1;
            if (symIndex >= 0 && symIndex < symcount) {
                fprintf(machine, "%d %02d  %d\n", address, opcode,  symtab[symIndex].address);
            } else {
                fprintf(machine, "%d %02d  %d\n", address, opcode,  operand);
            }
        }     
        else if (sscanf(line, "%d\t(IS, %02d)\t(L, %d)", &address, &opcode, &operand) == 3) {
            litIndex = operand - 1;
            if (litIndex >= 0 && litIndex < litcount) {
                fprintf(machine, "%d %02d  %d\n", address, opcode,  litt[litIndex].address);
            } else {
                fprintf(machine, "%d %02d  %d\n", address, opcode,  operand); 
            }
        }
        else if (sscanf(line, "%d\t(AD, %02d)\t(C, %s)", &address, &opcode, line) == 3) {
            fprintf(machine, "%d %02d %s\n", address, opcode, line);
        }  
        else if (sscanf(line, "%d\t(DL, %02d)\t %s", &address, &opcode, line) == 3) {
            fprintf(machine, "%d %02d %s\n", address, opcode, line);
        } 
        else if (sscanf(line, "%d\t(AD, %02d)", &address, &opcode) == 2) {
           // fprintf(machine, "%d %02d\n", address, opcode);
            fprintf(machine, "%d %d %d\n",address,opcode,litt[k].address);
            k++;
        } 
        else if (sscanf(line, "%d\t(DL, %02d)", &address, &opcode) == 2) {
            fprintf(machine, "%d %02d\n", address, opcode);
        } 
        else if (sscanf(line, "%d\t%s", &address,&opcode, line) == 2) {
            fprintf(machine, "%d %d %d\n",address,k,litt[k].address);
            printf("\nliterals %d %d\n",k,litt[k].address);
            k++;
        }
    }
}

int main() {
    FILE *input = fopen("in7.txt", "r");
    FILE *machineCode = fopen("machineCodee.txt", "w");
    fclose(input);

    FILE *intermediate = fopen("intermediate.txt", "r");
    generateMachineCode(intermediate, machineCode);

    fclose(intermediate);
    fclose(machineCode);

    printf("\nSymbol Table:\n");
    for (int i = 0; i < symcount; i++) {
        if (gop(symtab[i].symbol, MOT, 11) == -1 && gop(symtab[i].symbol, DL, 2) == -1) {
            printf("%s\t%d\n", symtab[i].symbol, symtab[i].address);
        }
    }

    printf("Literal Table:\n");
    for (int i = 0; i <= litcount; i++) {
        printf("%s\t%d\n", litt[i].literal, litt[i].address);
    }

    return 0;
}