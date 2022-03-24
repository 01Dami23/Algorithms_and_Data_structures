#ifndef LAB_8_QUOTATION_H
#define LAB_8_QUOTATION_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dateTime.h"

typedef struct quotation {
    float vn;
    int n;
    dateTime dataOra;
} quotation;

quotation QUOTATIONset(dateTime k, float value, int number);
void QUOTATIONstore(quotation quot);
quotation QUOTATIONsetNull();
int quotation_DATEcmp(dateTime k1, dateTime k2);
float QUOTATIONcompute(quotation quot);
int quotation_DATEget(dateTime dataOra);

#endif
