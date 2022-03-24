#include <stdio.h>
#include <stdlib.h>

#define filein "mat.txt"

void malloc2dP(int ***mp, int nr, int nc);
void separa(int **mat, int nr, int nc, int **bianche_p, int **nere_p);
void checkErroreAllocazione(void *p);
void stampa(int *vet, int n);

int main(void) {
    FILE *fin;
    int nr, nc, i, j;
    int **matrice, *p_vet_bianche, *p_vet_nere;

    if ((fin = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d %d", &nr, &nc);
    malloc2dP(&matrice, nr, nc);

    for (i=0; i<nr; i++) {
        for (j=0; j<nc; j++) {
            fscanf(fin, "%d", &matrice[i][j]);
        }
    }

    separa(matrice, nr, nc, &p_vet_bianche, &p_vet_nere);

    if ((nr * nc) % 2 == 0) {
        printf("Caselle bianche: ");
        stampa(p_vet_bianche, (nr * nc) / 2);
        printf("Caselle nere: ");
        stampa(p_vet_nere, (nr * nc) / 2);
    }
    else {
        printf("Caselle bianche: ");
        stampa(p_vet_bianche, ((nr * nc) / 2) + 1);
        printf("Caselle nere: ");
        stampa(p_vet_nere, (nr * nc) / 2);
    }


    for (i=0; i<nr; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(p_vet_bianche);
    free(p_vet_nere);

    fclose(fin);
    return EXIT_SUCCESS;
}


void malloc2dP(int ***mp, int nr, int nc) {
    int **matrix;
    int i;

    matrix = (int **) malloc(nr * sizeof(int *));
    checkErroreAllocazione(matrix);
    for (i=0; i<nr; i++) {
        matrix[i] = (int *) malloc(nc * sizeof(int));
        checkErroreAllocazione(matrix[i]);
    }
    *mp = matrix;
}


void separa(int **mat, int nr, int nc, int **bianche_pp, int **nere_pp) {
    int num_bianche, num_nere, i, j, m = 0, n = 0, flag = 1;
    int *pb, *pn;

    /*considero sempre la prima casella come bianca quindi se il numero di caselle
     * e' dispari avro' una casella bianca in piu' rispetto a quelle nere*/
    num_bianche = (nr * nc) % 2 == 0 ? (nr * nc) / 2 : ((nr * nc) / 2) + 1;
    num_nere = (nr * nc) / 2;

    pb = (int *) malloc(num_bianche * sizeof(int));
    checkErroreAllocazione(pb);
    pn = (int *) malloc(num_nere * sizeof(int));
    checkErroreAllocazione(pn);

    for (i = 0; i < nr; i++) {
        if (nc % 2 == 0 && i > 0 && i % 2 != 0) {
            flag = 0;
        }
        else if (nc % 2 == 0 && i > 0 && i % 2 == 0) {
            flag = 1;
        }

        for (j = 0; j < nc; j++) {
            if (flag == 1) {
                pb[m++] = mat[i][j];
                flag = 0;
            } else {
                pn[n++] = mat[i][j];
                flag = 1;
            }
        }
    }
    *bianche_pp = pb;
    *nere_pp = pn;
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}


void stampa(int *vet, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d ", vet[i]);
    }
    printf("\n");
}