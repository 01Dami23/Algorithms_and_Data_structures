#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filein "corse.txt"
#define fileout "testo.txt"
#define MAXCHAR 31
#define MAXCHAR_CMD 101
#define MAXRIGHE 1000

typedef struct dati_corse {
    char codice_tratta[MAXCHAR];
    char partenza[MAXCHAR];
    char capolinea[MAXCHAR];
    char data[MAXCHAR];
    char ora_partenza[MAXCHAR];
    char ora_arrivo[MAXCHAR];
    int ritardo;
} dati_corse;

typedef enum comando_e {
    r_stampa,
    r_ordData,
    r_ordCodiceTratta,
    r_ordPartenza,
    r_ordCapolinea,
    r_stampaPerPartenza,
    r_fine,
    r_errore
} comando_e;

typedef enum key{
    data,
    codice_tratta,
    partenza,
    capolinea
} key;

int leggiFile(FILE *fp, dati_corse dati[MAXRIGHE]);
comando_e leggiComando(void);
void menuParola(FILE *fp, dati_corse dati[MAXRIGHE], int num_righe);

void optBubbleSort(dati_corse *vetp[MAXRIGHE], int n, key chiave);
int compara(dati_corse *p1, dati_corse *p2, key chiave);

int ricercaLineare(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]);
int ricercaDicotomica(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]);

void stampaStruct(dati_corse *vetp[MAXRIGHE], int n);


