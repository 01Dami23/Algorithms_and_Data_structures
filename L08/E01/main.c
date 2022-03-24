#include <stdio.h>
#include <stdlib.h>

#include "titleList.h"
#include "title.h"
#include "dateTime.h"

#define MAXCHAR 21

void readFile(char filename[MAXCHAR], titleList_t titleList);


int main(void) {
    int comando, flag_fine=1, min, max, S;
    char file_name[MAXCHAR];
    titleList_t titleList = LISTinit();
    title_t title;
    dateTime dataOra1, dataOra2;

    dataOra1 = dataOra2 = DATETIMEsetNull();


    do {
        printf("Esegui un comando digitando un numero:\n");
        printf("\t1 = Salva elenco di transazioni\n");
        printf("\t2 = Ricerca di un titolo azionario\n");
        printf("\t3 = Ricerca della quotazione di un titolo per data\n");
        printf("\t4 = Ricerca delle quotazioni MIN e MAX di un titolo tra 2 date\n");
        printf("\t5 = Ricerca delle quotazioni MIN e MAX di un titolo su tutto il periodo\n");
        printf("\t6 = Bilanciamento BST quotazioni di un titolo (se (Cammino piu lungo)/(Cammino piu corto) > S)\n");
        printf("\t0 = Termina\n");

        scanf("%d", &comando);

        switch(comando){
            case 0: {
                flag_fine = 0;
            } break;

            case 1: {
                printf("Inserire il nome del file:\n");
                scanf("%s", file_name);
                readFile(file_name, titleList);

                titleList_print(titleList);

            } break;

            case 2: {
                printf("Inserire il nome del titolo:\n");
                scanf("%s", file_name);

                title = titleList_searchByTitleName(titleList, file_name);
                if (title == NULL)
                    printf("Titolo non trovato\n");
                else
                    title_print(title);
                printf("\n");
            } break;

            case 3: {
                printf("Inserire il nome del titolo:\n");
                scanf("%s", file_name);

                title = titleList_searchByTitleName(titleList, file_name);
                if (title == NULL)
                    printf("Titolo non trovato\n");
                else {
                    printf("Inserire la data aaaa/mm/gg:\n");
                    scanf("%d %*c %d %*c %d", &dataOra1.data.aaaa, &dataOra1.data.mm, &dataOra1.data.gg);
                    title_BSTsearch(title, dataOra1);
                }
                printf("\n");

                dataOra1 = DATETIMEsetNull();

            } break;

            case 4: {
                printf("Inserire il nome del titolo:\n");
                scanf("%s", file_name);

                title = titleList_searchByTitleName(titleList, file_name);
                if (title == NULL)
                    printf("Titolo non trovato\n");
                else {
                    printf("Inserire la prima data aaaa/mm/gg:\n");
                    scanf("%d %*c %d %*c %d", &dataOra1.data.aaaa, &dataOra1.data.mm, &dataOra1.data.gg);

                    printf("Inserire la seconda data aaaa/mm/gg:\n");
                    scanf("%d %*c %d %*c %d", &dataOra2.data.aaaa, &dataOra2.data.mm, &dataOra2.data.gg);

                    title_BSTvisit_datesInterval(title, dataOra1, dataOra2, 0);

                }
                printf("\n");

                dataOra1 = dataOra2 = DATETIMEsetNull();

            } break;

            case 5: {
                printf("Inserire il nome del titolo:\n");
                scanf("%s", file_name);

                title = titleList_searchByTitleName(titleList, file_name);
                if (title == NULL)
                    printf("Titolo non trovato\n");
                else {
                    dataOra1 = dataOra2 = DATETIMEsetNull();
                    title_BSTvisit_datesInterval(title, dataOra1, dataOra2, 1);
                }
                printf("\n");

                dataOra1 = dataOra2 = DATETIMEsetNull();

            } break;

            case 6: {
                printf("Inserire il nome del titolo:\n");
                scanf("%s", file_name);

                title = titleList_searchByTitleName(titleList, file_name);
                if (title == NULL)
                    printf("Titolo non trovato\n");
                else {
                    printf("Inserire la soglia S:\n");
                    scanf("%d", &S);
                    min = title_BSTminHeight(title);
                    max = title_BSTmaxHeight(title);

                    if ((float)max/(float)min > (float)S)
                        title_BSTbalance(title);
                }
            } break;

            default: {
                printf("Comando non valido\n");
            } break;
        }
    } while (flag_fine == 1);

    LISTfree(titleList);
    return EXIT_SUCCESS;
}


void readFile(char filename[MAXCHAR], titleList_t titleList) {
    FILE *fp = fopen(filename, "r");
    int num, transaction_num, i, j, number;
    float value;
    char name[MAXCHAR];
    dateTime dataOra;

    if (fp == NULL) {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &num);
    for (i=0; i < num; i++) {       /* per ogni titolo */
        fscanf(fp, "%s %d", name, &transaction_num);

        titleList = titleList_addTitle(titleList, name);

        for (j=0; j < transaction_num; j++) {       /* per ogni operazione */
            fscanf(fp, "%d %*c %d %*c %d", &dataOra.data.aaaa, &dataOra.data.mm, &dataOra.data.gg);
            fscanf(fp, "%d %*c %d", &dataOra.ora.hh, &dataOra.ora.mm);
            fscanf(fp, "%f %d", &value, &number);

            /* per ogni operazione faccio un inserimento nel BST di quel titolo:
             * - se trovo gia la data nel bst allora ne aggiorno la quotazione (quindi va riordinato il BST)
             * - se quella quella data non e ancora nel BST la aggiungo insieme alla sua quotazione */
            title_addQuotation(titleList_searchByTitleName(titleList, name), dataOra, value, number);
        }
    }
    fclose(fp);
}