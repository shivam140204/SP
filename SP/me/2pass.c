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


void processLine(char *line, FILE *intermediate) {
    char label[10] = "", opcode[10] = "", operand1[10] = "", operand2[10] = "";
    int fields = sscanf(line, "%s %s %s %s", label, opcode, operand1, operand2);
     printf("\nCHEKC %s %s %s %d",label,opcode,operand1,locctr);
    if (fields >= 3 && strlen(label) > 0 && strcmp(opcode, "START") != 0 && strcmp(opcode, "END") != 0  && strcmp(label, "END") != 0 && strcmp(label, "START") != 0) {
        
        int symIndex = searsys(label);
        if (symIndex == -1) {
            strcpy(symtab[symcount].symbol, label);
            symtab[symcount].address = locctr;
            symcount++;
        } else {
            symtab[symIndex].address = locctr;
        }
    }
    else if (fields>=3 && strlen(label) > 0 && strcmp(opcode, "START") != 0 && strcmp(opcode, "END") != 0 && strcmp(label, "END") != 0 && strcmp(label, "START") != 0) {
        int symIndex = searsys(label);
        if (symIndex == -1) {
            strcpy(symtab[symcount].symbol, label);
            symtab[symcount].address = locctr;
            symcount++;
        } else {
            symtab[symIndex].address = locctr;
        }
    }
    if (fields==3 && operand1[0] == '='){
        int opcodeVal = gop(label, MOT, 11);
        printf("\nTHREE %s %s %s %d",label,opcode,operand1,locctr);
        if (opcodeVal != -1) {
            int regCode = grp(opcode);
            int litIndex = searl(operand1);
            if (litIndex == -1 && operand1[0] == '=') {
                strcpy(litt[litcount].literal, operand1);
                litt[litcount].address = 0; 
                litcount++;
                litIndex = litcount - 1;
            }
            fprintf(intermediate, "%d\t(IS, %02d)\t(%d)\t(L, %d)\n", locctr, opcodeVal, regCode, litIndex + 1);
            locctr++;
        } else {
            
            opcodeVal = gop(opcode, POT, 5);
            if (opcodeVal != -1) {
                fprintf(intermediate, "%d\t(AD, %02d)\t(C, %s)\n", locctr, opcodeVal, operand1);
                if (strcmp(opcode, "START") == 0 || strcmp(opcode, "ORIGIN") == 0) {
                    locctr = atoi(operand1);
                    //locctr ++;
                }
            } else {
                opcodeVal = gop(opcode, DL, 2);
                if (opcodeVal != -1) {
                    fprintf(intermediate, "%d\t(DL, %02d)\t(C, %s)\n", locctr, opcodeVal, operand1);
                    locctr += atoi(operand1);
                    
                }
            }
        }
    }
    else if (fields==3 && operand1[0] != '=' && (label[0]!='B' && label[1]!='C')){
        int opcodeVal = gop(label, MOT, 11);
        printf("\nFIVEEE %s %s %s %d",label,opcode,operand1,locctr);
        if (opcodeVal != -1) {
            int regCode = grp(opcode);
            printf("MEEEEE\n");
            fprintf(intermediate, "%d\t(IS, %02d)\t(%d)\t(S, %d)\n", locctr, opcodeVal, regCode, searsys(operand1)+1);
            locctr++;
        } else if (gop(opcode, POT, 5)==3){
            //nothing
        }else {
            
            opcodeVal = gop(opcode, POT, 5);
            if (opcodeVal != -1) {
                fprintf(intermediate, "%d\t(AD, %02d)\t(C, %s)\n", locctr, opcodeVal, operand1);
                if (strcmp(opcode, "START") == 0 || strcmp(opcode, "ORIGIN") == 0) {
                    locctr = atoi(operand1);
                    //locctr ++;
                }
            } else {
                opcodeVal = gop(opcode, DL, 2);
                if (opcodeVal != -1) {
                   int symIndex = searsys(label);
                   printf("\nHEREEE %s\n",label);
                   if (symIndex == -1) {
                        printf("\nHEREEE %s\n",label);
                        strcpy(symtab[symcount].symbol, label);
                        symtab[symcount].address = locctr;
                        symcount++;
                    } else {
                        symtab[symIndex].address = locctr;
                    }
                    fprintf(intermediate, "%d\t(DL, %02d)\t %s\n", locctr, opcodeVal, operand1);
                   // locctr += atoi(operand1);
                   locctr ++;
                    
                }
            }
        }
    }
    else if (fields >= 3) {
      
        printf("\nFOUR %s %s %s %s %d",label,opcode,operand1,operand2,locctr);
        int opcodeVal = gop(opcode, MOT, 11);
        if (opcodeVal != -1) {
            int regCode = grp(operand1);
            int litIndex = searl(operand2);
            int symIndex = searsys(operand2);
            int l=0;
            if (litIndex == -1 && operand2[0] == '=') {
                strcpy(litt[litcount].literal, operand2);
                litt[litcount].address = 0; 
                litcount++;
                litIndex = litcount - 1;
                
            }else if (symIndex == -1) {
                        printf("\nHEREEE %s\n",operand1);
                        strcpy(symtab[symcount].symbol, operand1);
                        symtab[symcount].address = locctr;
                        symcount++;
            }
            litIndex = searl(operand2);
            symIndex = searsys(operand2);
            if(litIndex!=-1){
                fprintf(intermediate, "%d\t(IS, %02d)\t(%d)\t(L, %d)\n", locctr, opcodeVal, regCode, litIndex + 1);
            }else{
                fprintf(intermediate, "%d\t(IS, %02d)\t(%d)\t(S, %d)\n", locctr, opcodeVal, regCode, symIndex + 1);
            }
            locctr++;
        } else {
            
            opcodeVal = gop(opcode, POT, 5);
            if (opcodeVal != -1) {
                fprintf(intermediate, "%d\t(AD, %02d)\t(C, %s)\n", locctr, opcodeVal, operand1);
                if (strcmp(opcode, "START") == 0 || strcmp(opcode, "ORIGIN") == 0) {
                    locctr = atoi(operand1);
                    //locctr ++;
                }
            } else if (strcmp(label, "BC") == 0) {
                   int conditionCode = atoi(opcode);
                   int symIndex = searsys(operand1);
                   if (symIndex == -1) {
                        printf("\nHEREEE %s\n",operand1);
                        strcpy(symtab[symcount].symbol, operand1);
                        symtab[symcount].address = locctr;
                        symcount++;
                    } else {
                        symtab[symIndex].address = locctr;
                    }
                        fprintf(intermediate, "%d\t(IS, %02d)\t(S, %d)\n", locctr, gop(label, MOT, 11), symcount);
                        locctr++;
                    
                }else {
                opcodeVal = gop(opcode, DL, 2);
                if (opcodeVal != -1) {
                   int symIndex = searsys(label);
                   printf("\nHEREEE %s\n",label);
                   if (symIndex == -1) {
                        printf("\nHEREEE %s\n",label);
                        strcpy(symtab[symcount].symbol, label);
                        symtab[symcount].address = locctr;
                        symcount++;
                    } else {
                        symtab[symIndex].address = locctr;
                    }
                    fprintf(intermediate, "%d\t(DL, %02d)\t %s\n", locctr, opcodeVal, operand1);
                   // locctr += atoi(operand1);
                   locctr ++;
                    
                    
                }
            }
        }
    } else if (fields == 2) {
        printf("\nTWOLO %s %s %d",label,opcode,locctr);
        int opcodeVal = gop(label, POT, 5);
        if (strcmp(label, "PRINT") == 0) {
            int regCode = grp(opcode); 
            if(regCode!=-1){
                fprintf(intermediate, "%d\t(IS, 09)\t(S, %d)\n", locctr, regCode);
                locctr++;
            }else if (searsys(opcode)!=-1){
                fprintf(intermediate, "%d\t(IS, 09)\t(S, %d)\n", locctr,searsys(opcode));
                locctr++;
            }else if (searl(opcode)!=-1){
                fprintf(intermediate, "%d\t(IS, 09)\t(L, %d)\n", locctr, searl(opcode));
                locctr++;
            }
        }
        else if (opcodeVal != -1) {
            if ( strcmp(label, "ORIGIN") == 0) {
                locctr = atoi(opcode);
                //locctr ++;
            }
            else if  (strcmp(label, "START") == 0 ){
                locctr = atoi(opcode);
                fprintf(intermediate, "%d\t(AD, %02d)\t(C, %s)\n", locctr, opcodeVal, opcode);
            }
        } else if (gop(label, MOT, 11)==10){
            //NEEWWWWWWWW
            int symIndex = searsys(opcode);
            printf("\nHEREEE %s %d\n",opcode,symIndex);
            if (symIndex == -1) {
                printf("\nHEREEE %s\n",opcode);
                strcpy(symtab[symcount].symbol, opcode);
                symtab[symcount].address = locctr;
                symcount++;
            } else {
                symtab[symIndex].address = locctr;
            }
            fprintf(intermediate, "%d\t(IS, %02d)\t(S, %d)\n", locctr, gop(label, MOT, 11), symcount);
            locctr++;
        }else if(gop(label, MOT, 11)!=-1){
            //NEW
            if(opcode[0]!='='){
                printf("AKALKALAAL: %c\n ",opcode[0]);
                opcodeVal = gop(label, MOT, 11);
                int reg=0;
                reg=grp(opcode);
                if(reg==-1){
                    fprintf(intermediate, "%d\t(IS, %02d)\t(S, %d)\n", locctr, opcodeVal, opcode);
                    locctr++;
                }else{
                    fprintf(intermediate, "%d\t(IS, %02d)\t(S, %d)\n", locctr, opcodeVal, reg);
                    locctr++;
                }
             }else{
                int litIndex = searl(opcode);
                opcodeVal = gop(label, MOT, 11);
                if (litIndex == -1 && opcode[0] == '=') {
                    strcpy(litt[litcount].literal, opcode);
                    litt[litcount].address = 0; 
                    litcount++;
                    litIndex = litcount - 1;
                }
                fprintf(intermediate, "%d\t(IS, %02d)\t(L, %d)\n", locctr, opcodeVal, litIndex + 1);
                locctr++;
            }
        }else {
            opcodeVal = gop(label, DL, 2);
            if (opcodeVal != -1) {
                printf("\nLELE\n");
                fprintf(intermediate, "%d\t(DL, %02d)\t(C, %s)\n", locctr, opcodeVal, opcode);
                locctr += atoi(opcode);
                //locctr ++;
            } else {
                opcodeVal = gop(label, MOT, 11);
                if (opcodeVal != -1) {
                    int regCode = grp(opcode);
                    fprintf(intermediate, "%d\t(IS, %02d)\t(%d)\n", locctr, opcodeVal, regCode);
                }
            }
        }
        
    } else if (fields == 1) {
     
        int opcodeVal = gop(label, POT, 5);
       
        if (opcodeVal != -1) {
            //fprintf(intermediate, "%d\t(AD, %02d)\n", locctr, opcodeVal);
            if (strcmp(label, "LTORG") == 0||strcmp(label, "END") == 0) {
                printf("\nZZZZZZ %d\n",opcodeVal);
                for (int i = 0; i < litcount; i++) {
                    if (litt[i].address == 0) {
                        litt[i].address = locctr;
                        fprintf(intermediate, "%d\t(AD, %02d)\t%d\n", locctr, opcodeVal, litt[i].address);
                        locctr++;
                    }
                }
            }
        } 
    }else if (strcmp(label, "BC") == 0) {
    

            int conditionCode = atoi(operand1);
            if (conditionCode >= 1 && conditionCode <= 12) {
                fprintf(intermediate, "%d\t(IS, %02d)\t(C, %d)\n", locctr, gop(opcode, MOT, 11), conditionCode);
                locctr++;
            } 
        }
    else {
            fprintf(intermediate, "%d\t%s\n", locctr, label);
            locctr++;
        }
    }

void generateMachineCode(FILE *intermediate, FILE *machine) {
    char line[50];
    int address, opcode, regCode, operand;
    int litIndex, symIndex;
    char l[50];
    int t=0,fl=0,k=0;

    //char labeel[10] = "", opcodee[10] = "", operand11[10] = "", operand22[10] = "",op[10]="",po[10]="";
   // int fields = sscanf(line, "%s %s %s %s %s %s", labeel, opcodee, operand11, operand22,op,po);

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
//ad,05 LITERAL
//DL,INDEX space
//dont print label equ loop lines
//dont print origin lines

int main() {
    FILE *input = fopen("2pass_input.asm", "r");
    FILE *intermediate = fopen("2pass_intermediate.txt", "w");
    FILE *machineCode = fopen("2pass_machineCode.txt", "w");

    char line[50];
    while (fgets(line, sizeof(line), input)) {
        processLine(line, intermediate);
    }

    fclose(input);
    fclose(intermediate);

    intermediate = fopen("2pass_intermediate.txt", "r");
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