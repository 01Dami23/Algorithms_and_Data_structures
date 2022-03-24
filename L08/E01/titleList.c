#include "titleList.h"

typedef struct node *link;
struct node {
    title_t title;
    link next;
};

typedef struct titleList_s {
    link h;
} titleList_s;

static link newNode(title_t title, link next);
static link sortedInsert(link h, title_t title);


titleList_t LISTinit(void) {
    titleList_t titleList = (titleList_t) malloc(sizeof(titleList_s));
    checkErroreAllocazione(titleList);

    titleList->h = NULL;
    return titleList;
}


void LISTfree (titleList_t titleList) {
    link x = titleList->h, temp;
    while(x != NULL) {
        temp = x;
        x = x->next;
        TITLEfree(temp->title);
        free(temp);
    }
    free(titleList);
}


titleList_t titleList_addTitle(titleList_t titleList, char name[MAXCHAR]) {
    /* solo se non trovo il titolo lo devo inizializzare e inserire nella lista */
    title_t title = titleList_searchByTitleName(titleList, name);
    if (title == NULL) {
        title = TITLEinit(name);
        titleList->h = sortedInsert(titleList->h, title);
    }

    return titleList;
}


title_t titleList_searchByTitleName(titleList_t titleList, char name[MAXCHAR]) {
    link x;

    for (x=titleList->h; x!=NULL; x=x->next) {
        if (strcmp(TITLEname(x->title), name) == 0) {
            return x->title;
        }
    }
    return NULL;
}


static link newNode(title_t title, link next) {
    link x = malloc(sizeof *x);
    checkErroreAllocazione(x);
    x->title = title;
    x->next = next;
    return x;
}


static link sortedInsert(link h, title_t title) {
    link x, p;
    char name[MAXCHAR];

    strcpy(name, TITLEname(title));

    if (h == NULL || strcmp(name, TITLEname(h->title)) < 0) {
        return newNode(title, h);
    }

    for (x=h->next, p=h; x!=NULL && strcmp(name, TITLEname(x->title)) >= 0; p=x, x=x->next);

    p->next = newNode(title, x);
    return h;
}


void titleList_print(titleList_t titleList) {
    link x = titleList->h;

    while (x != NULL) {
        title_print(x->title);
        x = x->next;
    }
    printf("\n\n");
}