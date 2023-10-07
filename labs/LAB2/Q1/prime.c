/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/

#include <math.h>
#include <stdio.h>

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

int main(void) {
    printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
    printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
    printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
