#ifndef LAB_7_GRAPH_H
#define LAB_7_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "ST.h"

typedef struct edge {
    int v; int w; int wt;
} Edge;
typedef struct graph *Graph;

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHrealloc(Graph G, int V);
void IDinsert(Graph G, char *id_elab, char *id_rete, int i);
int IDsearch(Graph G, char *id_elab, char *id_rete, int *i_p);
void vertexSort(Graph G);
void LADJfill(Graph G);
int MADJsubGRAPHcheck(Graph G, char *id_elab1, char *id_elab2);
int LADJsubGRAPHcheck(Graph G, char *id_elab1, char *id_elab2);

void printfAll(Graph G);
void EDGEmergeSort(Edge *A, int N, tab table);

#endif