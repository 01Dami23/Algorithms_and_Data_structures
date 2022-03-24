#include "quotation.h"


quotation QUOTATIONset(dateTime k, float value, int number) {
    quotation quot;

    quot.dataOra = k;
    quot.vn = value * (float)number;
    quot.n = number;

    return quot;
}


void QUOTATIONstore(quotation quot) {
    printf("Data: ");
    DATEstore(quot.dataOra);
    printf("Quotazione: %.2f\n", QUOTATIONcompute(quot));
}


quotation QUOTATIONsetNull() {
    dateTime dataOra = DATETIMEsetNull();
    quotation quot = {0, 0, dataOra};
    return quot;
}


int quotation_DATEcmp(dateTime k1, dateTime k2) {
    if (DATEcmp(k1, k2) > 0)
        return 1;
    else if (DATEcmp(k1, k2) == 0)
        return 0;
    return -1;
}


float QUOTATIONcompute(quotation quot) {
    return quot.vn/(float)quot.n;
}


int quotation_DATEget(dateTime dataOra) {
    return DATEget(dataOra);
}