#include "graph.h"

typedef struct node *link;
struct node { int v; int wt; link next; } ;

struct graph {
    int V; int E; int **madj; link *ladj; tab table; link z;
};

static int **MATRIXint(int r, int c, int val);
static Edge EDGEcreate(int v, int w, int wt);
static void MADJinsertE(Graph G, Edge e);
static link NEW(int v, int wt, link next);
static void LADJinsertE(Graph G, Edge e);


Graph GRAPHinit(int est) {
    Graph G = malloc(sizeof *G);
    G->V = est;
    G->E = 0;

    G->madj = MATRIXint(est, est, 0);

    G->z = NEW(-1, -1, NULL);
    G->ladj = NULL;

    G->table = STinit(est);
    return G;
}


void GRAPHfree(Graph G) {
    int i, v;
    link t, next;

    for (i=0; i<G->V; i++)
        free(G->madj[i]);
    free(G->madj);

    if (G->ladj != NULL) {
        for (v=0; v < G->V; v++)
            for (t=G->ladj[v]; t != G->z; t = next) {
                next = t->next;
                free(t);
            }
        free(G->ladj);
    }
    free(G->z);

    STfree(G->table);
    free(G);
}


void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    MADJinsertE(G, EDGEcreate(id1, id2, wt));
}


/* funzione che fa da tramite per l'inserimento in tabella di simboli
 * di un vertice (essendo graph un ADT di I classe) */
void IDinsert(Graph G, char *id_elab, char *id_rete, int i) {
    STinsert(G->table, id_elab, id_rete, i);
}


int IDsearch(Graph G, char *id_elab, char *id_rete, int *i_p) {
    int index = STsearch(G->table, id_elab, *i_p);
    if (index == -1) {
        IDinsert(G, id_elab, id_rete, *i_p);
        (*i_p)++;
        index = STsearch(G->table, id_elab, *i_p);
    }
    return index;
}


/* per cambiare il num di vertici nel grafo quando li ho contati e so quanti
 * sono e fare riallocazione su matrice adiacenza e tabella di simboli */
void GRAPHrealloc(Graph G, int V) {

    for (int i=V; i < G->V; i++)
        free(G->madj[i]);

    G->madj = (int **) realloc(G->madj, V * sizeof(int *));
    checkErroreAllocazione(G->madj);
    for (int i=0; i < V; i++) {
        G->madj[i] = (int *) realloc(G->madj[i], V * sizeof(int));
        checkErroreAllocazione(G->madj[i]);
    }
    
    STrealloc(G->table, V);
    G->V = V;
}


void vertexSort(Graph G) {
    int i, j, k, m, v, w, wt;
    tab sortedTable = STinit(G->V);
    char id_elab1[MAXCHAR], id_elab2[MAXCHAR];
    Edge *edges_array;

    STtablecpy(sortedTable, G->table, G->V);

    STmergeSort(sortedTable, G->V);

    for (i=0; i < G->V; i++) {
        m = 0;
        strcpy(id_elab1, STsearchByIndex(sortedTable, i));
        printf("Vertice #%d: %s\n", i, id_elab1);

        edges_array = malloc(G->V * sizeof(Edge));
        j = STsearch(G->table, id_elab1, G->V);
        for (k=0; k < G->V; k++) {
            if (G->madj[j][k] != 0)
                edges_array[m++] = EDGEcreate(j, k, G->madj[j][k]);
        }

        EDGEmergeSort(edges_array, m, G->table);

        for (k=0; k < m; k++) {
            v = edges_array[k].v;
            w = edges_array[k].w;
            wt = edges_array[k].wt;
            strcpy(id_elab1, STsearchByIndex(G->table, v));
            strcpy(id_elab2, STsearchByIndex(G->table, w));

            printf("\t%s %s %d\n", id_elab1, id_elab2, wt);
        }
        printf("\n");
        free(edges_array);
    }
    STfree(sortedTable);
}


void LADJfill(Graph G) {
    int i, j, v;
    Edge e;

    G->ladj = malloc(G->V * sizeof(link));
    for (v = 0; v < G->V; v++)
        G->ladj[v] = G->z;

    for (i=0; i < G->V; i++) {
        for (j=0; j < G->V; j++) {
            if (G->madj[i][j] != 0) {
                e = EDGEcreate(i, j, G->madj[i][j]);
                LADJinsertE(G, e);
            }
        }
    }
}


