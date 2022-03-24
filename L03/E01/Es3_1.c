#include <stdlib.h>
#include <stdio.h>

int majority( int *a, int N);
int majorityR(int *a, int l, int r);
int check(int *a, int l, int r, int n1, int n2);

int main(void) {
    int vett_a[] = {3, 1, 3, 5, 3, 2, 3};
    int n = 7, majority_num;

    majority_num = majority(vett_a, n);

    if (majority_num == -1)
        printf("There is no majority number\n");
    else
        printf("The majority number is: %d\n", majority_num);

    return EXIT_SUCCESS;
}

/* funzione wrapper in cui calcolare l ed r in funzione di N */
int majority(int *a, int N) {
    int l=0, r=N-1, major_num;

    major_num = majorityR(a, l, r);
    return major_num;
}


int majorityR(int *a, int l, int r) {
    int m, num1, num2, maj_num;

    if (l >= r)
        return a[r];

    m = (l + r) / 2;
    num1 = majorityR(a, l, m);
    num2 = majorityR(a, m+1, r);
    maj_num = check(a, l, r, num1, num2);
    return maj_num;
}


int check(int *a, int l, int r, int n1, int n2) {
    int i, occ1 = 0, occ2 = 0;

    if (n1 == n2)
        return n1;

    for (i=l; i<=r; i++) {
        if (a[i] == n1)
            occ1++;
        else if (a[i] == n2)
            occ2++;
    }

    if (occ1 > (r-l+1) / 2)
        return n1;
    else if (occ2 > (r-l+1) / 2)
        return n2;
    else
        return -1;
}