#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);

int main(void) {
    int num1, num2, result;

    printf("Inserire 2 numeri:\n");

    scanf("%d %d", &num1, &num2);

    result = gcd(num1, num2);

    printf("The gcd of %d and %d is: %d\n", num1, num2, result);

    return EXIT_SUCCESS;
}


int gcd(int a, int b) {
    int result, temp;

    /*condizione di terminazione*/
    if (a == b) {
        return a;
    }

    if (b > a) {
        temp = a;
        a = b;
        b = temp;
    }

    /*a pari e b pari*/
    if (a % 2 == 0 && b % 2 == 0) {
        result = 2 * gcd(a/2, b/2);
        return result;
    }
    /*a dispari e b pari*/
    else if (a % 2 != 0 && b % 2 == 0) {
        result = gcd(a, b/2);
        return result;
    }
    /*a pari e b dispari*/
    else if (a % 2 == 0 && b % 2 != 0) {
        result = gcd(a/2, b);
        return result;
    }
    /*a dispari e b dispari*/
    else {
        result = gcd((a-b)/2, b);
        return result;
    }
}