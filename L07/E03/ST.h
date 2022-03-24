#ifndef LAB_7_ST_H
#define LAB_7_ST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 30+1

typedef struct ST *tab;

tab STinit(int V);
void STfree(tab table);
void STinsert(tab table, char *id_elab, char *id_rete, int i);
/* funzione che ritorna l'indice a cui sta il vertice nella ST (se non c'e ritorna -1) */
int STsearch(tab table, char *id_elab, int maxNum);
char *STsearchByIndex(tab table, int index);

void printST(FILE *fout, tab table, int i);
void STtablecpy(tab tableDest, tab tableSrc, int maxNum);
void STmergeSort(tab A, int N);
void STrealloc(tab table, int N);
void checkErroreAllocazione(void *p);

#endif