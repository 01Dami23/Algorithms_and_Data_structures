#ifndef LAB_8_TITLE_H
#define LAB_8_TITLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "quotationBST.h"
#include "quotation.h"
#include "dateTime.h"

#define MAXCHAR 21

typedef struct title_s *title_t;

title_t TITLEinit(char name[MAXCHAR]);
void TITLEfree(title_t title);
char *TITLEname(title_t title);
void title_addQuotation(title_t title, dateTime dataOra, float value, int number);
void title_print(title_t title);
void title_BSTsearch(title_t title, dateTime dataOra);
void title_BSTvisit_datesInterval(title_t title, dateTime dateTime1, dateTime dateTime2, int all);
int title_BSTminHeight(title_t title);
int title_BSTmaxHeight(title_t title);
void title_BSTbalance(title_t title);
void checkErroreAllocazione(void *p);

#endif