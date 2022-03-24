#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 101
#define MAXELEM 5
#define MAXDIAG 3
#define DD 11
#define DP 33

#define filein "elementi.txt"

typedef struct elemento {
    char nome[MAXCHAR];
    int tipologia;
    int ingresso;
    int uscita;
    int iniziale;
    int finale;
    float punteggio;
    int difficolta;
} elemento;

typedef struct vincoli {
    int vett_elem_acr[MAXDIAG];
    int elem_acr_in_sequenza;
    int elem_acr_avanti;
    int elem_acr_indietro;
} vincoli;

elemento *readElem(int *num_elem_p);
void freeElem(elemento *elem_p);
int diag(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM], int *vett_diff, int num_elem, int *tot_p);
void addElem(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM], int *vett_diff, int num_elem, int i, int d,
             int *orientamento, int *found, int *tot_p, vincoli *regole_p, int *vett_app);
void appetibilita(int *vett_app, elemento *vett_elem_cpy, int num_elem, int flag_posizione,
                  int orientamento, int num_diag, vincoli *regole_p, int *vett_diff);
void stampaSol(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM]);

void merge(elemento *A, elemento *B, int *vett_app, int *v_app_cpy, int l, int m, int r);
void mergeSortR(elemento *A, elemento *B, int *vett_app, int *v_app_cpy, int l, int r);
void mergeSort(elemento *A, int *vett_app, int N);


int main(void) {
    elemento *vett_elem;
    int i, j, num_elem, vett_diff[MAXDIAG] = {0};
    int sol_mat[MAXDIAG][MAXELEM];
    int flag, tot=0;

    for (i=0; i<MAXDIAG; i++) {
        for (j=0; j<MAXELEM; j++) {
            sol_mat[i][j] = -1;
        }
    }

    vett_elem = readElem(&num_elem);

    flag = diag(vett_elem, sol_mat, vett_diff, num_elem, &tot);


    if (flag == 1) {
        stampaSol(vett_elem, sol_mat);
    }
    else {
        printf("Soluzione non trovata\n");
    }

    freeElem(vett_elem);
    return EXIT_SUCCESS;
}


elemento *readElem(int *num_elem_p) {
    elemento *elem_p;
    FILE *fp = fopen(filein, "r");
    int num, i;

    if (fp == NULL) {
        printf("Error opening input file");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &num);
    *num_elem_p = num;

    elem_p = (elemento *) malloc(num * sizeof(elemento));
    if (elem_p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<num; i++) {
        fscanf(fp, "%s %d %d %d %d %d %f %d",
               elem_p[i].nome,
               &elem_p[i].tipologia,
               &elem_p[i].ingresso,
               &elem_p[i].uscita,
               &elem_p[i].iniziale,
               &elem_p[i].finale,
               &elem_p[i].punteggio,
               &elem_p[i].difficolta);
    }

    fclose(fp);
    return elem_p;
}


void freeElem(elemento *elem_p) {
    free(elem_p);
}


