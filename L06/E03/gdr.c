#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"
#include "invArray.h"
#include "pg.h"

#define pgfile "pg.txt"
#define invfile "inventario.txt"

#define DBG 0

enum { falso, vero };
typedef int bool;


int main() {
    int comando;
    char codice[LEN];

    FILE *fin;
    bool fineProgramma;

    pgList_t pgList = pgList_init();
    invArray_t invArray = invArray_init();
    pg_t *pgp, pg;

    fin = fopen(pgfile,"r");
    pgList_read(fin, pgList);
    fclose(fin);
#if DBG
    pgList_print(stdout, pgList, invArray);
#endif /* DBG */

    fin = fopen(invfile,"r");
    invArray_read(fin, invArray);
    fclose(fin);
#if DBG
    invArray_print(stdout, invArray);
#endif /* DBG */

    fineProgramma = falso;

    do {
        printf("Esegui un comando digitando un numero:\n");
        printf("\t1 = Stampa personaggi\n");
        printf("\t2 = Stampa inventario\n");
        printf("\t3 = Cerca personaggio\n");
        printf("\t4 = Aggiungi personaggio\n");
        printf("\t5 = Elimina personaggio\n");
        printf("\t6 = Modifica equipaggiamento\n");
        printf("\t0 = Termina\n");

        scanf("%d", &comando);

        switch(comando){

            case 0: {
                fineProgramma = vero;
            } break;

            case 1: {
                pgList_print(stdout, pgList, invArray);
            } break;

            case 2: {
                invArray_print(stdout, invArray);
            } break;

            case 3: {
                printf("Inserire codice personaggio:\n");
                scanf("%s", codice);
                pgp = pgList_searchByCode(pgList, codice);
                if (pgp!=NULL)
                    pg_print(stdout, pgp, invArray);
                else
                    fprintf(stdout, "Personaggio non presente\n\n");
            } break;

            case 4: {
                printf("Codice Nome Classe HP MP ATK DEF MAG SPR:\n");
                if (pg_read(stdin, &pg) != 0) {
                    pgList_insert(pgList, pg);
                }
            } break;

            case 5: {
                printf("Inserire codice personaggio:\n");
                scanf("%s", codice);
                pgList_remove(pgList, codice);
            } break;

            case 6: {
                printf("Inserire codice personaggio:\n");
                scanf("%s", codice);
                pgp = pgList_searchByCode(pgList, codice);
                if (pgp!=NULL)
                    pg_updateEquip(pgp, invArray);
                else
                    fprintf(stdout, "Personaggio non presente\n\n");
            } break;

            default:{
                printf("Comando non valido\n");
            } break;
        }
    } while(!fineProgramma);

    pgList_free(pgList);
    invArray_free(invArray);

    return EXIT_SUCCESS;
}