int main(void) {
    FILE *fin, *fout;
    dati_corse dati[MAXRIGHE];
    int num_righe;

    if ((fin = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }
    if ((fout = fopen(fileout, "w")) == NULL) {
        printf("Error opening output file\n");
        return EXIT_FAILURE;
    }

    /*chiamo la funzione che prende i dati e li inserisce in un vettore di struct*/
    num_righe = leggiFile(fin, dati);

    /*chiamo la funzione con il costrutto switch, per eseguire i diversi comandi*/
    menuParola(fout, dati, num_righe);

    fclose(fin);
    fclose(fout);
    return EXIT_SUCCESS;
}


int leggiFile(FILE *fp, dati_corse dati[MAXRIGHE]) {
    int file_length, i;

    fscanf(fp, "%d", &file_length);
    for (i = 0; i < file_length; i++) {
        fscanf(fp, "%s%s%s%s %s%s %d", dati[i].codice_tratta, dati[i].partenza,
               dati[i].capolinea, dati[i].data, dati[i].ora_partenza, dati[i].ora_arrivo,
               &dati[i].ritardo);
    }
    return file_length;
}


comando_e leggiComando(void) {
    comando_e comando;
    char comando_utente[MAXCHAR_CMD];
    char tabella_comandi[r_fine + 1][MAXCHAR] = {"stampa", "ord_data", "ord_codice_tratta",
                                                 "ord_partenza", "ord_capolinea",
                                                 "stampa_per_partenza", "fine"};

    printf("Insert an instruction (\"stampa [console/file]\", \"ord_data\", \"ord_codice_tratta\", "
           "\"ord_partenza\", \"ord_capolinea\", \"stampa_per_partenza [lineare/dicotomica][prefisso]\" o \"fine\"):\n");
    scanf("%s", comando_utente);

    comando = r_stampa;
    while (comando < r_errore && strcmp(comando_utente, tabella_comandi[comando]) != 0) {
        comando++;
    }
    return comando;
}


void menuParola(FILE *fp, dati_corse dati[MAXRIGHE], int num_righe) {
    char parametro[MAXCHAR], tipo_ricerca[MAXCHAR];
    int flag = 1, i, j, mid_index, start_index, end_index, temp, flag_ricerca;
    dati_corse *dati_pointers[MAXRIGHE];
    comando_e cmd;

    while (flag) {

        /*creo un vettore di puntatori alle struct del vettore di struct che
         *si resetta ad ogni comando cosi posso operare l'ordinamento sul
         *vettore di puntatori senza spostare le struct nel vettore di struct*/
        for (j=0; j<num_righe; j++) {
            dati_pointers[j] = &dati[j];
        }

        /*chiamo la funzione che chiede all'utente il comando*/
        cmd = leggiComando();

        switch (cmd) {
            case r_stampa:
                scanf("%s", parametro);
                if (strcmp(parametro, "console") == 0) {
                    for (i=0; i<num_righe; i++) {
                        printf("%s %s %s %s %s %s %d\n", dati[i].codice_tratta,
                               dati[i].partenza, dati[i].capolinea, dati[i].data,
                               dati[i].ora_partenza, dati[i].ora_arrivo, dati[i].ritardo);
                    }
                    printf("\n");
                }
                else if (strcmp(parametro, "file") == 0) {
                    for (i=0; i<num_righe; i++) {
                        fprintf(fp, "%s %s %s %s %s %s %d\n", dati[i].codice_tratta,
                                dati[i].partenza, dati[i].capolinea, dati[i].data,
                                dati[i].ora_partenza, dati[i].ora_arrivo, dati[i].ritardo);
                    }
                }
                else {
                    printf("Wrong command inserted\n\n");
                }
                break;
            case r_ordData:
                optBubbleSort(dati_pointers, num_righe, data);
                stampaStruct(dati_pointers, num_righe);
                break;
            case r_ordCodiceTratta:
                optBubbleSort(dati_pointers, num_righe,codice_tratta);
                stampaStruct(dati_pointers, num_righe);
                break;
            case r_ordPartenza:
                optBubbleSort(dati_pointers, num_righe, partenza);
                stampaStruct(dati_pointers, num_righe);
                break;
            case r_ordCapolinea:
                optBubbleSort(dati_pointers, num_righe, capolinea);
                stampaStruct(dati_pointers, num_righe);
                break;
            case r_stampaPerPartenza:
                scanf("%s", tipo_ricerca);
                scanf("%s", parametro);

                if (strcmp(tipo_ricerca, "lineare") == 0) {
                    flag_ricerca = ricercaLineare(dati_pointers, num_righe, parametro);
                    if (flag_ricerca == -1) {
                        printf("There are no matches for this prefix\n\n");
                        break;
                    }
                }
                else if (strcmp(tipo_ricerca, "dicotomica") == 0) {
                    optBubbleSort(dati_pointers, num_righe, partenza);
                    mid_index = ricercaDicotomica(dati_pointers, num_righe, parametro);

                    if (mid_index == -1) {
                        printf("There are no matches for this prefix\n\n");
                        break;
                    }

                    /*ricerca lineare di match prima di mid_index*/
                    temp = mid_index;
                    start_index = mid_index;
                    while (temp > 0 && strncmp(dati_pointers[--temp] -> partenza, parametro, strlen(parametro)) == 0) {
                        start_index = temp;
                    }
                    /*ricerca lineare di match dopo mid_index*/
                    temp = mid_index;
                    end_index = mid_index;
                    while (temp < (num_righe - 1) && strncmp(dati_pointers[++temp] -> partenza, parametro, strlen(parametro)) == 0) {
                        end_index = temp;
                    }
                    for (i=start_index; i<=end_index; i++) {
                        printf("%s %s %s %s %s %s %d\n", dati_pointers[i] -> codice_tratta,
                               dati_pointers[i] -> partenza, dati_pointers[i]  -> capolinea,
                               dati_pointers[i] -> data, dati_pointers[i] -> ora_partenza,
                               dati_pointers[i] -> ora_arrivo, dati_pointers[i] -> ritardo);
                    }
                    printf("\n");
                }
                else {
                    printf("Wrong command inserted\n\n");
                }
                break;
            case r_fine: flag = 0;
                break;
            case r_errore:
            default: printf("Wrong command inserted\n\n");
                break;
        }
    }
}


void optBubbleSort(dati_corse *vetp[MAXRIGHE], int n, key chiave) {
    dati_corse *temp;
    int i, j, l = 0, r = n-1, flag = 1;

    for (i = l; i < r && flag == 1; i++) {
        flag = 0;
        for (j = l; j < r-i+l; j++) {
            if (compara(vetp[j], vetp[j+1], chiave) > 0) {
                flag = 1;
                temp = vetp[j];
                vetp[j] = vetp[j+1];
                vetp[j+1] = temp;
            }
            else if (chiave == data && compara(vetp[j], vetp[j+1], chiave) == 0) {
                if (strcmp(vetp[j] -> ora_partenza, vetp[j+1] -> ora_partenza) > 0) {
                    flag = 1;
                    temp = vetp[j];
                    vetp[j] = vetp[j+1];
                    vetp[j+1] = temp;
                }
            }
        }
    }
}


/*funzione che compara con uno switch la strcmp del bubble sort e gli do l'enum e i
 * 2 puntatori alla struct i cui campi sono da comparare*/
int compara(dati_corse *p1, dati_corse *p2, key chiave) {
    int result;

    switch (chiave) {
        case data:
            result = strcmp(p1 -> data, p2 -> data);
            break;
        case codice_tratta:
            result = strcmp(p1 -> codice_tratta, p2 -> codice_tratta);
            break;
        case partenza:
            result = strcmp(p1 -> partenza, p2 -> partenza);
            break;
        case capolinea:
            result = strcmp(p1 -> capolinea, p2 -> capolinea);
            break;
        default:
            printf("Something went wrong\n\n");
            exit(EXIT_FAILURE);
    }
    return result;
}


int ricercaLineare(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]) {
    int i, len_prefix, flag = -1;

    len_prefix = strlen(prefix);

    for (i=0; i<n; i++) {
        if (strncmp(vetp[i] -> partenza, prefix, len_prefix) == 0) {
            printf("%s %s %s %s %s %s %d\n", vetp[i] -> codice_tratta,
                   vetp[i] -> partenza, vetp[i]  -> capolinea,
                   vetp[i] -> data, vetp[i] -> ora_partenza,
                   vetp[i] -> ora_arrivo, vetp[i] -> ritardo);
            flag = 1;
        }
    }
    printf("\n");
    return flag;
}


int ricercaDicotomica(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]) {
    int l = 0, r = n-1, middle;

    while (l <= r) {
        middle = l + (r - l) / 2;
        if (strncmp((vetp[middle] -> partenza), prefix, strlen(prefix)) == 0) {
            return middle;
        }
        else if (strncmp((vetp[middle] -> partenza), prefix, strlen(prefix)) > 0) {
            r = middle - 1;
        }
        else {
            l = middle + 1;
        }
    }
    return -1;
}


void stampaStruct(dati_corse *vetp[MAXRIGHE], int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%s %s %s %s %s %s %d\n", vetp[i] -> codice_tratta,
               vetp[i] -> partenza, vetp[i]  -> capolinea,
               vetp[i] -> data, vetp[i] -> ora_partenza,
               vetp[i] -> ora_arrivo, vetp[i] -> ritardo);
    }
    printf("\n");
}