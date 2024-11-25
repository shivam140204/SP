#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char optab[][6] = {"STOP","ADD","SUB","MULT",
                   "MOVER","MOVEM","COMP","BC",
                   "DIV","READ","PRINT"};

int search_op(char *s)
{
    int i;
    for(i = 0; i < 11; i++)
    {
        if(strcmp(optab[i], s) == 0)
            return i;
    }
    return -1;
}

char regtab[][5] = {"AREG","BREG","CREG","DREG"};

int search_reg(char *s)
{
    int i;
    for(i = 0; i < 4; i++)
    {
        if(strcmp(regtab[i], s) == 0)
            return i;
    }
    return -1;
}

char adtab[][7] = {"START","END","ORIGIN","EQU","LTORG"};

int search_ad(char *s)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        if(strcmp(adtab[i], s) == 0)
            return i;
    }
    return -1;
}

int main()
{
    FILE *fp, *fp1;
    char fname[40], buff[80], t1[20], t2[20], t3[20], t4[20];
    int n, i, j, pc = 0;
    
    printf("Enter source file name: ");
    scanf("%s", fname);
    
    fp = fopen(fname, "r");
    if(fp == NULL)
    {
        printf("File %s not found.\n", fname);
        exit(1);
    }
    
    fp1 = fopen("temp.i", "w");
    
    while(fgets(buff, 80, fp) != NULL)
    {
        n = sscanf(buff, "%s %s %s %s", t1, t2, t3, t4);
        
        switch(n)
        {
            case 1:
                i = search_op(t1);
                if(i == 0) // STOP
                {
                    fprintf(fp1, "%03d) (IS, %02d)\n", pc, i);
                    break;
                }
                i = search_ad(t1);
                fprintf(fp1, "%03d) (AD, %02d)\n", pc, i+1);
                break;
            
            case 2:
                i = search_ad(t1);
                if(i == 0) // START
                {
                    fprintf(fp1, "%03d) (AD, %02d) (C, %s)\n", pc, i+1, t2);
                    pc = atoi(t2) - 1;
                    break;
                }
                i = search_op(t1);
                if(i == 9 || i == 10) // READ or PRINT
                {
                    fprintf(fp1, "%03d) (IS, %02d) %s\n", pc, i, t2);
                    break;
                }
                fprintf(fp1, "%03d) (IS, 00)\n", pc);
                break;
            
            case 3:
                if(strcmp(t2, "DS") == 0)
                {
                    fprintf(fp1, "%03d) (DL, 02) (C, %s)\n", pc, t3);
                    pc += atoi(t3) - 1;
                    break;
                }
                if(strcmp(t2, "DC") == 0)
                {
                    fprintf(fp1, "%03d) (DL, 01) (C, %d)\n", pc, atoi(t3+1));
                    break;
                }
                i = search_op(t1);
                if(i >= 1 && i <= 8) // ADD to DIV
                {
                    t2[strlen(t2)-1] = '\0';
                    j = search_reg(t2);
                    fprintf(fp1, "%03d) (IS, %02d) (%d) %s\n", pc, i, j+1, t3);
                    break;
                }
                i = search_op(t2);
                fprintf(fp1, "%03d) (IS, %02d) %s\n", pc, i, t3);
                break;
            
            case 4:
                i = search_op(t2);
                t3[strlen(t3)-1] = '\0';
                j = search_reg(t3);
                fprintf(fp1, "%03d) (IS, %02d) (%d) %s\n", pc, i, j+1, t4);
                break;
        }
        pc++;
    }
    
    fclose(fp);
    fclose(fp1);
    
    printf("Intermediate Code:\n");
    fp1 = fopen("temp.i", "r");
    while(fgets(buff, 80, fp1) != NULL)
        printf("%s", buff);
    
    fclose(fp1);
    return 0;
}
