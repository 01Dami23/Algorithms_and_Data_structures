#include "pg.h"


int pg_read(FILE *fp, pg_t *pgp) {

    if (fscanf(fp, "%s %s %s %d %d %d %d %d %d",
           pgp->codice,
           pgp->nome,
           pgp->classe,
           &pgp->b_stat.hp,
           &pgp->b_stat.mp,
           &pgp->b_stat.atk,
           &pgp->b_stat.def,
           &pgp->b_stat.mag,
           &pgp->b_stat.spr) == 9)
    {
        pgp->eq_stat = pgp->b_stat;     /* inizializzo al valore base anche le stat che poi si modificano con gli oggetti */
        pgp->equip = equipArray_init();
        return 1;
    }
    return 0;
}


void pg_clean(pg_t *pgp) {
    equipArray_free(pgp->equip);
}


void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    int i, index;

    fprintf(fp, "%s %s %s ",
           pgp->codice,
           pgp->nome,
           pgp->classe);
    stat_print(fp, &pgp->b_stat, INT_MIN);

    /* stampo l'equipaggiamento del pg */
    if (equipArray_inUse(pgp->equip) > 0) {
        fprintf(fp, "\t---------------Oggetti----------------\n");
        for (i=0; i<MAX_OBJ; i++) {
            index = equipArray_getEquipByIndex(pgp->equip, i);
            if (index >= 0) {
                /* stampo i dati di ogni oggetto che ha pg */
                invArray_printByIndex(fp, invArray, index);
            }
        }
        fprintf(fp, "\t--------------------------------------\n");
        fprintf(fp, "\tStatistiche dopo applicazione oggetti:\n\t      ");
        /* stampo eq_stat del personaggio */
        stat_print(fp, &pgp->eq_stat, MIN_STAT);
        fprintf(fp, "\n");
    }
}


void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    int comando, index, result;
    char objName[LEN];
    inv_t *inv_p;
    stat_t statObj;

    printf("Esegui un comando digitando un numero:\n");
    printf("\t0 = Aggiungi oggetto\n");
    printf("\t1 = Rimuovi oggetto\n");
    fscanf(stdin, "%d", &comando);

    if (comando != add && comando != rmv) {
        printf("Comando non valido\n\n");
        return;
    }

    printf("Inserisci il nome dell'oggetto:\n");
    fscanf(stdin, "%s", objName);

    index = invArray_searchByName(invArray, objName);
    if (index == -1) {
        printf("Oggetto non trovato\n\n");
        return;
    }

    result = equipArray_update(pgp->equip, invArray, objName, comando);

    /* modifico eq_stat di pg in base al nuovo oggetto */
    if (result == 1) {
        inv_p = invArray_getByIndex(invArray, index);
        statObj = inv_getStat(inv_p);
        pgp->eq_stat = pg_modStat(statObj, pgp->eq_stat, comando);
    }
}


stat_t pg_modStat(stat_t obj_stat, stat_t eq_stat, mode mod) {
    if (mod == add) {
        eq_stat.hp += obj_stat.hp;
        eq_stat.mp += obj_stat.mp;
        eq_stat.atk += obj_stat.atk;
        eq_stat.def += obj_stat.def;
        eq_stat.mag += obj_stat.mag;
        eq_stat.spr += obj_stat.spr;
    }
    else if (mod == rmv) {
        eq_stat.hp -= obj_stat.hp;
        eq_stat.mp -= obj_stat.mp;
        eq_stat.atk -= obj_stat.atk;
        eq_stat.def -= obj_stat.def;
        eq_stat.mag -= obj_stat.mag;
        eq_stat.spr -= obj_stat.spr;
    }
    return eq_stat;
}