int MADJsubGRAPHcheck(Graph G, char *id_elab1, char *id_elab2) {
    int i, j;

    i = STsearch(G->table, id_elab1, G->V);
    j = STsearch(G->table, id_elab2, G->V);

    if (G->madj[i][j] != 0)
        return 1;
    else
        return 0;
}


int LADJsubGRAPHcheck(Graph G, char *id_elab1, char *id_elab2) {
    int i, j, flag=0;
    link t, next;

    i = STsearch(G->table, id_elab1, G->V);
    j = STsearch(G->table, id_elab2, G->V);

    for (t=G->ladj[i]; t != G->z; t=next) {
        next = t->next;

        if (j == t->v) {
            flag++;
            next = G->z;
        }
    }

    for (t=G->ladj[j]; t != G->z; t=next) {
        next = t->next;

        if (i == t->v) {
            flag++;
            next = G->z;
        }
    }

    if (flag == 2)
        return 1;
    else
        return 0;
}


/* funzione per inizializzare la matrice delle adiacenze */
static int **MATRIXint(int r, int c, int val) {
    int i, j;
    int **t = malloc(r * sizeof(int *));
    for (i=0; i < r; i++)
        t[i] = malloc(c * sizeof(int));
    for (i=0; i < r; i++)
        for (j=0; j < c; j++)
            t[i][j] = val;
    return t;
}


/* funzione per creare un arco */
static Edge EDGEcreate(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}


static void MADJinsertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt =e.wt;
    if (G->madj[v][w] == 0)
        G->E++;
    G->madj[v][w] = wt;
    G->madj[w][v] = wt;
}


static link NEW(int v, int wt, link next) {
    link x = malloc(sizeof *x);
    x->v = v; x->wt = wt; x->next = next;
    return x;
}


static void LADJinsertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;
    G->ladj[v] = NEW(w, wt, G->ladj[v]);
}


/* funzione di per vedere se i dati sono stati inseriti correttamente */
void printfAll(Graph G) {
    int i, j;
    FILE *fout = fopen("output.txt", "w");

    if (fout == NULL)
        exit(EXIT_FAILURE);

    fprintf(fout, "Numero di vertici: %d\n", G->V);
    fprintf(fout, "Numero di archi: %d\n", G->E);
    fprintf(fout, "\n");
    fprintf(fout, "Tabella di simboli:\n");
    for (i = 0; i < G->V; i++) {
        fprintf(fout, "Index <%d>    ", i);
        printST(fout, G->table, i);
    }
    fprintf(fout, "\n");

    fprintf(fout, "#%.2d ", 0);
    for (i=0; i < G->V; i++)
        fprintf(fout, "#%.2d ", i);
    fprintf(fout, "\n");
    for (i=0; i < G->V; i++) {
        fprintf(fout, "#%.2d ", i);
        for (j=0; j < G->V; j++) {
            fprintf(fout, "%.3d ", G->madj[i][j]);
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
}


void EDGEmerge(Edge *A, Edge *B, int l, int m, int r, tab table) {
    int i, j, k, v1, w1, v2, w2;

    i = l;
    j = m+1;

    for (k=l; k<=r; k++) {

        v1 = A[i].v; w1 = A[i].w;
        v2 = A[j].v; w2 = A[j].w;

        if (i > m) {
            B[k] = A[j++];
        }
        else if (j > r) {
            B[k] = A[i++];
        }
        else if (strcmp(STsearchByIndex(table, v1), STsearchByIndex(table, v2)) < 0)
        {
            B[k] = A[i++];
        }
        else if (strcmp(STsearchByIndex(table, v1), STsearchByIndex(table, v2)) == 0 &&
                 (strcmp(STsearchByIndex(table, w1), STsearchByIndex(table, w2)) < 0 ||
                 strcmp(STsearchByIndex(table, w1), STsearchByIndex(table, w2)) == 0))
        {
            B[k] = A[i++];
        }
        else {
            B[k] = A[j++];
        }
    }
    for (k=l; k<=r; k++) {
        A[k] = B[k];
    }
}


void EDGEmergeSortR(Edge *A, Edge *B, int l, int r, tab table) {
    int m;

    if (r <= l)
        return;
    m = (l + r)/2;
    EDGEmergeSortR(A, B, l, m, table);
    EDGEmergeSortR(A, B, m+1, r, table);
    EDGEmerge(A, B, l, m, r, table);
}


void EDGEmergeSort(Edge *A, int N, tab table) {
    int l=0, r=N-1;
    Edge *B = malloc(N * sizeof(Edge));
    EDGEmergeSortR(A, B, l, r, table);

    free(B);
}