#include "graph.h"
#include <limits.h>

typedef struct node *link;
struct node {int v; int wt; link next;};

struct graph {
    int V; int E; link *ladj; tab table; link z;
};

static void dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st, int *flag_back_p);
static Edge EDGEcreate(int v, int w, int wt);
static link NEW(int v, int wt, link next);
static void LADJinsertE(Graph G, Edge e);
static int printMinEdgeSet(tab table, Edge *a, int *sol, int E, int k);
static void printfAll(Graph G, tab table, int flag_stdout);
static void TSdfsR(Graph D, int v, int *ts, int *pre, int *time);


Graph GRAPHinit(int V) {
    int v;
    Graph G = malloc(sizeof *G);
    checkErroreAllocazione(G);
    G->V = V;
    G->E = 0;

    G->z = NEW(-1, -1, NULL);
    G->ladj = malloc(G->V * sizeof(link));
    checkErroreAllocazione(G->ladj);
    for (v = 0; v < G->V; v++)
        G->ladj[v] = G->z;

    G->table = STinit(V);
    return G;
}


void GRAPHfree(Graph G) {
    int v;
    link t, next;

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


Graph GRAPHload(FILE *fin) {
    int V, i, id1, id2, wt;
    char label1[MAXCHAR], label2[MAXCHAR];
    Graph G;
    fscanf(fin, "%d", &V);
    G = GRAPHinit(V);
    for (i=0; i<V; i++) {
        fscanf(fin, "%s", label1);
        STinsert(G->table, label1, i);
    }
    while(fscanf(fin,"%s %s %d", label1, label2, &wt) == 3) {
        id1 = STsearch(G->table, label1);
        id2 = STsearch(G->table, label2);
        if (id1 >= 0 && id2 >=0)
            GRAPHinsertE(G, id1, id2, wt);
    }
    return G;
}


void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    LADJinsertE(G, EDGEcreate(id1, id2, wt));
}


void  GRAPHedges(Graph G, Edge *a) {
    int v, E = 0;
    link t;
    for (v=0; v < G->V; v++)
        for (t=G->ladj[v]; t != G->z; t = t->next)
            a[E++] = EDGEcreate(v, t->v, t->wt);
}


int GRAPHnumV(Graph G) {
    return G->V;
}


int GRAPHnumE(Graph G) {
    return G->E;
}


/* attraverso una DFS sul grafo che trovo con le combinazioni semplici
 * verifico se e' aciclico o meno */
static void dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st, int *flag_back_p) {
    link t;
    int v, w = e.w;
    st[e.w] = e.v;
    pre[w] = (*time)++;
    for (t = G->ladj[w]; t != G->z; t = t->next)
        if (pre[t->v] == -1)
            dfsR(G, EDGEcreate(w, t->v, 0), time, pre, post, st, flag_back_p);
        else {
            v = t->v;
            if (post[v] == -1) {
                (*flag_back_p)++;
            }
        }
    post[w] = (*time)++;
}

void GRAPHdfs(Graph G, int id, int *flag_back_p) {
    int v, time=0, *pre, *post, *st;

    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));
    if ((pre == NULL) || (post == NULL) || (st == NULL))
        return;

    for (v=0; v < G->V; v++) {
        pre[v] = -1;
        post[v] = -1;
        st[v] =  -1;
    }

    dfsR(G, EDGEcreate(id,id,0), &time, pre, post, st, flag_back_p);

    for (v=0; v < G->V; v++) {
        if (pre[v] == -1)
            dfsR(G, EDGEcreate(v, v, 0), &time, pre, post, st, flag_back_p);
    }

    free(pre); free(post); free(st);
}


void comb_sempl(int pos, int *val, int *sol, int n, int k, int start, Edge *a, int *flag_back_p, int *flag_found_p, Graph G, int *tot_wt_p, int *max_sol) {
    int i, num;
    Graph newG;

    if (pos >= k) {
        newG = GRAPHfromEdges(G, sol, a, k);

        GRAPHdfs(newG, 0, flag_back_p);
        if (*flag_back_p == 0) {
            num = printMinEdgeSet(G->table, a, sol, G->E, k);
            if (num > *tot_wt_p) {
                *tot_wt_p = num;
                for (int j=0; j < k; j++)
                    max_sol[j] = sol[j];
            }
            (*flag_found_p)++;
        }
        *flag_back_p = 0;
        GRAPHfree(newG);
        return;
    }

    for (i=start; i < n; i++) {
        sol[pos] = val[i];
        comb_sempl(pos+1, val, sol, n, k, i+1, a, flag_back_p, flag_found_p, G, tot_wt_p, max_sol);
    }
}


Graph GRAPHfromEdges(Graph G, int *sol, Edge *a, int E) {
    Graph newG = GRAPHinit(G->V);
    int id1, id2, wt;

    for (int j=0; j < E; j++) {
        id1 = a[sol[j]].v;
        id2 = a[sol[j]].w;
        wt = a[sol[j]].wt;
        GRAPHinsertE(newG, id1, id2, wt);
    }
    return newG;
}


static Edge EDGEcreate(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}


static link NEW(int v, int wt, link next) {
    link x = malloc(sizeof *x);
    checkErroreAllocazione(x);
    x->v = v; x->wt = wt; x->next = next;
    return x;
}


static void LADJinsertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;
    G->ladj[v] = NEW(w, wt, G->ladj[v]);
    G->E++;
}


