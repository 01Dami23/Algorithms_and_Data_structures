#include <stdlib.h>
#include <stdio.h>
#include "inventario.h"
#include "personaggi.h"

#define invfile "inventario.txt"
#define pgfile "pg.txt"


#define DBG 1

#if DBG
void printPgDBG(link x, FILE *fp) {
    fprintf(fp, "%s %s %s %d %d %d %d %d %d\n", x->pg.codice, x->pg.nome,
            x->pg.classe, x->pg.stat.hp, x->pg.stat.mp, x->pg.stat.atk, x->pg.stat.def,
            x->pg.stat.mag, x->pg.stat.spr);
}
#endif

void readCmd(tabPg_t *tabPg, tabInv_t *tabInv);

int main(void) {
    tabInv_t *tabInv;
    tabPg_t *tabPg;

    /* salvo gli oggetti in un vettore allocato dinamicamente */
    tabInv = allocaInv(invfile);

    /* salvo i personaggi in una lista */
    tabPg = readFilePg(pgfile);

    /* chiamo la funzione di menu */
    readCmd(tabPg, tabInv);

    /* libero tutti i dati allocati dinamicamente */
    free_tabPg(tabPg);
    freeTab(tabInv);

    return EXIT_SUCCESS;
}


void readCmd(tabPg_t *tabPg, tabInv_t *tabInv) {
    int comando;
    char codice[CODE_LEN], oggetto[MAXCHAR];

    do {
        printf("Esegui un comando digitando un numero:\n");
        printf("\t1 = Aggiungi un nuovo personaggio\n");
        printf("\t2 = Rimuovi un personaggio\n");
        printf("\t3 = Aggiungi oggetto ad un personaggio\n");
        printf("\t4 = Rimuovi oggetto ad un personaggio\n");
        printf("\t5 = Mostra statistiche personaggio\n");
        printf("\t0 = Termina\n");

        scanf("%d", &comando);

        switch (comando) {
            case 1:
                printf("Inserisci i dati di un nuovo personaggio:\n");
                savePg(stdin, tabPg);
                break;
            case 2:
                printf("Inserisci il codice del personaggio:\n");
                scanf("%s", codice);
                eliminatePg(tabPg, codice);
                break;
            case 3:
                printf("Inserisci il codice del personaggio:\n");
                scanf("%s", codice);
                printf("Inserisci il nome dell'oggetto:\n");
                scanf("%s", oggetto);
                addObjToPg(tabPg, tabInv, codice, oggetto);
                printPg(tabPg, codice);
                break;
            case 4:
                printf("Inserisci il codice del personaggio:\n");
                scanf("%s", codice);
                printf("Inserisci il nome dell'oggetto:\n");
                scanf("%s", oggetto);
                rmvObjFromPg(tabPg, tabInv, codice, oggetto);
                printPg(tabPg, codice);
                break;
            case 5:
                printf("Inserisci il codice del personaggio:\n");
                scanf("%s", codice);
                printPg(tabPg, codice);
                break;
            case 0:
                break;
            default:
                printf("Comando non valido\n");
                break;
        }

        #if DBG
            int i;
            link x = tabPg->headPg;
            for(i=0; i<tabPg->nPg; i++) {
                printPgDBG(x, stdout);
                x = x->next;
            }
            printf("%d\n", tabPg->nPg);
            printf("\n");
        #endif
    } while (comando != 0);
}