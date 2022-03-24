#include <stdio.h>
#include <stdlib.h>

#define filein "grafo.txt"

typedef struct {
    int n_vertici;
    int n_archi;
    int **matrix;
    int *val;
} graph;


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}


graph leggiFile(void) {
    FILE *fp;
    graph grafo;
    int i;

    if ((fp = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }

    /* il numero di archi equivale al numero di righe da leggere nel file */
    fscanf(fp, "%d %d", &grafo.n_vertici, &grafo.n_archi);

    grafo.matrix = (int **) malloc(grafo.n_archi * sizeof(int *));
    checkErroreAllocazione(grafo.matrix);

    for (i=0; i<grafo.n_archi; i++) {
        grafo.matrix[i] = (int *) malloc(2 * sizeof(int));
        checkErroreAllocazione(grafo.matrix[i]);
        fscanf(fp, "%d %d", &grafo.matrix[i][0], &grafo.matrix[i][1]);
    }

    /* alloco un vettore con tutti i valori su cui fare il powerset */
    grafo.val = (int *) malloc(grafo.n_vertici * sizeof(int));
    checkErroreAllocazione(grafo.val);
    /* riempio il vettore dei valori con i valori degli N vertici (da 0 a N-1) */
    for (i=0; i<grafo.n_vertici; i++) {
        grafo.val[i] = i;
    }
    fclose(fp);
    return grafo;
}


int check(graph *graph_p, int *sol, int count) {
    int i = 0, j, flag = 0;

    while (flag == 0 && i < graph_p -> n_archi) {
        flag = -1;
        if (sol[graph_p -> matrix[i][0]] == 1 || sol[graph_p -> matrix[i][1]] == 1) {
            flag = 0;
            i++;
        }
    }

    if (i == graph_p -> n_archi) {
        printf("{");
        for (j=0; j<graph_p -> n_vertici; j++) {
            if (sol[j] != 0) {
                printf(" %d ", graph_p->val[j]);
            }
        }
        printf("};\n");
        return count+1;
    }
    return count;
}


int powersetR(int pos, graph *graph_p, int *sol, int count) {
    /* caso terminale */
    if (pos >= graph_p -> n_vertici) {
        count = check(graph_p, sol, count);
        return count;
    }

    sol[pos] = 0;
    count = powersetR(pos+1, graph_p, sol, count);
    sol[pos] = 1;
    count = powersetR(pos+1, graph_p, sol, count);

    return count;
}


/* funzione wrapper per powersetR() */
int powerset(graph *grafo_p) {
    int *sol, count = 0;

    sol = (int *) malloc(grafo_p -> n_vertici * sizeof(int));
    checkErroreAllocazione(sol);

    count = powersetR(0, grafo_p, sol, count);

    free(sol);
    return count;
}


void libera(graph *graph_p) {
    int i;

    for (i=0; i<graph_p -> n_archi; i++) {
        free(graph_p -> matrix[i]);
    }
    free(graph_p -> matrix);
    free(graph_p -> val);
}


int main(void) {
    graph dati;
    int sol_num;

    dati = leggiFile();
    sol_num = powerset(&dati);
    printf("The number of solutions is: %d\n", sol_num);

    libera(&dati);

    return EXIT_SUCCESS;
}