#include <stdio.h>
#include <stdlib.h>

#define filein "hard_test_set.txt"

typedef enum {
    zaffiro, rubino, topazio, smeraldo
} stone_types;

int fZ(int ****matZ, int ****matR, int ****matT, int ****matS, int *val);
int fR(int ****matZ, int ****matR, int ****matT, int ****matS, int *val);
int fT(int ****matZ, int ****matR, int ****matT, int ****matS, int *val);
int fS(int ****matZ, int ****matR, int ****matT, int ****matS, int *val);

int max(int num1, int num2, int num3, int num4);

int ****mat_init(int *val);
void free_mat(int ****mat, int *val);
void checkErroreAllocazione(void *p);



int main(void) {
    FILE *fin;
    int num, i, data[4], maxLen, len;
    int ****matZ, ****matR, ****matT, ****matS;
    int maxLenZ, maxLenR, maxLenT, maxLenS;

    if  ((fin = fopen(filein, "r")) == NULL) {
        printf("Error opening input file\n");
        return EXIT_FAILURE;
    }

    fscanf(fin, "%d", &num);
    for (i=0; i<num; i++) {

        fscanf(fin, "%d %d %d %d", &data[zaffiro], &data[rubino],
               &data[topazio], &data[smeraldo]);
        len = data[zaffiro] + data[smeraldo] + data[rubino] + data[topazio];

        matZ = mat_init(data);
        matR = mat_init(data);
        matT = mat_init(data);
        matS = mat_init(data);

        printf("TEST #%d\nzaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n",
               i+1, data[zaffiro], data[rubino], data[topazio], data[smeraldo], len);

        if (data[zaffiro] > 0)
            maxLenZ = fZ(matZ, matR, matT, matS, data);
        else
            maxLenZ = 0;

        if (data[rubino] > 0)
            maxLenR = fR(matZ, matR, matT, matS, data);
        else
            maxLenR = 0;

        if (data[topazio] > 0)
            maxLenT = fT(matZ, matR, matT, matS, data);
        else
            maxLenT = 0;

        if (data[smeraldo] > 0)
            maxLenS = fS(matZ, matR, matT, matS, data);
        else
            maxLenS = 0;

        maxLen = max(maxLenZ, maxLenR, maxLenT, maxLenS);

        printf("Collana massima di lunghezza %d\n", maxLen);

        free_mat(matZ, data);
        free_mat(matR, data);
        free_mat(matT, data);
        free_mat(matS, data);
    }

    fclose(fin);
    return EXIT_SUCCESS;
}


int fZ(int ****matZ, int ****matR, int ****matT, int ****matS, int *val) {
    int maxLenZ = 0, maxLenR = 0, count = 0;
    int z=val[zaffiro], r=val[rubino], t=val[topazio], s=val[smeraldo];

    if (matZ[z][r][t][s] > 0)
        return matZ[z][r][t][s];

    val[zaffiro]--;

    if (val[zaffiro] > 0) {
        maxLenZ = fZ(matZ, matR, matT, matS, val);
        count++;
    }
    if (val[rubino] > 0) {
        maxLenR = fR(matZ, matR, matT, matS, val);
        count++;
    }

    val[zaffiro]++;

    if (count == 0) {
        matZ[z][r][t][s] = 1;
        return matZ[z][r][t][s];
    }
    if (maxLenZ >= maxLenR) {
        matZ[z][r][t][s] = maxLenZ+1;
        return matZ[z][r][t][s];
    }
    else {
        matZ[z][r][t][s] = maxLenR+1;
        return matZ[z][r][t][s];
    }
}



int fR(int ****matZ, int ****matR, int ****matT, int ****matS, int *val) {
    int maxLenS=0, maxLenT=0, count=0;
    int z=val[zaffiro], r=val[rubino], t=val[topazio], s=val[smeraldo];

    if (matR[z][r][t][s] > 0)
        return matR[z][r][t][s];

    val[rubino]--;

    if (val[smeraldo] > 0) {
        maxLenS = fS(matZ, matR, matT, matS, val);
        count++;
    }
    if (val[topazio] > 0) {
        maxLenT = fT(matZ, matR, matT, matS, val);
        count++;
    }

    val[rubino]++;

    if (count == 0) {
        matR[z][r][t][s] = 1;
        return matR[z][r][t][s];
    }
    if (maxLenS >= maxLenT) {
        matR[z][r][t][s] = maxLenS+1;
        return matR[z][r][t][s];
    }
    else {
        matR[z][r][t][s] = maxLenT+1;
        return matR[z][r][t][s];
    }
}


