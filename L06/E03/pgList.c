#include "pgList.h"

typedef struct nodoPg_t *link;
struct nodoPg_t {
    pg_t pg;
    link next;
};

typedef struct pgList_s{
    link headPg;
    link tailPg;
    int nPg;
} pgList_s;


link newNode(pg_t pg, link next) {
    link x = malloc(sizeof *x);
    checkErroreAllocazione(x);
    x->pg = pg;
    x->next = next;
    return x;
}


pgList_t pgList_init(void) {
    pgList_t pgList;

    pgList = (pgList_t) malloc(sizeof(pgList_s));
    checkErroreAllocazione(pgList);

    pgList->headPg = pgList->tailPg = NULL;
    pgList->nPg = 0;

    return pgList;
}


void pgList_free(pgList_t pgList) {
    link x = pgList->headPg, temp;
    while(x != NULL) {
        temp = x;
        x = x->next;
        pg_clean(&temp->pg);
        free(temp);
    }
    free(pgList);
}


void pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg;
    int flag = 0;

    if (fp == NULL) {
        printf("Error opening pg file\n");
        exit(EXIT_FAILURE);
    }

    while (flag==0 && pg_read(fp, &pg) == 1)
    {
        pgList_insert(pgList, pg);
        /* se prendo da stdin mi aspetto un solo personaggio */
        if (fp == stdin)
            flag = 1;
    }
}


void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    int i;
    link x = pgList->headPg;

    for(i=0; i<pgList->nPg; i++) {
        pg_print(fp, &x->pg, invArray);
        x = x->next;
    }
    printf("\n\n");
}


void pgList_insert(pgList_t pgList, pg_t pg) {
    if (pgList->headPg == NULL) {
        pgList->headPg = pgList->tailPg = newNode(pg, NULL);
    }
    else {
        pgList->tailPg->next = newNode(pg, NULL);
        pgList->tailPg = pgList->tailPg->next;
    }
    pgList->nPg++;
}


void pgList_remove(pgList_t pgList, char* code) {
    link x, p;

    if (pgList->headPg == NULL)
        return;

    /* for per scorrere la lista fino all'elemento cercato */
    for (x = pgList->headPg, p = NULL; x!=NULL &&
         strcmp(x->pg.codice, code)!=0; p = x, x = x->next);

    if (x != NULL) {
        if (x == pgList->headPg) {
            pgList->headPg = x->next;
            if (x == pgList->tailPg)
                pgList->tailPg = NULL;
        }
        else {
            p->next = x->next;
            if (x == pgList->tailPg)
                pgList->tailPg = p;
        }

        pgList->nPg--;
        pg_clean(&x->pg);
        free(x);
    }
    else {
        printf("Personaggio non presente\n\n");
        return;
    }
}


pg_t *pgList_searchByCode(pgList_t pgList, char* code) {
    link x;

    for (x=pgList->headPg; x!=NULL; x=x->next) {
        if (strcmp(x->pg.codice, code) == 0) {
            return &x->pg;
        }
    }
    return NULL;
}
