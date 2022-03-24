#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"


tabInv_t *allocaInv(char *filein) {
    FILE *fp = fopen(filein, "r");
    int num, i;
    tabInv_t *tabInv;
    char buf1[MAXCHAR], buf2[MAXCHAR];

    if (fp == NULL) {
        printf("Error opening %s", filein);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &num);
    tabInv = (tabInv_t *) malloc(sizeof(tabInv_t));
    checkErroreAllocazione(tabInv);
    tabInv -> vettInv = (inv_t *) malloc(num * sizeof(inv_t));
    checkErroreAllocazione(tabInv->vettInv);

    tabInv -> nInv = num;

    for (i=0; i<num; i++) {
        fscanf(fp, "%s %s %d %d %d %d %d %d",
               buf1, buf2,
               &tabInv -> vettInv[i].stat.hp,
               &tabInv -> vettInv[i].stat.mp,
               &tabInv -> vettInv[i].stat.atk,
               &tabInv -> vettInv[i].stat.def,
               &tabInv -> vettInv[i].stat.mag,
               &tabInv -> vettInv[i].stat.spr);

        tabInv -> vettInv[i].name = strdup(buf1);
        checkErroreAllocazione(tabInv->vettInv[i].name);

        tabInv -> vettInv[i].type = strdup(buf2);
        checkErroreAllocazione(tabInv->vettInv[i].type);
    }

    fclose(fp);
    return tabInv;
}


inv_t * searchObjByName(tabInv_t *tabInv, char *objName) {
    int i;
    for (i=0; i<tabInv->nInv; i++) {
        if (strcmp(objName, tabInv->vettInv[i].name) == 0)
            return &(tabInv->vettInv[i]);
    }
    return NULL;
}


int fieldValue(stats field, stat_t stat_s) {
    int num;
    switch (field) {
        case hp:
            num = stat_s.hp;
            break;
        case mp:
            num = stat_s.mp;
            break;
        case atk:
            num = stat_s.atk;
            break;
        case def:
            num = stat_s.def;
            break;
        case mag:
            num = stat_s.mag;
            break;
        case spr:
            num = stat_s.spr;
            break;
        default:
            num = 0;
            break;
    }
    return num;
}


void printObj(inv_t *obj_p) {
    stats j;

    printf("\t- %s %s ", obj_p->name, obj_p->type);
    for (j=hp; j<=spr; j++) {
        printf("%d ", fieldValue(j, obj_p->stat));
    }
    printf("\n");
}


void freeTab(tabInv_t *tabInv) {
    int i;

    for (i=0; i<tabInv->nInv; i++) {
        free(tabInv->vettInv[i].name);
        free(tabInv->vettInv[i].type);
    }
    free(tabInv->vettInv);
    free(tabInv);
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}