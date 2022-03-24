#ifndef LAB_9_ST_H
#define LAB_9_ST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXCHAR 31

typedef struct ST *tab;

tab STinit(int V);
void STfree(tab table);
void STinsert(tab table, char *name, int i);
int STsearch(tab table, char *name);
char *STsearchByIndex(tab table, int index);
void printST(FILE *fout, tab table, int i);
void checkErroreAllocazione(void *p);

#endif