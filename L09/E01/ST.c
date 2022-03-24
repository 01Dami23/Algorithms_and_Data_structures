#include "ST.h"

typedef struct vertex_s {
    char name[MAXCHAR];
} vertex;

struct ST {
    vertex *vertex_array;
    int V;
};


tab STinit(int V) {
    tab table = malloc(sizeof(*table));
    checkErroreAllocazione(table);

    table->vertex_array = (vertex *) malloc(V * sizeof(vertex));
    checkErroreAllocazione(table->vertex_array);

    table->V = 0;

    return table;
}


void STfree(tab table) {
    free(table->vertex_array);
    free(table);
}


void STinsert(tab table, char *name, int i) {
    strcpy(table->vertex_array[i].name, name);
    table->V++;
}


int STsearch(tab table, char *name) {
    for (int i=0; i < table->V; i++) {
        if (strcmp(name, table->vertex_array[i].name) == 0)
            return i;
    }
    return -1;
}


char *STsearchByIndex(tab table, int index) {
    return table->vertex_array[index].name;
}


void printST(FILE *fout, tab table, int i) {
    fprintf(fout, "Name <%s>\n", table->vertex_array[i].name);
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}