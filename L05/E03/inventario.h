#ifndef _INVENTARIO_H
#define _INVENTARIO_H

#define MAXCHAR 51

typedef enum {
    hp, mp, atk, def, mag, spr
} stats;

typedef struct {
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct {
    char *name;
    char *type;
    stat_t stat;
} inv_t;

typedef struct {
    inv_t *vettInv;
    int nInv;
} tabInv_t;

tabInv_t *allocaInv(char *filein);
inv_t *searchObjByName(tabInv_t *tabInv, char *objName);
int fieldValue(stats field, stat_t stat_s);
void printObj(inv_t *obj_p);
void freeTab(tabInv_t *tabInv);
void checkErroreAllocazione(void *p);

#endif