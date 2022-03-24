#ifndef _PERSONAGGI_H
#define _PERSONAGGI_H
#include "inventario.h"
#define CODE_LEN 7  /* 6+1 contando anche il terminatore */
#define MAX_OBJ 8

typedef struct {
    int inUso;  /* il numero di oggetti che ha il personaggio */
    inv_t **vettEq;
} tabEquip_t;

typedef struct {
    char codice[CODE_LEN];
    char *nome;
    char *classe;
    tabEquip_t *equip;
    stat_t stat;
} pg_t;

typedef struct nodoPg_t *link;
struct nodoPg_t {
    pg_t pg;
    link next;
};

typedef struct {
    link headPg;
    link tailPg;
    int nPg;
} tabPg_t;

link newNode(pg_t pg, link next);
void listInsTail(tabPg_t *tabPg, pg_t pg);
void savePg(FILE *fp, tabPg_t *tabPg);
tabPg_t *readFilePg(char *filein);
link searchPgByCode(tabPg_t *tabPg, link *predecessor_p, char *code);
void freePg(link x);
void eliminatePg(tabPg_t *tabPg, char *codice);
void addObjToPg(tabPg_t *tabPg, tabInv_t *tabInv,char *codice, char *objName);
void rmvObjFromPg(tabPg_t *tabPg, tabInv_t *tabInv,char *codice, char *objName);
void printPg(tabPg_t *tabPg, char *codice);
void free_tabPg(tabPg_t *tabPg);

#endif