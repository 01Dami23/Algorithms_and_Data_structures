#include <stdio.h>
#include <stdlib.h>

#define filein "easy_test_set.txt"

typedef enum {
    zaffiro, rubino, topazio, smeraldo
} stone_types;

void necklaceR(int pos, int *sol, int *val, int k, int *stop_p);
int check(int previous, int current);
void checkErroreAllocazione(void *p);


int main(void) {
    FILE *fin;
    int num, i, data[4], maxLen, lenSol, *sol, stop = 0;
    int j;

    if  ((fin = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &num);
    for (i=0; i<num; i++) {
        maxLen = 0;
        stop = 0;

        fscanf(fin, "%d %d %d %d", &data[zaffiro], &data[rubino],
               &data[topazio], &data[smeraldo]);

        lenSol = data[zaffiro] + data[smeraldo] + data[rubino] + data[topazio];
        sol = (int *) malloc(lenSol * sizeof(int));
        checkErroreAllocazione(sol);


        printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
               i+1, data[zaffiro], data[rubino], data[topazio], data[smeraldo], lenSol);

        for (j=1; j<=lenSol; j++) {
            stop = 0;
            necklaceR(0, sol, data, j, &stop);
            if (stop == 1)
                maxLen = j;
        }
        printf("Collana massima di lunghezza %d\n", maxLen);

        free(sol);
    }

    fclose(fin);
    return EXIT_SUCCESS;
}


void necklaceR(int pos, int *sol, int *val, int k, int *stop_p) {
    int i;

    /* condizione di terminazione */
    if (pos >= k) {
        *stop_p = 1;
        return;
    }

    for (i=zaffiro; i<=smeraldo; i++) {
        if (val[i] > 0 && *stop_p == 0 && (pos == 0 || check(sol[pos-1], i) == 0)) {
            sol[pos] = i;
            val[i]--;
            necklaceR(pos+1, sol, val, k, stop_p);
            val[i]++;
        }
    }
}


int check(int previous, int current) {

    if (!((previous == zaffiro || previous == topazio) && (current == zaffiro || current == rubino)) &&
        !((previous == smeraldo || previous == rubino) && (current == smeraldo || current == topazio))) {
        return -1;
    }
    return 0;
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}