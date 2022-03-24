#include "dateTime.h"


void DATEstore(dateTime dataOra) {
    printf("%.4d/%.2d/%.2d ", dataOra.data.aaaa, dataOra.data.mm, dataOra.data.gg);
}


dateTime DATETIMEsetNull() {
    date data = {0, 0, 0};
    time ora = {0, 0};
    dateTime dataOra = {data, ora};

    return dataOra;
}


int DATEcmp(dateTime dateTime1, dateTime dateTime2) {
    int data1, data2;
    data1 = DATEget(dateTime1);
    data2 = DATEget(dateTime2);

    if (data1 > data2)
        return 1;
    else if (data1 == data2)
        return 0;
    return -1;
}


int DATEget(dateTime dataOra) {
    int date;
    date = (dataOra.data.aaaa * 10000) + (dataOra.data.mm * 100) + (dataOra.data.gg);
    return date;
}