#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node
{
    char symb[20];
    int addr,val,len;
    struct node *next;
}NODE;

NODE *first = NULL, *last = NULL;

void add_symb(char *s, int a, int v, int l)
{
    NODE *p;
    p = (NODE*)malloc(sizeof(NODE));
    strcpy(p->symb, s);
    p->addr = a;
    p->val = v;
    p->len = l;
    p->next = NULL;
    
    if(first == NULL)
        first = p;
    else
        last->next = p;
    
    last = p;
}

void display_symbtab()
{
    NODE *p;
    printf("Symbol Table:\n");
    printf("symb\taddr\tval\tlen\n");
    p = first;
    
    while(p != NULL)
    {
        printf("%s\t%d\t%d\t%d\n",
               p->symb, p->addr, p->val, p->len);
        p = p->next;
    }
}

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
    FILE *fp;
    char fname[40], buff[80], t1[20], t2[20], t3[20], t4[20];
    int n, i, pc = 0;
    
    printf("Enter source file name: ");
    scanf("%s", fname);
    
    fp = fopen(fname, "r");
    if(fp == NULL)
    {
        printf("File %s not found.\n", fname);
        exit(1);
    }
    
    while(fgets(buff, 80, fp) != NULL)
    {
        n = sscanf(buff, "%s %s %s %s", t1, t2, t3, t4);
        
        switch(n)
        {
            case 2:
                // Only add labels that are not instructions
                if(search_op(t1) == -1 && 
                   search_ad(t1) == -1 && 
                   strcmp(t1, "START") != 0)
                {
                    add_symb(t1, pc, 0, 0);
                }
                
                if(strcmp(t1, "START") == 0)
                {
                    pc = atoi(t2) - 1;
                }
                break;
            
            case 3:
                // For DS, DC, and labels
                if(strcmp(t2, "DS") == 0)
                {
                    add_symb(t1, pc, 0, atoi(t3));
                    pc += atoi(t3) - 1;
                    break;
                }
                
                if(strcmp(t2, "DC") == 0)
                {
                    add_symb(t1, pc, atoi(t3+1), 1);
                    break;
                }
                
                // Only add if not an instruction
                if(search_op(t1) == -1 && search_ad(t1) == -1)
                {
                    add_symb(t1, pc, 0, 0);
                }
                break;
            
            case 4:
                // Only add label if not an instruction
                if(search_op(t2) == -1 && search_ad(t2) == -1)
                {
                    add_symb(t1, pc, 0, 0);
                }
                break;
        }
        pc++;
    }
    
    fclose(fp);
    display_symbtab();
    
    return 0;
}