int fT(int ****matZ, int ****matR, int ****matT, int ****matS, int *val) {
    int maxLenZ = 0, maxLenR = 0, count = 0;
    int z=val[zaffiro], r=val[rubino], t=val[topazio], s=val[smeraldo];

    if (matT[z][r][t][s] > 0)
        return matT[z][r][t][s];

    val[topazio]--;

    if (val[zaffiro] > 0) {
        maxLenZ = fZ(matZ, matR, matT, matS, val);
        count++;
    }
    if (val[rubino] > 0) {
        maxLenR = fR(matZ, matR, matT, matS, val);
        count++;
    }

    val[topazio]++;

    if (count == 0) {
        matT[z][r][t][s] = 1;
        return matT[z][r][t][s];
    }
    if (maxLenZ >= maxLenR) {
        matT[z][r][t][s] = maxLenZ + 1;
        return matT[z][r][t][s];
    }
    else {
        matT[z][r][t][s] = maxLenR + 1;
        return matT[z][r][t][s];
    }
}


int fS(int ****matZ, int ****matR, int ****matT, int ****matS, int *val) {
    int maxLenS=0, maxLenT=0, count=0;
    int z=val[zaffiro], r=val[rubino], t=val[topazio], s=val[smeraldo];

    if (matS[z][r][t][s] > 0)
        return matS[z][r][t][s];

    val[smeraldo]--;

    if (val[smeraldo] > 0) {
        maxLenS = fS(matZ, matR, matT, matS, val);
        count++;
    }
    if (val[topazio] > 0) {
        maxLenT = fT(matZ, matR, matT, matS, val);
        count++;
    }

    val[smeraldo]++;

    if (count == 0) {
        matS[z][r][t][s] = 1;
        return matS[z][r][t][s];
    }
    if (maxLenS >= maxLenT) {
        matS[z][r][t][s] = maxLenS+1;
        return matS[z][r][t][s];
    }
    else {
        matS[z][r][t][s] = maxLenT+1;
        return matS[z][r][t][s];
    }
}


int max(int num1, int num2, int num3, int num4) {
    int max, max1, max2;

    if (num1 >= num2) max1 = num1;
    else max1 = num2;

    if (num3 >= num4) max2 = num3;
    else max2 = num4;

    if (max1 >= max2) max = max1;
    else max = max2;

    return max;
}


int ****mat_init(int *val) {
    int ****mat;
    int i, j, m;

    mat = (int ****) malloc((val[zaffiro]+1) * sizeof(int ***));
    checkErroreAllocazione(mat);

    for (i=0; i<val[zaffiro]+1; i++) {
        mat[i] = (int ***) malloc((val[rubino]+1) * sizeof(int **));
        checkErroreAllocazione(mat[i]);

        for (j=0; j<val[rubino]+1; j++) {
            mat[i][j] = (int **) malloc((val[topazio]+1) * sizeof(int *));
            checkErroreAllocazione(mat[i][j]);

            for (m=0; m<val[topazio]+1; m++) {
                mat[i][j][m] = (int *) calloc((val[smeraldo]+1), sizeof(int));
                checkErroreAllocazione(mat[i][j][m]);
            }
        }
    }
    return mat;
}


void free_mat(int ****mat, int *val) {
    int i, j, m;

    for (i=0; i<val[zaffiro]+1; i++) {
        for (j=0; j<val[rubino]+1; j++) {
            for (m=0; m<val[topazio]+1; m++) {
                free(mat[i][j][m]);
            }
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}


void checkErroreAllocazione(void *p) {
    if (p == NULL) {
        printf("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
}