/* ritorna il peso del set di archi */
static int printMinEdgeSet(tab table, Edge *a, int *sol, int E, int k) {
    int i, j, flag, tot_wt = 0;

    printf("Set di %d archi eliminabile per creare un DAG:\n", E-k);

    for (i=0; i < E; i++) {
        flag = 0;
        for (j=0; j < k && flag == 0; j++) {
            if (i == sol[j])
                flag = 1;
        }
        if (flag == 0) {
            // stampo l'arco che risulta escluso dal sol
            printf("\t- Vertice 1: <%s>    Vertice 2: <%s>    Peso: <%d>\n",
                   STsearchByIndex(table, a[i].v),
                   STsearchByIndex(table, a[i].w),
                   a[i].wt);
            tot_wt += a[i].wt;
        }
    }
    printf("\n");
    return tot_wt;
}


/* funzione per vedere se i dati sono stati inseriti correttamente nel grafo */
static void printfAll(Graph G, tab table, int flag_stdout) {
    int i;
    link next;

    FILE *fout;
    if (flag_stdout == 1)
        fout = stdout;
    else {
        fout = fopen("output.txt", "w");
        if (fout == NULL)
            exit(EXIT_FAILURE);
    }

    fprintf(fout, "Numero di vertici: %d\n", G->V);
    fprintf(fout, "Numero di archi: %d\n", G->E);
    fprintf(fout, "\n");
    fprintf(fout, "Tabella di simboli:\n");
    for (i=0; i < G->V; i++) {
        fprintf(fout, "Index <%d>    ", i);
        printST(fout, table, i);
    }
    fprintf(fout, "\n");


    if (G->ladj != NULL) {
        for (int v=0; v < G->V; v++) {
            fprintf(fout, "Vertice di partenza: <%s>\n", STsearchByIndex(table, v));
            for (link t=G->ladj[v]; t != G->z; t = next) {
                next = t->next;
                fprintf(fout, "\tVertice: <%s>    Peso: <%d>\n", STsearchByIndex(table, t->v), t->wt);
            }
            fprintf(fout, "\n");
        }
    }
    if (flag_stdout != 1)
        fclose(fout);
}


void printfAll_wrapper(Graph G, Graph G1, int flag_stdout) {
    printfAll(G1, G->table, flag_stdout);
}


static void TSdfsR(Graph D, int v, int *ts, int *pre, int *time) {
    link t;
    pre[v] = 0;
    for (t = D->ladj[v]; t != D->z; t = t->next)
        if (pre[t->v] == -1)
            TSdfsR(D, t->v, ts, pre, time);
    ts[(*time)++] = v;
}


void DAGts(Graph D, Graph G, int *topological_ord_vett) {
    int v, time = 0, *pre, *ts, i=0;
    pre = malloc(D->V*sizeof(int));
    ts = malloc(D->V*sizeof(int));
    if ((pre == NULL) || (ts == NULL))
        return;

    for (v=0; v < D->V; v++) {
        pre[v] = -1;
        ts[v] = -1;
    }
    for (v=0; v <D->V; v++)
        if (pre[v]== -1)
            TSdfsR(D, v, ts, pre, &time);

    printf("Nodi del DAG in ordine topologico:\n");
    for (v=D->V-1; v >= 0; v--) {
        printf("%s ", STsearchByIndex(G->table, ts[v]));
        topological_ord_vett[i++] = ts[v];
    }
    printf("\n");
    free(pre); free(ts);
}


/* funzione per trovare quali sono i nodi sorgente nel DAG */
int GRAPH_nodiSorgente(Graph D, int *nodi_sorgente, Edge *b) {
    int i, j, k=0, found;

    for (i=0; i < D->V; i++) {
        found = 0;
        for (j=0; j < D->E; j++) {
            if (i == b[j].w)
                found = 1;
        }
        if (found == 0)
            nodi_sorgente[k++] = i;
    }
    return k;
}


/* opero sul DAG D, ma uso la tabella di simboli che ho gia' in G
 * e parto dal vertice id specificato */
void GRAPH_DAGmaxPath(Graph D, Graph G, int id, int *topological_ord_vett) {
    int i, start = -1, index;
    link t;
    int *d = malloc(D->V * sizeof(int));
    checkErroreAllocazione(d);

    /* inizializzo a -infinito tutte le distanze */
    for (i=0; i < D->V; i++) {
        d[i] = INT_MIN;
    }

    /* cerco il punto da cui iniziare nel vettore di vertici (topological_ord_vett)
     * in base all'id specificato */
    for (i=0; i < D->V && start == -1; i++) {
        if (id == topological_ord_vett[i]) {
            start = i;
            d[id] = 0;
        }
    }

    /* per tutti i vertici da start fino all'ultimo applico la relaxation inversa
     * ai loro archi uscenti */
    for (int v=start; v < D->V; v++) {
        index = topological_ord_vett[v];
        for (t=D->ladj[index]; t!=D->z ; t=t->next)
            if (d[t->v] < d[index] + t->wt) {
                d[t->v] = d[index] + t->wt;
            }
    }

    printf("\nDistanze massime dal nodo sorgente %s:\n", STsearchByIndex(G->table, id));
    for (i=0; i < G->V; i++) {
        if (d[i] < 0)
            printf("- nodo %s non raggiungibile da %s\n", STsearchByIndex(G->table, i), STsearchByIndex(G->table, id));
        else
            printf("d[%s] = %d \n", STsearchByIndex(G->table, i), d[i]);
    }
    free(d);
}