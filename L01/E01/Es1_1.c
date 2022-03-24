#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);
int parentesi(char *included, char *excluded, char *regex_pointer, char *character, int *len_list_pointer);
int upper_lower(char *regex_pointer, char *character);

int main(void) {

    char string[] = "motoRe natante notoI notoi", *stringp = string;
    char regex[] = "[mn][^a].o\\A", *regexp = regex;
    char *result;

    result = cercaRegexp(stringp, regexp);
    /*stampo la stringa dal puntatore trovato in poi*/
    printf("%s", result);

    return 0;
}


char *cercaRegexp(char *src, char *regexp) {
    int i, flag, srclen, counter, len_list;
    char *letter, *regex_moving_p;
    char included_list[53] = {'\0'}, excluded_list[53] = {'\0'};        /*fino a max 26 minuscole e 26 maiuscole (cioe tutti i char alfabetici) piu il terminatore*/

    srclen = strlen(src);

    for (i=0; i<srclen; i++) {        /*per ogni lettera della source*/
        letter = src + i;
        regex_moving_p = regexp;
        flag = 0;
        counter = 0;

        while (*regex_moving_p != '\0' && flag==0) {        /*fino a quando non ho finito di leggere la regex ed ho corrispondenza tra espressione(regex) e lettera*/
            counter++;
            /*per il caso con il .*/
            if (*regex_moving_p == '.' && isalpha(*letter)) {
                letter++;
                regex_moving_p++;
            }

            /*per i casi con [] e [^]*/
            else if (*regex_moving_p == '[' && parentesi(included_list, excluded_list, regex_moving_p, letter, &len_list)) {
                letter++;
                regex_moving_p += len_list;
            }

            /*per \a (lettera minuscola) e \A (lettera maiuscola)*/
            else if (*regex_moving_p == '\\' && upper_lower(regex_moving_p, letter)) {
                letter++;
                regex_moving_p += 2;
            }

            /*per le lettere normali nella regex*/
            else if (*regex_moving_p == *letter) {
                letter++;
                regex_moving_p++;
            }

            /*se non trovo corrispondenza tra lettera ed espressione*/
            else {
                flag = 1;
            }
        }
        if (*regex_moving_p == '\0' && flag == 0 && (i == 0 || *(letter - counter - 1) == ' ') && (*letter == '\0' || *letter == ' ')) {
            return (letter - counter);
        }
    }
    return NULL;
}


int parentesi(char *included, char *excluded, char *regex_pointer, char *character, int *len_list_pointer) {
    int i, j=0, flag = 0, excl_flag = 0;

    regex_pointer++;

    while(*regex_pointer != ']') {
        if (*regex_pointer == '^') {
            excl_flag = 1;
        }
        else {
            if (excl_flag == 0) {
                included[j++] = *regex_pointer;
                included[j] = '\0';
            }
            else {
                excluded[j++] = *regex_pointer;
                excluded[j] = '\0';
            }
        }
        regex_pointer++;
    }

    if (excl_flag == 0) {
        *len_list_pointer = strlen(included);

        for (i=0; i<*len_list_pointer && flag==0; i++) {
            if (*character == included[i]) {
                flag = 1;
            }
        }
        *len_list_pointer += 2;
    }
    else {
        *len_list_pointer = strlen(excluded);
        flag = 1;

        for (i=0; i<*len_list_pointer && flag == 1; i++) {
            if (*character == excluded[i]) {
                flag = 0;
            }
        }
        *len_list_pointer += 3;
    }
    return flag;
}


int upper_lower(char *regex_pointer, char *character) {
    int flag = 0;

    regex_pointer++;

    if (*regex_pointer == 'a' && islower(*character)) {
        flag = 1;
    }
    else if (*regex_pointer == 'A' && isupper(*character)) {
        flag = 1;
    }
    return flag;
}