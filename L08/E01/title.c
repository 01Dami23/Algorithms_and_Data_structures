#include "title.h"

#define DBG 1

typedef struct title_s {
    char name[MAXCHAR];
    BST bst;
} title_s;


title_t TITLEinit(char name[MAXCHAR]) {
    title_t title = (title_t) malloc(sizeof(title_s));
    checkErroreAllocazione(title);

    strcpy(title->name, name);
    title->bst = BSTinit();

    return title;
}


void TITLEfree(title_t title) {
    BSTfree(title->bst);
    free(title);
}


char *TITLEname(title_t title) {
    return title->name;
}


void title_addQuotation( title_t title, dateTime dataOra, float value, int number) {
    /* cerco nel bst in base alla dateTime: */
    quotation *quot_p = BSTsearch(title->bst, dataOra);
    quotation quot;

    /* se non trovo faccio un inserimento in foglia della nuova data e della sua quotazione */
    if (quot_p == NULL) {
        quot = QUOTATIONset(dataOra, value, number);
        BSTinsert_leafR(title->bst, quot);
    }
    /* se trovo aggiorno la quotazione */
    else {
        quot_p->vn += value * (float)number;
        quot_p->n += number;
    }
}


void title_print(title_t title) {
    printf("Name: %s\n", title->name);
    tree_print_wrapper(title->bst);
    printf("\n");

#if DBG
print2DBST_wrapper(title->bst);
#endif
}


void title_BSTsearch(title_t title, dateTime dataOra) {
    quotation *quot_p = BSTsearch(title->bst, dataOra);
    if (quot_p == NULL)
        printf("Quotazione non presente per la data indicata\n");
    else
        QUOTATIONstore(*quot_p);
}


void title_BSTvisit_datesInterval(title_t title, dateTime dateTime1, dateTime dateTime2, int all) {
    BSTvisit_datesInterval(title->bst, dateTime1, dateTime2, all);
}


int title_BSTminHeight(title_t title) {
    return BSTminHeight(title->bst);
}


int title_BSTmaxHeight(title_t title) {
    return BSTmaxHeight(title->bst);
}


void title_BSTbalance(title_t title) {
    BSTbalance(title->bst);
#if DBG
    print2DBST_wrapper(title->bst);
#endif
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}