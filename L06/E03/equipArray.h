#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_OBJ 8

#include "invArray.h"

/* per l'aggiunta/rimozione di oggetti da un pg */
typedef enum mode {
    add, rmv
} mode;

/* ADT di prima classe collezione di oggetti di equipaggiamento */
typedef struct equipArray_s *equipArray_t;

/* creatore e disruttore */
equipArray_t equipArray_init(void);
void equipArray_free(equipArray_t equipArray);

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray);

/* scrittura su file (non utilizzata) */
/* void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray); */

/* modifica vettore equipaggiamento */
int equipArray_update(equipArray_t equipArray, invArray_t invArray, char *objName, mode mod);
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index);

#endif