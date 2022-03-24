#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 51
#define NUM_PARAM 5

typedef struct {
    int gg;
    int mm;
    int aaaa;
} data;

typedef struct {
    char *codice, *nome, *cognome, *via, *citta;
    data d;
    int cap;
} Item;

/* definisco la lista */
typedef struct node *link;
struct node {
    Item val;
    link next;
};


link newNode(link next, Item val) {
    link x = malloc(sizeof *x);
    if (x == NULL)
        return NULL;
    x->val = val;
    x->next = next;
    return x;
}


int keyGet(data d) {
    int key;
    key = (d.aaaa * 10000) + (d.mm * 100) + (d.gg);
    return key;
}


link sortedInsert(link h, Item val) {
    link x, p;
    int key = keyGet(val.d);

    if (h == NULL || key >= keyGet(h -> val.d)) {
        return newNode(h, val);
    }
    /* for per scorrere la lista fino a quando arrivo al punto in cui fare l'inserimento */
    for (x=h->next, p=h; x!=NULL && key < keyGet(x -> val.d); p=x, x=x->next);

    p->next = newNode(x, val);
    return h;
}


link read(FILE *fp, link h) {
    char vett_parametri[NUM_PARAM][MAXCHAR];
    Item val;
    int flag = 0;

    while (flag == 0 && fscanf(fp, "%s%s%s   %d %*c %d %*c %d   %s%s %d", vett_parametri[0],
                  vett_parametri[1], vett_parametri[2], &val.d.gg, &val.d.mm, &val.d.aaaa,
                  vett_parametri[3], vett_parametri[4], &val.cap) != EOF) {
        val.codice = strdup(vett_parametri[0]);
        val.nome = strdup(vett_parametri[1]);
        val.cognome = strdup(vett_parametri[2]);
        val.via = strdup(vett_parametri[3]);
        val.citta = strdup(vett_parametri[4]);

        h = sortedInsert(h, val);
        if (fp == stdin)
            flag = -1;
    }
    return h;
}


link searchByCode(link h, link *predecessor_p, char *code) {
    link x;
    for (x=h, *predecessor_p = NULL; x!=NULL; *predecessor_p=x, x=x->next) {
        if (strcmp(x->val.codice, code) == 0) {
            return x;
        }
    }
    return NULL;
}


link cancelNodeByCode(link *hp, char *code) {
    link x, p;

    if (*hp == NULL)
        return NULL;

    x = searchByCode(*hp, &p, code);

    if (x != NULL) {
        if (x == *hp)
            *hp = x->next;
        else
            p->next = x->next;
        return x;
    }
    return NULL;
}


link searchByDate(link h, link *predecessor_p, int key1, int key2) {
    link x;
    int key;
    for (x=h, *predecessor_p = NULL; x!=NULL; *predecessor_p=x, x=x->next) {
        key = keyGet(x->val.d);
        if (key >= key2 && key <= key1) {
            return x;
        }
    }
    return NULL;
}


link cancelNodeByDate(link *hp, data date1, data date2) {
    int key1 = keyGet(date1), key2 = keyGet(date2), temp;
    link x, p;

    if (*hp == NULL)
        return NULL;

    if (key2 > key1) {
        temp = key1;
        key1 = key2;
        key2 = temp;
    }

    x = searchByDate(*hp, &p, key1, key2);

    if (x != NULL) {
        if (x == *hp)
            *hp = x->next;
        else
            p->next = x->next;
        return x;
    }
    return NULL;
}


void printList(link x, FILE *fp) {
    fprintf(fp, "%s %s %s %d/%d/%d %s %s %d\n", x->val.codice, x->val.nome,
            x->val.cognome, x->val.d.gg, x->val.d.mm, x->val.d.aaaa, x->val.via,
            x->val.citta, x->val.cap);
}


void freeNode(link x) {
    free(x->val.codice);
    free(x->val.nome);
    free(x->val.cognome);
    free(x->val.via);
    free(x->val.citta);
    free(x);
}


link readCmd(link h) {
    int comando, flag, counter;
    FILE *fp;
    char nomeFile[MAXCHAR], codice[5];
    link x, p;
    data data1, data2;

    do {
        printf("Esegui un comando digitando un numero:\n");
        printf("\t1 = Acquisizione da tastiera\n");
        printf("\t2 = Acquisizione da file\n");
        printf("\t3 = Ricerca per codice\n");
        printf("\t4 = Cancellazione tramite codice\n");
        printf("\t5 = Cancellazione di elementi tra 2 date\n");
        printf("\t6 = Stampa tutti gli elementi su file\n");
        printf("\t0 = Termina\n");

        scanf("%d", &comando);

        switch (comando) {
            case 1:
                printf("Inserire i dati:\n");
                h = read(stdin, h);
                break;

            case 2:
                printf("Inserire il nome del file:\n");
                scanf("%s", nomeFile);

                if ((fp = fopen(nomeFile, "r")) == NULL) {
                    printf("Errore nell'apertura del file\n");
                    break;
                }
                h = read(fp, h);
                fclose(fp);
                break;

            case 3:
                printf("Inserire il codice:\n");
                scanf("%s", codice);
                x = searchByCode(h, &p, codice);
                if (x != NULL)
                    printList(x, stdout);
                else
                    printf("Nessuna corrispondenza trovata\n");
                break;

            case 4:
                printf("Inserire il codice:\n");
                scanf("%s", codice);
                x = cancelNodeByCode(&h, codice);
                if (x != NULL) {
                    printList(x, stdout);
                    freeNode(x);
                }
                else
                    printf("Nessuna corrispondenza trovata\n");
                break;

            case 5:
                counter = 0;
                flag = 0;

                printf("Inserire le 2 date:\n");
                scanf("%d %*c %d %*c %d    %d %*c %d %*c %d", &data1.gg,
                      &data1.mm, &data1.aaaa, &data2.gg, &data2.mm, &data2.aaaa);
                while (flag == 0){
                    x = cancelNodeByDate(&h, data1, data2);
                    if (x != NULL) {
                        printList(x, stdout);
                        freeNode(x);
                        counter++;
                    }
                    else {
                        flag = -1;
                    }
                }
                if (counter == 0)
                    printf("Nessuna corrispondenza trovata\n");
                break;

            case 6:
                printf("Inserire il nome del file:\n");
                scanf("%s", nomeFile);

                if ((fp = fopen(nomeFile, "w")) == NULL) {
                    printf("Errore nell'apertura del file\n");
                    break;
                }
                for (x=h; x!=NULL; x=x->next) {
                    printList(x, fp);
                }
                fclose(fp);
                break;

            case 0:
                break;
            default:
                printf("Comando non valido\n");
                break;
        }
    } while (comando != 0);
    return h;
}


void freeList(link h) {
    link temp;
    while (h != NULL) {
        temp = h;
        h = h->next;
        freeNode(temp);
    }
}


int main(void) {
    /* creo la lista vuota */
    link head = NULL;

    head = readCmd(head);

    /* libero la lista */
    freeList(head);

    return EXIT_SUCCESS;
}