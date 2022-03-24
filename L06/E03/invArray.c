#include "invArray.h"

typedef struct invArray_s {
    inv_t *vettInv;
    int nInv;
} invArray_s;


invArray_t invArray_init(void) {
    invArray_t invArray;

    invArray = (invArray_t) malloc(sizeof(invArray_s));
    checkErroreAllocazione(invArray);
    
    return invArray;
}


void invArray_free(invArray_t invArray) {
    free(invArray->vettInv);
    free(invArray);
}


void invArray_read(FILE *fp, invArray_t invArray) {
    int num, i;
    
    if (fp == NULL) {
        printf("Error opening inv file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", &num);
    invArray->vettInv = (inv_t *) malloc(num * sizeof(inv_t));
    checkErroreAllocazione(invArray->vettInv);

    invArray->nInv = num;

    for (i=0; i<num; i++) {
        inv_read(fp, &invArray->vettInv[i]);
    }
}


void invArray_print(FILE *fp, invArray_t invArray) {
    int i;

    fprintf(fp, "Oggetti disponibili:\n");
    for (i=0; i<invArray->nInv; i++) {
        inv_print(fp, &invArray->vettInv[i]);
    }
    printf("\n\n");
}


void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_t *inv_p = invArray_getByIndex(invArray, index);
    inv_print(fp, inv_p);
}


inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    return &invArray->vettInv[index];
}


int invArray_searchByName(invArray_t invArray, char *objName) {
    int i;
    for (i=0; i<invArray->nInv; i++) {
        if (strcmp(objName, invArray->vettInv[i].name) == 0) {
            return i;
        }
    }
    return -1;
}