#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    r_nuovoFile,
    r_fine,
    r_errore
} comando_e;

typedef enum key{
    data,
    codice_tratta,
    partenza,
    capolinea
} key;

int leggiFile(FILE *fp, dati_corse **dati_p);
comando_e leggiComando(void);
void menuParola(FILE *fp_in, FILE *fp_out, dati_corse *dati_p, int num_righe);

void optBubbleSort(dati_corse *vetp[MAXRIGHE], int n, key chiave);
int compara(dati_corse *p1, dati_corse *p2, key chiave);
int ricercaLineare(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]);
int ricercaDicotomica(dati_corse *vetp[MAXRIGHE], int n, char prefix[MAXCHAR]);

void stampaStruct(dati_corse *vetp[MAXRIGHE], int start, int finish, char mode[8]);
void checkErroreAllocazione(void *p);
void allocaVettPunt(int n, dati_corse **dati_pp, dati_corse ***dati_ppp_data,
                    dati_corse ***dati_ppp_codice, dati_corse ***dati_ppp_partenza,
                    dati_corse ***dati_ppp_capolinea);

int main(void) {
    FILE *fin, *fout;
    dati_corse *dati_p;
    int num_righe;
    char filename[MAXCHAR];

    printf("Specify the name of the file you want to open:\n");
    scanf("%s", filename);

    if ((fin = fopen(filename, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }
    if ((fout = fopen(fileout, "w")) == NULL) {
        printf("Error opening output file\n");
        return EXIT_FAILURE;
    }

    num_righe = leggiFile(fin, &dati_p);

    menuParola(fin, fout, dati_p, num_righe);

    fclose(fout);
    return EXIT_SUCCESS;
}


int leggiFile(FILE *fp, dati_corse **dati_pp) {
    int file_length, i;

    fscanf(fp, "%d", &file_length);

    (*dati_pp) = (dati_corse *) malloc(file_length * sizeof(dati_corse));
    checkErroreAllocazione(*dati_pp);

    for (i = 0; i < file_length; i++) {
        fscanf(fp, "%s%s%s%s %s%s %d", (*dati_pp)[i].codice_tratta, (*dati_pp)[i].partenza,
               (*dati_pp)[i].capolinea, (*dati_pp)[i].data, (*dati_pp)[i].ora_partenza, (*dati_pp)[i].ora_arrivo,
               &(*dati_pp)[i].ritardo);
    }
    return file_length;
}


comando_e leggiComando(void) {
    comando_e comando;
    char comando_utente[MAXCHAR_CMD];
    char tabella_comandi[r_fine + 1][MAXCHAR] = {"stampa", "ord_data", "ord_codice_tratta",
                                                 "ord_partenza", "ord_capolinea",
                                                 "stampa_per_partenza", "nuovo_file", "fine"};

    printf("Insert an instruction (\"stampa [console/file]\", \"ord_data\","
           " \"ord_codice_tratta\", \"ord_partenza\", \"ord_capolinea\","
           " \"stampa_per_partenza [lineare/dicotomica][prefisso]\", \"nuovo_file [nome file]\" o \"fine\"):\n");
    scanf("%s", comando_utente);

    comando = r_stampa;
    while (comando < r_errore && strcmp(comando_utente, tabella_comandi[comando]) != 0) {
        comando++;
    }
    return comando;
}


void menuParola(FILE *fp_in, FILE *fp_out, dati_corse *dati_p, int num_righe) {
    char parametro[MAXCHAR], tipo_ricerca[MAXCHAR];
    int flag = 1, i, mid_index, start_index, end_index, temp, flag_ricerca;
    dati_corse **dati_pp_data, **dati_pp_codice, **dati_pp_partenza, **dati_pp_capolinea;
    comando_e cmd;

    allocaVettPunt(num_righe, &dati_p, &dati_pp_data, &dati_pp_codice, &dati_pp_partenza, &dati_pp_capolinea);

    while (flag) {
        /*chiamo la funzione che chiede all'utente il comando*/
        cmd = leggiComando();

        switch (cmd) {
            case r_stampa:
                scanf("%s", parametro);
                if (strcmp(parametro, "console") == 0) {
                    stampaStruct(&dati_p, 0, num_righe, "normal");
                    printf("\n");
                }
                else if (strcmp(parametro, "file") == 0) {
                    for (i=0; i<num_righe; i++) {
                        fprintf(fp_out, "%s %s %s %s %s %s %d\n", dati_p[i].codice_tratta,
                                dati_p[i].partenza, dati_p[i].capolinea, dati_p[i].data,
                                dati_p[i].ora_partenza, dati_p[i].ora_arrivo, dati_p[i].ritardo);
                    }
                }
                else {
                    printf("Wrong command inserted\n\n");
                }
                break;
            case r_ordData:
                optBubbleSort(dati_pp_data, num_righe, data);
                stampaStruct(dati_pp_data, 0, num_righe, "pointer");
                printf("\n");
                break;
            case r_ordCodiceTratta:
                optBubbleSort(dati_pp_codice, num_righe,codice_tratta);
                stampaStruct(dati_pp_codice, 0, num_righe, "pointer");
                printf("\n");
                break;
            case r_ordPartenza:
                optBubbleSort(dati_pp_partenza, num_righe, partenza);
                stampaStruct(dati_pp_partenza, 0, num_righe, "pointer");
                printf("\n");
                break;
            case r_ordCapolinea:
                optBubbleSort(dati_pp_capolinea, num_righe, capolinea);
                stampaStruct(dati_pp_capolinea, 0, num_righe, "pointer");
                printf("\n");
                break;
            case r_stampaPerPartenza:
                scanf("%s", tipo_ricerca);
                scanf("%s", parametro);

                if (strcmp(tipo_ricerca, "lineare") == 0) {
                    flag_ricerca = ricercaLineare(dati_pp_partenza, num_righe, parametro);
                    if (flag_ricerca == -1) {
                        printf("There are no matches for this prefix\n\n");
                        break;
                    }
                }
                else if (strcmp(tipo_ricerca, "dicotomica") == 0) {
                    optBubbleSort(dati_pp_partenza, num_righe, partenza);
                    mid_index = ricercaDicotomica(dati_pp_partenza, num_righe, parametro);

                    if (mid_index == -1) {
                        printf("\nThere are no matches for this prefix\n\n");
                        break;
                    }

                    /*ricerca lineare di match prima di mid_index*/
                    temp = mid_index;
                    start_index = mid_index;
                    while (temp > 0 && strncmp(dati_pp_partenza[--temp] -> partenza, parametro,
                                               strlen(parametro)) == 0) {
                        start_index = temp;
                    }
                    /*ricerca lineare di match dopo mid_index*/
                    temp = mid_index;
                    end_index = mid_index;
                    while (temp < (num_righe - 1) && strncmp(dati_pp_partenza[++temp] -> partenza,
                                                             parametro, strlen(parametro)) == 0) {
                        end_index = temp;
                    }
                    stampaStruct(dati_pp_partenza, start_index, end_index+1, "pointer");
                    printf("\n");
                }
                else {
                    printf("Wrong command inserted\n\n");
                }
                break;
            case r_nuovoFile:
                fclose(fp_in);

                free(dati_pp_data);
                free(dati_pp_codice);
                free(dati_pp_partenza);
                free(dati_pp_capolinea);
                free(dati_p);

                scanf("%s", parametro);
                if ((fp_in = fopen(parametro, "r")) == NULL) {
                    printf("Error opening input file\n");
                    exit(EXIT_FAILURE);
                }

                num_righe = leggiFile(fp_in, &dati_p);

                allocaVettPunt(num_righe, &dati_p, &dati_pp_data, &dati_pp_codice, &dati_pp_partenza, &dati_pp_capolinea);

                break;
            case r_fine: flag = 0;
                break;
            case r_errore:
            default: printf("Wrong command inserted\n\n");
                break;
        }
    }
    /*qui faccio anche la free dei vettori di puntatori allocati dinamicamente*/
    free(dati_p);
    free(dati_pp_data);
    free(dati_pp_codice);
    free(dati_pp_partenza);
    free(dati_pp_capolinea);
    fclose(fp_in);
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
    int i, flag = -1;

    for (i=0; i<n; i++) {
        if (strncmp(vetp[i] -> partenza, prefix, strlen(prefix)) == 0) {
            stampaStruct(vetp, i, i+1, "pointer");
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


void stampaStruct(dati_corse *vetp[MAXRIGHE], int start, int finish, char mode[8]) {
    int i;
    if (strcmp(mode, "pointer") == 0) {
        for (i=start; i<finish; i++) {
            printf("%s %s %s %s %s %s %d\n", vetp[i] -> codice_tratta,
                   vetp[i] -> partenza, vetp[i]  -> capolinea,
                   vetp[i] -> data, vetp[i] -> ora_partenza,
                   vetp[i] -> ora_arrivo, vetp[i] -> ritardo);
        }
    }
    else if (strcmp(mode, "normal") == 0) {
        for (i=start; i<finish; i++) {
            printf("%s %s %s %s %s %s %d\n", (*vetp)[i].codice_tratta,
                   (*vetp)[i].partenza, (*vetp)[i].capolinea, (*vetp)[i].data,
                   (*vetp)[i].ora_partenza, (*vetp)[i].ora_arrivo, (*vetp)[i].ritardo);
        }
    }
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}


void allocaVettPunt(int n, dati_corse **dati_pp, dati_corse ***dati_ppp_data,
                    dati_corse ***dati_ppp_codice, dati_corse ***dati_ppp_partenza,
                    dati_corse ***dati_ppp_capolinea) {
    int j;

    *dati_ppp_data = (dati_corse **) malloc(n * sizeof(dati_corse *));
    checkErroreAllocazione(*dati_ppp_data);
    *dati_ppp_codice = (dati_corse **) malloc(n * sizeof(dati_corse *));
    checkErroreAllocazione(*dati_ppp_codice);
    *dati_ppp_partenza = (dati_corse **) malloc(n * sizeof(dati_corse *));
    checkErroreAllocazione(*dati_ppp_partenza);
    *dati_ppp_capolinea = (dati_corse **) malloc(n * sizeof(dati_corse *));
    checkErroreAllocazione(*dati_ppp_capolinea);

    for (j=0; j<n; j++) {
        (*dati_ppp_data)[j] = (*dati_ppp_codice)[j] = (*dati_ppp_partenza)[j] =
                (*dati_ppp_capolinea)[j] = &((*dati_pp)[j]);
    }
}