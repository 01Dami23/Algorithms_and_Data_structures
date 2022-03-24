#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


int main(void) {
    char fileName[MAXCHAR];
    Graph G, DAG;
    Edge *a, *b;
    int flag_back = 0, flag_found = 0, tot_wt = 0, flag_newDAG = 0;
    int i, k, E, *val, *sol, *max_sol, *topological_ord_vett, num_nodi_sorgente, *nodi_sorgente;


    printf("Inserire il nome del file:\n");
    scanf("%s", fileName);

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error opening input file\n");
        return(EXIT_FAILURE);
    }

    G = GRAPHload(fp);
    fclose(fp);

    printf("< Grafo di partenza >\n\n");
    printfAll_wrapper(G, G, 1);
    printf("-------------------------------------------------------\n\n");

    GRAPHdfs(G, 0, &flag_back);
    if (flag_back == 0) {
        printf("-------------------Il grafo e' un DAG------------------\n\n");
        printf("-------------------------------------------------------\n\n");
        DAG = G;
    }
    else {
        flag_newDAG = 1;

        a = (Edge *) malloc(GRAPHnumE(G) * sizeof(Edge));
        checkErroreAllocazione(a);
        GRAPHedges(G, a);

        E = GRAPHnumE(G);
        val = malloc(E * sizeof(int));
        checkErroreAllocazione(val);
        sol = malloc(E * sizeof(int));
        checkErroreAllocazione(sol);
        max_sol = malloc(E * sizeof(int));
        checkErroreAllocazione(max_sol);
        flag_back = 0;

        for (i=0; i < E; i++) {
            val[i] = i;
        }

        /* TASK 1 */
        /* faccio le combinazioni semplici sugli archi fino a quando trovo uno o piu insiemi di archi il cui grafo forma un DAG */
        for (k=GRAPHnumE(G)-1; k >= GRAPHnumV(G)-1 && flag_found == 0; k--) {
            comb_sempl(0, val, sol, E, k, 0, a, &flag_back, &flag_found, G, &tot_wt, max_sol);
        }
        printf("-------------------------------------------------------\n\n");

        /* TASK 2 */
        /* salvo e stampo il DAG ottenuto rimuovendo il set di archi a peso massimo */
        DAG = GRAPHfromEdges(G, max_sol, a, k+1);
        printf("< DAG ottenuto rimuovendo il set di archi con il peso maggiore dal grafo di partenza >\n\n");
        printfAll_wrapper(G, DAG, 1);

        free(a); free(val); free(sol); free(max_sol);
    }

    /* stampo il DAG in ordine topologico e mi salvo il vettore di vertici in ordine topologico */
    topological_ord_vett = malloc(GRAPHnumV(DAG) * sizeof(int));
    checkErroreAllocazione(topological_ord_vett);
    DAGts(DAG, G, topological_ord_vett);

    /* TASK 3 */
    b = (Edge *) malloc(GRAPHnumE(DAG) * sizeof(Edge));
    checkErroreAllocazione(b);
    GRAPHedges(DAG, b);

    /* individuo e salvo i vertici (nodi) sorgente in un vettore di interi */
    nodi_sorgente = malloc(GRAPHnumV(DAG) * sizeof(int));
    checkErroreAllocazione(nodi_sorgente);

    num_nodi_sorgente = GRAPH_nodiSorgente(DAG, nodi_sorgente, b);

    /* per ogni nodo sorgente cerco il cammino massimo per raggiungere tutti gli altri nodi (raggiungibili) */
    for (i=0; i < num_nodi_sorgente; i++) {
        GRAPH_DAGmaxPath(DAG, G, nodi_sorgente[i], topological_ord_vett);
    }

    free(b); free(topological_ord_vett); free(nodi_sorgente);
    GRAPHfree(G);
    if (flag_newDAG) {
        GRAPHfree(DAG);
    }

    return EXIT_SUCCESS;
}