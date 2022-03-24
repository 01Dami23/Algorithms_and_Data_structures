#include "equipArray.h"

typedef struct equipArray_s {
    int inUso;
    int vettIndici[MAX_OBJ];
} equipArray_s;


equipArray_t equipArray_init(void) {
    equipArray_t equipArray;
    int i;

    equipArray = (equipArray_t) malloc(sizeof(equipArray_s));
    checkErroreAllocazione(equipArray);

    equipArray->inUso = 0;
    for (i=0; i<MAX_OBJ; i++)
        equipArray->vettIndici[i] = -1;
    return equipArray;
}


void equipArray_free(equipArray_t equipArray) {
    free(equipArray);
}


int equipArray_inUse(equipArray_t equipArray) {
    return equipArray->inUso;
}


int equipArray_update(equipArray_t equipArray, invArray_t invArray, char *objName, mode mod) {
    int i, index = invArray_searchByName(invArray, objName);

    if (mod == add) {
        for (i=0; i < MAX_OBJ && equipArray->vettIndici[i] != -1; i++);
        if (i < MAX_OBJ) {
            equipArray->vettIndici[i] = index;
            equipArray->inUso++;
        }
        else {
            printf("Il personaggio scelto ha l'equipaggiamento completo\n\n");
            return 0;
        }
    }
    else if (mod == rmv) {
        for (i=0; i < MAX_OBJ && equipArray->vettIndici[i] != index; i++);
        if (i < MAX_OBJ) {
            equipArray->vettIndici[i] = -1;
            equipArray->inUso--;
        }
        else {
            printf("Il personaggio scelto non ha questo oggetto\n\n");
            return 0;
        }
    }
    return 1;
}


int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    return equipArray->vettIndici[index];
}