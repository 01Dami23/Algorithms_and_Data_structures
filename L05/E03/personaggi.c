#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaggi.h"


link newNode(pg_t pg, link next) {
    link x = malloc(sizeof *x);
    checkErroreAllocazione(x);
    x->pg = pg;
    x->next = next;
    return x;
}


void listInsTail(tabPg_t *tabPg, pg_t pg) {
    if (tabPg->headPg == NULL) {
        tabPg->headPg = tabPg->tailPg = newNode(pg, NULL);
        tabPg -> nPg++;
    }
    else {
        tabPg->tailPg->next = newNode(pg, NULL);
        tabPg->tailPg = tabPg->tailPg->next;
        tabPg -> nPg++;
    }
}


void savePg(FILE *fp, tabPg_t *tabPg) {
    pg_t pg;
    char buf1[MAXCHAR], buf2[MAXCHAR];
    int flag = 0;

    while (flag==0 && fscanf(fp, "%s%s%s%d%d%d%d%d%d",
                  pg.codice, buf1, buf2,
                  &pg.stat.hp,
                  &pg.stat.mp,
                  &pg.stat.atk,
                  &pg.stat.def,
                  &pg.stat.mag,
                  &pg.stat.spr) != EOF) {

        pg.nome = strdup(buf1);
        checkErroreAllocazione(pg.nome);

        pg.classe = strdup(buf2);
        checkErroreAllocazione(pg.classe);

        /* inizializzo il puntatore equip e il suo vettore interno per ogni personaggio */
        pg.equip = (tabEquip_t *) malloc(sizeof(tabEquip_t));
        checkErroreAllocazione(pg.equip);
        pg.equip->inUso = 0;
        pg.equip->vettEq = (inv_t **) calloc(MAX_OBJ, sizeof(inv_t *));
        checkErroreAllocazione(pg.equip->vettEq);

        listInsTail(tabPg, pg);

        /* se prendo da stdin mi aspetto un solo personaggio */
        if (fp == stdin)
            flag = 1;
    }
}


tabPg_t *readFilePg(char *filein) {
    FILE *fp = fopen(filein, "r");
    tabPg_t *tabPg;

    if (fp == NULL) {
        printf("Error opening %s", filein);
        exit(EXIT_FAILURE);
    }

    tabPg = (tabPg_t *) malloc(sizeof(tabPg_t));
    checkErroreAllocazione(tabPg);
    tabPg -> headPg = tabPg -> tailPg = NULL;
    tabPg -> nPg = 0;

    savePg(fp, tabPg);

    fclose(fp);
    return tabPg;
}


link searchPgByCode(tabPg_t *tabPg, link *predecessor_p, char *codice) {
    link x;
    for (x=tabPg->headPg, *predecessor_p = NULL; x!=NULL; *predecessor_p=x, x=x->next) {
        if (strcmp(x->pg.codice, codice) == 0) {
            return x;
        }
    }
    return NULL;
}


void freePg(link x) {
    free(x->pg.nome);
    free(x->pg.classe);
    free(x->pg.equip->vettEq);
    free(x->pg.equip);
    free(x);
}


void eliminatePg(tabPg_t *tabPg, char *codice) {
    link x, p;

    if (tabPg->headPg == NULL)
        return;

    x = searchPgByCode(tabPg, &p, codice);

    if (x != NULL) {
        if (x == tabPg->headPg) {
            tabPg->headPg = x->next;
            if (x == tabPg->tailPg)
                tabPg->tailPg = NULL;
        }
        else {
            p->next = x->next;
            if (x == tabPg->tailPg)
                tabPg->tailPg = p;
        }
    }
    else
        return;
    tabPg->nPg--;
    freePg(x);
}


