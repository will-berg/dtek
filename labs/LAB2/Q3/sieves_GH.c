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


void print_sieves(int n) {
    // Initialize array with numbers from 1 to n,
    char primeArray[n + 1];
    int r = 0;
    // Fill the array
    for (int i = 1; i <= n; i++) {
        primeArray[i] = i;
    }
    for (int i = 2; i <= n; i++) {
        if(is_prime(primeArray[i]) == 1) {
            print_number(primeArray[i]);
            r++;
            if(r % COLUMNS == 0) {
                printf("\n");
            }
            for(int j = i*2; j <= n; j+=i) {
                //Set all multiples of the prime to -1, so they will not be prime.
                primeArray[j] = -1;
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
        print_sieves(atoi(argv[1]));
    }
    else {
        printf("Please state an interger number.\n");
    }
    return 0;
}
