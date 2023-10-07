# Lab 2
## Assignment 1
```C
int is_prime(int n) {
    // Negative numbers are not prime.
    if (n < 0) {
        return 0;
    }
    // Try values.
    for (int i = 2; i < sqrt((double) n); i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}
```

## Assignment 2
*What does it mean when a function does not return a value? How do you state that in a program? How can then the function (or more precisely, the procedure) perform anything useful?*

When a function does not return a value, that means that nothing is returned when calling the function that the caller can use in the program. This type of operation does nothing `int my_variable = my_function();` since the function does not return anything to assign to `my_variable`. To state that a function does not return anything, you give it a return type of `void`. The function can still be useful however, by modifying inputs or printing to standard out for example.

*How did you implement the side effect that is needed to make `print_number` behave correctly?*

If the "side effect" being referred to is to make it print in several columns, then this was achieved by incrementing a variable every time a prime number was printed and then printing a new line when that variable turned into a multiple of the value in `COLUMNS`. This was all done in the for loop of the `print_primes()` function.

## Assignment 3
*How did you represent the marking of 'prime' and 'not a prime' in the memory array?*

This was done with the characters '1' and '0', with '1' representing prime and '0' representing not a prime.

*Which are the main steps of the algorithm? How have you implemented these steps?*

First we created an array and marked all of the entries as prime. We then started a loop from 2, which is the first prime, and kept on looping until the value of i<sup>2</sup> is less than or equal to the input of `n`. This is because the first value marked as non-prime is the square of the prime number and if the square of that number is bigger than `n`, then that means that all of the non-primes have been marked already. We then keep on stopping at primes in the loop and marking their multiples as non prime until the above is true. When that loop is finished we have an array of 1 or 0 values, that represent each number from 2 to n. They can be printed with another loop beginning at 2 and ending at `n`.

*What is the largest prime number that you can print within 2 seconds of computation? What is the largest number you can print within 10 seconds? Is it the same for `print-primes.c`, `sieves.c`, and `sieves-heap.c`? Why or why not?*

Using the unix `time` command when running the programs with different inputs gave us the numbers we needed here.

When running `sieves-heap.c`, the largest prime number that we could print within 2 seconds was 39999983 (n = 40 million). The largest number we could print within 10 seconds was 199999991 (n = 200 million).

When running `sieves.c`, the largest prime number that we could print before memory corruption was somewhere around 8300000 (n = 8.3 million). This took about 0.4 seconds to compute.

When running `print-primes.c`, the largest prime number that we could print within 2 seconds was 3999971 (n = 4 million). The largest number we could print within 10 seconds was 11999989 (n = 12 million).

Because all of these solutions differ, they have different performance for different sized inputs. The sieves algorithm is generally much faster, although memory usage is much higher (for the array). Storing the array on the heap is in this case very beneficial to avoid memory problems.

## Assignment 4
*Explain how you get the pointer addresses to the two char arrays (`text1` and `text2`), and the counter variable (`count`) in function `work()`.*

Since arrays in C are pointers to the first element in the array by default, the array name for the char arrays was sufficient to get their addresses. The address for the `count` variable was accessed with the "address of" ampersand (&) operator.

*What does it mean to increment a pointer? What is the difference between incrementing the pointer that points to the ASCII text string, and incrementing the pointer that points to the integer array? In what way is the assembler code and the C code different?*

Incrementing a pointer simply makes the pointer point to the next address in memory. Incrementing the ASCII text string pointer increases the value of the pointer address by one (since every character takes up one byte in memory) and incrementing the pointer that points to the integer array increases the memory address by four (since every integer takes up four bytes in memory).

One difference between the C code and the Assembly code is that when you increment a pointer, you do not have to specify by how many bytes it should be incremented. This is decided by what type the pointer is casted to. In Assembly we state that the pointer for the list should increment by 4 bytes, since that is one int. In C, we simply write that the pointer increments by one, and because it is an int it will increment by 4 bytes automatically.

*What is the difference between incrementing a pointer and incrementing a variable that a pointer points to? Explain how your code is incrementing the `count` variable.*

When you increment a pointer the pointer proceeds to point to the next address in memory, whereas incrementing the variable that the pointer points to simply increases the value of that variable (the address of the variable is not changed). Since `count` is passed by reference to the `copycodes` function, it is incremented by dereferencing the counter variable.

*Explain a statement in your code where you are dereferencing a pointer. What does this mean? Explain by comparing with the corresponding assembler code.*

When you dereference a pointer, you are going to the memory address that the pointer points to and retrieving the value. It is a way to access a variable's value having only the pointer to that variable. We are for example doing this when we are incrementing the value of the `count` variable. The corresponding Assembly code for this operation has us loading the counter variable and then storing it again for reuse when `copycodes` is called once more.

*Is your computer using big-endian or little-endian? How did you come to your conclusion? Is there any benefit of using either of the two alternatives?*

Our computers are using little-endian. We can see this with the `endian_proof` function. Since the first character being printed in the function is the first element of the char array that represents the address of `count`, the lsb of that address are stored in byteplace 0. Therefore the computer is using little-endian. The only difference is in byte addressing, so there is no inherent benefit to using one over the other. One could maybe argue that one is more intuitive than the other, but it is mostly personal preference.

## Assignment 5
**Questions:**

* _Consider AM18, AM19, and AF1. Explain why gv ends up with the incremented value, but m does not._
* _Pointer cp is a character pointer that points to a sequence of bytes. What is the size of the cp pointer itself?_
* _Explain how a C string is laid out in memory. Why does the character string that cp points to have to be 9 bytes?_
* _Which addresses have fun and main? Which sections are they located in? What kind of memory are they stored in? What is the meaning of the data that these symbols points to?_

Non essential questions:

* _Which addresses are variables in and gv located at? Which memory sections according to the PIC32 memory map? Why?_
* _Variables p and m are not global variables. Where are they allocated? Which memory section is used for these variables? Why are the address numbers for p and m much larger than for in and gv?_
* _At print statement AM5, what is the address of pointer p, what is the value of pointer p, and what value is pointer p pointing to?_
* _At print statement AM7, what is the address of pointer p, what is the value of pointer p, and what value is pointer p pointing to?_
* _Consider AM14 to AM17. Is the PIC32 processor using big-endian or little-endian? Why?_

**Answers:**

* `gv` is global and `m` is local. `m` is only incremented in the fun function.
* the `cp` pointer is as big as the system it is run on, which is 4 bytes (or 32 bits) on the chipkit.
* A string in C is an array of characters laid out consecutively in memory. The character string that `cp` points to needs to be 9 bytes long because it contains 9 chars (which take up one byte each in memory).
* The `fun` function is in address 0x9D001180 and `main` is in address 0x9D0011D8. They are in the program flash section. They are stored in KSEG0 memory, which is cacheable memory.

Non essential answers:

* `in` is stored at address 0xA0000008 and `gv` is stored at address 0xA00000C, they are both in internal RAM.
* `p` is in 0xA0003FE8 and `m` is in 0xA0003FE4. They are in the reserved segment in KSEG1.
* Address for pointer `p` in AM5 is 0xA0003FE8. The value of `p` is 0xA0003FE4 which is the address for the variable `m` (containing value 7).
* Address for pointer `p` in AM7 is 0xA0003FE8. The value of `p` is 0xA0003FE4 which is the address for the variable `m` (containing value 8).
* It uses little-endian. We can see this because the LSB from pointer `p` is in byte address 0.