void addObjToPg(tabPg_t *tabPg, tabInv_t *tabInv,char *codice, char *objName) {
    link x, p;
    inv_t *obj_p;
    int i;

    x = searchPgByCode(tabPg, &p, codice);
    obj_p = searchObjByName(tabInv, objName);
    if (x == NULL || obj_p == NULL) {
        printf("Errore: personaggio o oggetto non presente\n\n");
        return;
    }
    /* assegno il puntatore all'oggetto al personaggio */
    for (i=0; i<MAX_OBJ; i++) {
        if (x->pg.equip->vettEq[i] == NULL) {
            x->pg.equip->vettEq[i] = obj_p;
            x->pg.equip->inUso++;
            return;
        }
    }
}


void rmvObjFromPg(tabPg_t *tabPg, tabInv_t *tabInv,char *codice, char *objName) {
    link x, p;
    inv_t *obj_p;
    int i;

    /* prima di togliere l'ogg al Pg, nel caso in cui lo abbia, controllo che sia un oggetto esistente*/
    x = searchPgByCode(tabPg, &p, codice);
    obj_p = searchObjByName(tabInv, objName);
    if (x == NULL || obj_p == NULL) {
        printf("Errore: personaggio o oggetto non presente\n\n");
        return;
    }

    for (i=0; i<MAX_OBJ; i++) {
        if (x->pg.equip->vettEq[i] != NULL) {
            if (strcmp(x->pg.equip->vettEq[i]->name, objName) == 0) {
                x->pg.equip->vettEq[i] = NULL;
                x->pg.equip->inUso--;
                return;
            }
        }
    }
}


void printPg(tabPg_t *tabPg, char *codice) {
    link x, p;
    int i, flag = 0, num;
    stat_t modStat; /* struct su cui calcolo le stat del Pg dopo i modificatori */

    x = searchPgByCode(tabPg, &p, codice);

    if (x == NULL) {
        printf("Il personaggio indicato non e' presente\n\n");
        return;
    }

    modStat.hp = x->pg.stat.hp;
    modStat.mp = x->pg.stat.mp;
    modStat.atk = x->pg.stat.atk;
    modStat.def = x->pg.stat.def;
    modStat.mag = x->pg.stat.mag;
    modStat.spr = x->pg.stat.spr;

    printf("%s %s %s ", x->pg.codice, x->pg.nome, x->pg.classe);
    for (stats j=hp; j<=spr; j++) {
        printf("%d ", fieldValue(j, x->pg.stat));
    }
    printf("\n");

    for (i=0; i<MAX_OBJ; i++) {
        if (x->pg.equip->vettEq[i] != NULL) {
            if (flag == 0)
                printf("\tOggetti in uso:\n");
            flag = 1; /* ho almeno un modificatore quindi ha senso stampare anche le statistiche modificate */

            printObj(x->pg.equip->vettEq[i]);

            /* calcolo le statistiche modificate dagli oggetti */
            modStat.hp += x->pg.equip->vettEq[i]->stat.hp;
            modStat.mp += x->pg.equip->vettEq[i]->stat.mp;
            modStat.atk += x->pg.equip->vettEq[i]->stat.atk;
            modStat.def += x->pg.equip->vettEq[i]->stat.def;
            modStat.mag += x->pg.equip->vettEq[i]->stat.mag;
            modStat.spr += x->pg.equip->vettEq[i]->stat.spr;
        }
    }
    if (flag == 1) {
        printf("\nStatistiche dopo applicazione oggetti:\n");

        printf("%s %s %s ", x->pg.codice, x->pg.nome, x->pg.classe);
        for (stats j=hp; j<=spr; j++) {
            num = fieldValue(j, modStat);
            (num>=0) ? (printf("%d ", num)) : (printf("%d ", 0));
        }
        printf("\n");
    }
    printf("\n");
}


void free_tabPg(tabPg_t *tabPg) {
    link x = tabPg->headPg, temp;

    /* libero un personaggio alla volta */
    while (x != NULL) {
        temp = x;
        x = x->next;
        freePg(temp);
    }
    free(tabPg);
}