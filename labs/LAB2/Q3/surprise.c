#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n) {
    printf("%10d ", n);
}

void print_sieves(int n) {
    // Create an array and mark all values as prime (1).
    char nums[n + 1];
    for (int i = 0; i < n + 1; i++) {
        nums[i] = '1';
    }
    for (int i = 2; pow(i, 2) <= n; i++) {
        // If marked as prime, mark all multiples as non-prime (0).
        if (nums[i] == '1') {
            for (int j = pow(i, 2); j <= n; j += i) {
                nums[j] = '0';
            }
        }
    }
    // Filling array with all the primes up to n.
    int k = 0;
    int primeArray[n];
    for (int i = 2; i <= n; i++) {
        if (nums[i] == '1') {
            primeArray[k] = i;
            k++;
        }
    }
    // Comparing consecutive primes in the array.
    int x;
    int y;
    int count = 0;
    for (int i = 0; i < k; i++) {
        x = primeArray[i];
        y = primeArray[i + 1];
        if ((y - x) == 4) {
            count++;
        }
    }
    printf("Number of times the distance was four: %d \n", count);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]) {
    if (argc == 2) {
        print_sieves(atoi(argv[1]));
    }
    else {
        printf("Please state an interger number.\n");
    }
    return 0;
}
