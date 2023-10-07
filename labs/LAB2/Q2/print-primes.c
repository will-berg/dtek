/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n) {
    printf("%10d ", n);
}

int is_prime(int n) {
    // Negative numbers are not prime.
    if (n < 0) {
        return 0;
    }
    // Try values.
    for (int i = 2; i <= ceil(n/2); i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void print_primes(int n) {
    int j = 0;
    for (int i = 2; i <= n; i++) {
        // If the number is prime, print it.
        if (is_prime(i) == 1) {
            print_number(i);
            j++;
            if (j % COLUMNS == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]) {
    if(argc == 2) {
        print_primes(atoi(argv[1]));
    }
    else {
        printf("Please state an interger number.\n");
    }
    return 0;
}
