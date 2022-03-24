#ifndef LAB_8_DATETIME_H
#define LAB_8_DATETIME_H

#include <stdio.h>

typedef struct date {
    int aaaa;
    int mm;
    int gg;
} date;

typedef struct time {
    int hh;
    int mm;
} time;

typedef struct dateTime {
    date data;
    time ora;
} dateTime;

void DATEstore(dateTime dataOra);
dateTime DATETIMEsetNull();
int DATEcmp(dateTime dateTime1, dateTime dateTime2);
int DATEget(dateTime dataOra);

#endif