int diag(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM], int *vett_diff, int num_elem, int *tot_p) {
    int i, *vett_app;   /* vettore di indici per ordinare gli elementi in base alla loro appetibilita */
    int orientamento[MAXDIAG] = {1, 1, 1}, found[MAXDIAG] = {1, 1, 1};
    vincoli regole;

    /* inizializzo i vincoli (regole) */
    regole.vett_elem_acr[0] = regole.vett_elem_acr[1] = regole.vett_elem_acr[2] = regole.elem_acr_in_sequenza =
            regole.elem_acr_avanti = regole.elem_acr_indietro = 0;

    /* flag_posizione: ha come valore il numero dell'elemento che sto inserendo, ad Es.
     * se sto inserendo il terzo elem. sulla diag1 flag_posizione = 3 */

    vett_app = (int *) malloc(num_elem * sizeof(int));
    if (vett_app == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    /* itero tante volte quanti elem. posso mettere al massimo */
    for (i=0; i<MAXELEM && *tot_p < DP; i++) {

        /* per diag1 */
        addElem(elem_p, sol_matrix, vett_diff, num_elem, i, 0, orientamento, found, tot_p, &regole, vett_app);

        /* per diag2 */
        /* se sono al terzo elemento inserisco il secondo elemento di diag3 */
        if (i == 2)
            addElem(elem_p, sol_matrix, vett_diff, num_elem, i-1, 2, orientamento, found, tot_p, &regole, vett_app);
        else
            addElem(elem_p, sol_matrix, vett_diff, num_elem, i, 1, orientamento, found, tot_p, &regole, vett_app);

        /* per diag3 */
        /* se sono al secondo elemento prima inserisco il terzo elemento di diag2 (per avere piu spazio per un elemento indietro) */
        if (i == 1)
            addElem(elem_p, sol_matrix, vett_diff, num_elem, i+1, 1, orientamento, found, tot_p, &regole, vett_app);
        else
            addElem(elem_p, sol_matrix, vett_diff, num_elem, i, 2, orientamento, found, tot_p, &regole, vett_app);

    }
    free(vett_app);

    /* se rispetto tutti i vincoli ho soluzione ammissibile altrimenti dico al main che la soluzione non risulta valida */
    if (regole.vett_elem_acr[0] == regole.vett_elem_acr[1] == regole.vett_elem_acr[2] == 1 &&
        regole.elem_acr_avanti == 1 && regole.elem_acr_indietro == 1 &&
        regole.elem_acr_in_sequenza == 1)
        return 1;
    else
        return 0;
}


void addElem(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM], int *vett_diff, int num_elem, int i, int d,
             int *orientamento, int *found, int *tot_p, vincoli *regole_p, int *vett_app) {
    int j, index;
    elemento *vett_elem_cpy;

    /* creo un vettore copia del vettore di elementi in cui mi salvo i punteggi cambiati in base alla priorita */
    vett_elem_cpy = (elemento *) malloc(num_elem * sizeof(elemento));
    if (vett_elem_cpy == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    for (j=0; j<num_elem; j++) {
        vett_elem_cpy[j] = elem_p[j];
    }

    if (found[d] == 1) {
        found[d] = 0;

        /* se sono al secondo elemento della seconda diagonale e non ho ancora messo sia un elem avanti che uno indietro l'obiettivo
         * sara di mettere un elem indietro oppure cambiare orientamento */

        appetibilita(vett_app, vett_elem_cpy, num_elem, i+1, orientamento[d], d, regole_p, vett_diff);

        for (j=0; j<num_elem && vett_elem_cpy[j].punteggio != -1 && found[d] == 0; j++) {
            index = vett_app[j];
            if (elem_p[index].difficolta <= (DD - vett_diff[d]) && elem_p[index].difficolta <= (DP - *tot_p)) {

                found[d] = 1;

                vett_diff[d] += elem_p[index].difficolta;
                *tot_p += elem_p[index].difficolta;

                sol_matrix[d][i] = index;
                orientamento[d] = elem_p[index].uscita;
            }
        }

        if (found[d] == 1) {
            if (elem_p[sol_matrix[d][i]].tipologia == 1 || elem_p[sol_matrix[d][i]].tipologia == 2) {
                regole_p->vett_elem_acr[d] = 1;
                if (i > 0) {
                    if (elem_p[sol_matrix[d][i-1]].tipologia == 1 || elem_p[sol_matrix[d][i-1]].tipologia == 2)
                        regole_p->elem_acr_in_sequenza = 1;
                }
            }
            /* controllo sul vincolo di avere almeno 1 elem acr in avanti e uno indietro */
            if (elem_p[sol_matrix[d][i]].tipologia == 2) {
                regole_p->elem_acr_avanti = 1;
            }
            else if (elem_p[sol_matrix[d][i]].tipologia == 1) {
                regole_p->elem_acr_indietro = 1;
            }
        }

        /* se l'elemento e finale metto found a 0 e in quella diagonale non aggiungo piu elementi */
        if (elem_p[index].finale == 1)
            found[d] = 0;
    }
    free(vett_elem_cpy);
}


void appetibilita(int *vett_app, elemento *vett_elem_cpy, int num_elem, int flag_posizione,
                  int orientamento, int num_diag, vincoli *regole_p, int *vett_diff) { /* orientamento = 1 in avanti e = 0 all'indietro */
    int i;

    /* inizializzo il vettore delle appetibilita con gli indici poi da ordinare in base al punteggio cambiato */
    for (i=0; i<num_elem; i++) {
        vett_app[i] = i;
    }

    for (i=0; i<num_elem; i++) {

        /* elem acrobatici */
        if (vett_elem_cpy[i].tipologia == 2 || vett_elem_cpy[i].tipologia == 1)
            vett_elem_cpy[i].punteggio = (float)1000.0*(1+vett_elem_cpy[i].punteggio)/(float)vett_elem_cpy[i].difficolta;

        /* elem di transizione */
        if (vett_elem_cpy[i].tipologia == 0) {
            vett_elem_cpy[i].punteggio = (float)10.0*(1+vett_elem_cpy[i].punteggio)/(float)vett_elem_cpy[i].difficolta;
        }

        /* se sono all'inizio ci sono alcuni elementi che non posso fare */
        if (flag_posizione == 1){
            /* elem acr non iniziale o elem acr finale o elem acr all'indietro (priorita negativa all'inizio) */
            if (vett_elem_cpy[i].iniziale == 1 || vett_elem_cpy[i].finale == 1 || vett_elem_cpy[i].tipologia == 1) {
                vett_elem_cpy[i].punteggio = -1;
            }
        }

        /* se l'elemento ha direzione di ingresso diversa da quella corrente ha priorita negativa */
        if (vett_elem_cpy[i].ingresso != orientamento)
            vett_elem_cpy[i].punteggio = -1;

        /* se sono alla seconda diag seconda posizione cerco di mettere un elem acr indietro oppure mi giro indietro e poi lo metto */
        if (regole_p->elem_acr_indietro == 0 && num_diag == 1 && flag_posizione == 2) {
            if (vett_elem_cpy[i].tipologia == 0 && orientamento == 1 && vett_elem_cpy[i].uscita == 0) {
                if (vett_elem_cpy[i].tipologia == 0)
                    vett_elem_cpy[i].punteggio *= 1000;
                else
                    vett_elem_cpy[i].punteggio *= 10;
            }
        }

        /* gestione per gli elementi finali (decido se metterli oppure no) */
        if (vett_elem_cpy[i].finale == 1 && (DD-vett_diff[num_diag]-vett_elem_cpy[i].difficolta) > 2)
            vett_elem_cpy[i].punteggio = -1;
    }

    /* devo ordinare gli indici nel vettore di appetibilita in base al punteggio dell'elem associato a quell'indice (ordine decrescente) */
    mergeSort(vett_elem_cpy, vett_app, num_elem);
}


void merge(elemento *A, elemento *B, int *vett_app, int *v_app_cpy, int l, int m, int r) {
    int i, j, k;

    i = l;
    j = m+1;

    for (k=l; k<=r; k++) {
        if (i > m) {
            v_app_cpy[k] = vett_app[j];
            B[k] = A[j++];
        }
        else if (j > r) {
            v_app_cpy[k] = vett_app[i];
            B[k] = A[i++];
        }
        else if ((A[i].punteggio > A[j].punteggio) || (A[i].punteggio == A[j].punteggio)) {
            v_app_cpy[k] = vett_app[i];
            B[k] = A[i++];
        }
        else {
            v_app_cpy[k] = vett_app[j];
            B[k] = A[j++];
        }
    }
    for (k=l; k<=r; k++) {
        vett_app[k] = v_app_cpy[k];
        A[k] = B[k];
    }
}


void mergeSortR(elemento *A, elemento *B, int *vett_app, int *v_app_cpy, int l, int r) {
    int m;

    if (r <= l)
        return;
    m = (l + r)/2;
    mergeSortR(A, B, vett_app, v_app_cpy, l, m);
    mergeSortR(A, B, vett_app, v_app_cpy, m+1, r);
    merge(A, B, vett_app, v_app_cpy, l, m, r);
}


void mergeSort(elemento *A, int *vett_app, int N) {
    int l=0, r=N-1;
    elemento *B = (elemento *) malloc(N * sizeof(elemento));
    int *v_app_cpy = (int *) malloc(N * sizeof(int));

    mergeSortR(A, B, vett_app, v_app_cpy, l, r);

    free(B);
    free(v_app_cpy);
}


void stampaSol(elemento *elem_p, int sol_matrix[MAXDIAG][MAXELEM]) {
    int i, j, flag_bonus=0;
    float tot=0, vett_pt[MAXDIAG]={0};

    for (i=0; i<MAXDIAG; i++){
        for (j=0; j<MAXELEM && sol_matrix[i][j] != -1; j++) {
            vett_pt[i] += elem_p[sol_matrix[i][j]].punteggio;
        }
    }
    if (elem_p[sol_matrix[i-1][j-1]].punteggio >= 8) {
        vett_pt[i-1] = vett_pt[i-1] * (float)1.5;
        flag_bonus=  1;
    }

    printf("DD = %d DP = %d\n", DD, DP);

    for (i=0; i<MAXDIAG; i++) {

        /* se ho avuto il bonus, i pt di diag3 sono stati moltiplicati per 1.5
         * quindi prima di stampare quelli effettivi devo dividere */
        if (i+1 == 3 && flag_bonus)
            printf("DIAG #%d > %.3f * 1.5 (BONUS)\n", i+1, vett_pt[i]/1.5);
        else
            printf("DIAG #%d > %.3f\n", i+1, vett_pt[i]);

        for (j=0; j<MAXELEM && sol_matrix[i][j]!=-1; j++) {
            printf("%s ", elem_p[sol_matrix[i][j]].nome);
        }

        tot += vett_pt[i];
        printf("\n");
    }
    printf("TOT = %.3f", tot);
    printf("\n");
}