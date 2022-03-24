#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define DBG 1

Graph readFile(char *filename);


int main(void) {
    char file_name[MAXCHAR], id_elab1[MAXCHAR], id_elab2[MAXCHAR], id_elab3[MAXCHAR];
    Graph G;
    int comando, flag_fine=1, flagLADJ=0;

    printf("Inserire il nome del file:\n");
    scanf("%s", file_name);
    G = readFile(file_name);

#if DBG
printfAll(G);
#endif

    do {
        printf("Esegui un comando digitando un numero:\n");
        printf("\t1 = Stampa vertici (ordine alfabetico)\n");
        printf("\t2 = Genera lista di adiacenza\n");
        printf("\t3 = Sottografo completo (madj)\n");
        printf("\t4 = Sottografo completo (ladj)\n");
        printf("\t0 = Termina\n");

        scanf("%d", &comando);

        switch(comando){
            case 0: {
                flag_fine = 0;
            } break;

            case 1: {
                vertexSort(G);
            } break;

            case 2: {
                if (flagLADJ == 0){
                    LADJfill(G);
                    flagLADJ = 1;
                }
            } break;

            case 3: {
                printf("Scrivere il nome dei 3 vertici da considerare:\n");
                scanf("%s %s %s", id_elab1, id_elab2, id_elab3);
                if (MADJsubGRAPHcheck(G, id_elab1, id_elab2) &&
                    MADJsubGRAPHcheck(G, id_elab2, id_elab3) &&
                    MADJsubGRAPHcheck(G, id_elab1, id_elab3))
                {
                    printf("I 3 vertici formano un sottografo completo\n");
                }
                else
                    printf("I 3 vertici non formano un sottografo completo\n");
                printf("\n");
            } break;

            case 4: {
                printf("Scrivere il nome dei 3 vertici da considerare:\n");
                scanf("%s %s %s", id_elab1, id_elab2, id_elab3);

                if (flagLADJ == 0){
                    LADJfill(G);
                    flagLADJ = 1;
                }

                if (LADJsubGRAPHcheck(G, id_elab1, id_elab2) &&
                    LADJsubGRAPHcheck(G, id_elab2, id_elab3) &&
                    LADJsubGRAPHcheck(G, id_elab1, id_elab3))
                {
                    printf("I 3 vertici formano un sottografo completo\n");
                }
                else
                    printf("I 3 vertici non formano un sottografo completo\n");
                printf("\n");
            } break;

            default: {
                printf("Comando non valido\n");
            } break;
        }
    } while (flag_fine == 1);

    GRAPHfree(G);
    return EXIT_SUCCESS;
}


Graph readFile(char *filename) {
    int num=0, i=0, id1, id2, wt;
    char id_elab1[MAXCHAR], id_elab2[MAXCHAR];
    char id_rete1[MAXCHAR], id_rete2[MAXCHAR];
    Graph G;
    FILE *fp;

    /* prima lettura del file per calcolare il numero di archi */
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp, "%*s %*s %*s %*s %*d") != EOF) num++;
    /* sovrastimo il numero di archi e alloco la tabella di simboli (vettore) */
    G = GRAPHinit(num*2);
    fclose(fp);



    /* seconda lettura del file per salvare i dati nella tabella di simboli */
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp, "%s %s %s %s %d",
                  id_elab1, id_rete1, id_elab2, id_rete2, &wt) != EOF)
    {
        id1 = IDsearch(G, id_elab1, id_rete1, &i);
        id2 = IDsearch(G, id_elab2, id_rete2, &i);
        GRAPHinsertE(G, id1, id2, wt);
    }

    GRAPHrealloc(G, i);

    fclose(fp);
    return G;
}