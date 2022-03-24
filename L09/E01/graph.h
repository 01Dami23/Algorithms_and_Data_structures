#ifndef LAB_9_GRAPH_H
#define LAB_9_GRAPH_H

#include "ST.h"

typedef struct edge {
    int v; int w; int wt;
} Edge;

typedef struct graph *Graph;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHedges(Graph G, Edge *a);
int GRAPHnumV(Graph G);
int GRAPHnumE(Graph G);
void GRAPHdfs(Graph G, int id, int *flag_back_p);

void comb_sempl(int pos, int *val, int *sol, int n, int k, int start, Edge *a, int *flag_back_p, int *flag_found_p, Graph G, int *tot_wt_p, int *max_sol);
Graph GRAPHfromEdges(Graph G, int *sol, Edge *a, int E);

void DAGts(Graph D, Graph G, int *topological_ord_vett);

int GRAPH_nodiSorgente(Graph D, int *nodi_sorgente, Edge *b);
void GRAPH_DAGmaxPath(Graph D, Graph G, int id, int *topological_ord_vett);

void printfAll_wrapper(Graph G, Graph G1, int flag_stdout);

#endif