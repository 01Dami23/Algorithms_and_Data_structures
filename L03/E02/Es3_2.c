#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filein "brani.txt"
#define MAXCHAR 256

typedef struct {
    char **canzoni;
    int num;
} amico;

amico *leggiFile(FILE *fp, int n);
int princ_molt(int pos, amico *amici_vett, char **sol, int n, int count);
void checkErroreAllocazione(void *p);
void stampaSol(char **sol, int n);

int main(void) {
    FILE *fin;
    amico *vett_amici;
    int num_sol, num_amici, num_canzoni, i, j;
    char **sol;

    if ((fin = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &num_amici);

    sol = (char **) malloc(num_amici * sizeof(char *));
    checkErroreAllocazione(sol);
    for (i=0; i<num_amici; i++) {
        sol[i] = (char *) malloc(MAXCHAR * sizeof(char));
        checkErroreAllocazione(sol[i]);
    }

    vett_amici = leggiFile(fin, num_amici);

    num_sol = princ_molt(0, vett_amici, sol, num_amici, 0);
    printf("The number of solutions is: %d\n", num_sol);


    /* libero il vettore delle soluzioni, prima pero libero i punt del vett sol */
    for (i=0; i<num_amici; i++) {
        free(sol[i]);
    }
    free(sol);
    /* libero la struct degli amici, prima pero libero le canzoni e i vettori di punt a canzoni */
    for (i=0; i<num_amici; i++) {
        num_canzoni = vett_amici[i].num;
        for (j=0; j<num_canzoni; j++) {
            free(vett_amici[i].canzoni[j]);
        }
        free(vett_amici[i].canzoni);
    }
    free(vett_amici);

    fclose(fin);
    return EXIT_SUCCESS;
}


amico *leggiFile(FILE *fp, int n) {
    int num_canzoni, i, j, len;
    char canzone[MAXCHAR];
    amico *amici_p;

    /* alloco il vettore di struct */
    amici_p = (amico *) malloc(n * sizeof(amico));
    checkErroreAllocazione(amici_p);

    for (i=0; i<n; i++) {
        fscanf(fp, "%d", &num_canzoni);

        (amici_p+i) -> num = num_canzoni;
        /* alloco il vettore di puntatori a char (puntatori alle singole canzoni che sono stringhe) */
        (amici_p+i) -> canzoni = (char **) malloc(num_canzoni * sizeof(char *));
        checkErroreAllocazione((amici_p+i) -> canzoni);

        for (j=0; j<num_canzoni; j++) {
            fscanf(fp, "%s", canzone);
            len = strlen(canzone) + 1;
            (amici_p+i) -> canzoni[j] = (char *) malloc(len * sizeof(char));
            checkErroreAllocazione((amici_p+i) -> canzoni[j]);
            strcpy((amici_p+i) -> canzoni[j], canzone);
        }
    }
    return amici_p;
}


int princ_molt(int pos, amico *amici_vett, char **sol, int n, int count) {
    int i;

    if (pos >= n) {
        stampaSol(sol, n);
        return count+1;
    }
    for (i=0; i<amici_vett[pos].num; i++) {
        strcpy(sol[pos], amici_vett[pos].canzoni[i]);
        count = princ_molt(pos+1, amici_vett, sol, n, count);
    }
    return count;
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}


void stampaSol(char **sol, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%s\n", sol[i]);
    }
    printf("\n");
}