#include "ST.h"

/* la ST e un vettore di struct con ognuna id_elab e id_rete */
typedef struct vertex {
    char id_elab[MAXCHAR];
    char id_rete[MAXCHAR];
} vertex;

struct ST {
    vertex *vertex_array;
};


tab STinit(int V) {
    tab table = malloc(sizeof *table);
    checkErroreAllocazione(table);

    table->vertex_array = (vertex *) malloc(V * sizeof(vertex));
    checkErroreAllocazione(table->vertex_array);

    return table;
}


void STfree(tab table) {
    free(table->vertex_array);
    free(table);
}


void STinsert(tab table, char *id_elab, char *id_rete, int i) {
    strcpy(table->vertex_array[i].id_elab, id_elab);
    strcpy(table->vertex_array[i].id_rete, id_rete);
}


int STsearch(tab table, char *id_elab, int maxNum) {
    int j;
    for (j=0; j < maxNum; j++) {
        if (strcmp(table->vertex_array[j].id_elab, id_elab) == 0)
            return j;
    }
    return -1;
}


char *STsearchByIndex(tab table, int index) {
    return table->vertex_array[index].id_elab;
}


void printST(FILE *fout, tab table, int i) {
    fprintf(fout, "Name <%s>    Subnetwork <%s>\n", table->vertex_array[i].id_elab,
           table->vertex_array[i].id_rete);
}


void STtablecpy(tab tableDest, tab tableSrc, int maxNum) {
    for (int i=0; i < maxNum; i++) {
        strcpy(tableDest->vertex_array[i].id_elab, tableSrc->vertex_array[i].id_elab);
        strcpy(tableDest->vertex_array[i].id_rete, tableSrc->vertex_array[i].id_rete);
    }
}


void STmerge(tab A, tab B, int l, int m, int r) {
    int i, j, k;

    i = l;
    j = m+1;

    for (k=l; k<=r; k++) {
        if (i > m) {
            strcpy(B->vertex_array[k].id_elab, A->vertex_array[j].id_elab);
            strcpy(B->vertex_array[k].id_rete, A->vertex_array[j++].id_rete);
        }
        else if (j > r) {
            strcpy(B->vertex_array[k].id_elab, A->vertex_array[i].id_elab);
            strcpy(B->vertex_array[k].id_rete, A->vertex_array[i++].id_rete);
        }
        else if (strcmp(A->vertex_array[i].id_elab, A->vertex_array[j].id_elab) < 0 ||
                 strcmp(A->vertex_array[i].id_elab, A->vertex_array[j].id_elab) == 0)
        {
            strcpy(B->vertex_array[k].id_elab, A->vertex_array[i].id_elab);
            strcpy(B->vertex_array[k].id_rete, A->vertex_array[i++].id_rete);
        }
        else {
            strcpy(B->vertex_array[k].id_elab, A->vertex_array[j].id_elab);
            strcpy(B->vertex_array[k].id_rete, A->vertex_array[j++].id_rete);
        }
    }
    for (k=l; k<=r; k++) {
        A->vertex_array[k] = B->vertex_array[k];
    }
}


void STmergeSortR(tab A, tab B, int l, int r) {
    int m;

    if (r <= l)
        return;
    m = (l + r)/2;
    STmergeSortR(A, B, l, m);
    STmergeSortR(A, B, m+1, r);
    STmerge(A, B, l, m, r);
}


void STmergeSort(tab A, int N) {
    int l=0, r=N-1;
    tab B = STinit(N);
    STmergeSortR(A, B, l, r);

    STfree(B);
}


void STrealloc(tab table, int N) {
    table->vertex_array = (vertex *) realloc(table->vertex_array, N * sizeof(vertex));
    checkErroreAllocazione(table->vertex_array);
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}