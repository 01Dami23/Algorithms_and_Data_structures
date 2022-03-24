#include "inv.h"


void stat_read(FILE *fp, stat_t *stat_p) {
    fscanf(fp, " %d %d %d %d %d %d",
           &stat_p->hp,
           &stat_p->mp,
           &stat_p->atk,
           &stat_p->def,
           &stat_p->mag,
           &stat_p->spr);
}


void stat_print(FILE *fp, stat_t *stat_p, int soglia) {

    fprintf(fp, " (");

    (stat_p->hp>=soglia) ? (fprintf(fp, "%d ", stat_p->hp)) : (fprintf(fp, "%d ", soglia));
    (stat_p->mp>=soglia) ? (fprintf(fp, "%d ", stat_p->mp)) : (fprintf(fp, "%d ", soglia));
    (stat_p->atk>=soglia) ? (fprintf(fp, "%d ", stat_p->atk)) : (fprintf(fp, "%d ", soglia));
    (stat_p->def>=soglia) ? (fprintf(fp, "%d ", stat_p->def)) : (fprintf(fp, "%d ", soglia));
    (stat_p->mag>=soglia) ? (fprintf(fp, "%d ", stat_p->mag)) : (fprintf(fp, "%d ", soglia));
    (stat_p->spr>=soglia) ? (fprintf(fp, "%d", stat_p->spr)) : (fprintf(fp, "%d", soglia));

    fprintf(fp, ")\n");
}


void inv_read(FILE *fp, inv_t *inv_p) {
    fscanf(fp, "%s %s",
           inv_p->name,
           inv_p->type);
    stat_read(fp, &inv_p->stat);
}


void inv_print(FILE *fp, inv_t *inv_p) {
    fprintf(fp, "\t- %s %s",
           inv_p->name,
           inv_p->type);
    stat_print(fp, &inv_p->stat, INT_MIN);
}


stat_t inv_getStat(inv_t *inv_p) {
    return inv_p->stat;
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}