#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n) {
    printf("%10d ", n);
}


void print_sieves(int n) {
    // Allocate nums array on heap with size of n + 1 chars.
    char* nums = (char*)malloc((n+1)*sizeof(char));
    for (int i = 2; i < n+1; i++) {
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
    // Print all numbers marked as prime (with value 1).
    int j = 0;
    for (int i = 2; i <= n; i++) {
        if (nums[i] == '1') {
            print_number(i);
            j++;
            if (j % COLUMNS == 0) {
                printf("\n");
            }
        }
    }
    // Release the memory allocated on the heap.
    free(nums);
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
