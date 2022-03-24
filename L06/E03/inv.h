#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LEN 100
#define MIN_STAT 1

/* quasi ADT statistiche */
typedef struct stat_s {
    int hp, mp, atk, def, mag, spr;
} stat_t;

/* quasi ADT oggetto di inventario */
typedef struct inv_s {
    char name[LEN];
    char type[LEN];
    stat_t stat;
} inv_t;

/* funzioni di input/output delle statistiche */
void stat_read(FILE *fp, stat_t *stat_p);
void stat_print(FILE *fp, stat_t *stat_p, int soglia);

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *inv_p);
void inv_print(FILE *fp, inv_t *inv_p);
/* ritorna la stat di un oggetto */
stat_t inv_getStat(inv_t *inv_p);

void checkErroreAllocazione(void *p);

#endif