# Lab 3 - Input/Output Programming

## Assignment 1: Polling switches
### Test pressing BTN3 and BTN2 at the same time. What happens? Why?
It changes both values to the the number specified by the switches. Because our if statements check all possibilities. `getbtns()` will return the binary value 011 (decimal 3) and will enter the two first if statements where it will change both the 2nd and 3rd number in `mytime`.

### Three device-registers for input/output control are TRISE, TRISESET, and TRISECLR. Their functions are related. How? What are the differences?
The `TRISE` register determines whether the specified pins of port E are output pins (bit value 0) or input pins (bit value 1).
A value written to the `TRISESET` register reads the `TRISE` register and sets any bits specified as 1 and writes the modified value back to the `TRISE` register.
A value written to the `TRISECLR` register reads the `TRISE` register and clears any bits specified as 1 and writes the modified value back to the `TRISE` register.
`TRISECLR` and `TRISESET` both act on the `TRISE` register (`TRISECLR` sets outputs and `TRISESET` sets inputs).
```c
TRISDSET = 0x00000FE0;      // will initialize port D pins 11-5 as inputs
TRISDCLR = 0x00000FE0;      // will clear port D pins 11-5 to 0 (turn all pins with value 1 to 0)
```

### In the generated assembly code, in which MIPS register will the return values from functions getbtns and getsw be placed in. You should be able to answer this question without debugging the generated assembly code.
Return values are placed in the `\$v0` register.

### In this exercise, we explained which bits that should be used in Port D and Port E. How can you find this information in the PIC32 and ChipKIT manuals? Be prepared to demonstrate how to find this information in the manuals. Advice: check the lecture slides from lecture 5 for ideas.
By looking at both the *chipKIT Basic I/O Shield Pinout Table* and the *Pinout Table by Logical Number* table, we can see what button/switch corresponds to what bit. For example:
SW1 has Pin# 2 according to the first table. When looking up that pin number in the second table we can see that it corresponds to the RD8 PIC32 signal. Meaning that TRISD and PORTD with bit index 8 should be used (just like it says in the question).


## Assignment 2: Timer
### When the time-out event-flag is a "1", how does your code reset it to "0"?
The flag is reset using the IFSCLR(0) definition from the header file `IFSCLR(0) = 0x100;`. It clears all values set to 1 (to 0); in this case that would be the 8th bit (the bit in IFS(0) where the time-out interrupt flag for Timer 2 is stored). IFSCLR(0) is used because it contains the Timer 2 interrupt flag.

### What would happen if the time-out event-flag was not reset to "0" by your code? Why?
There would be no more time out events and the desired delay would not be there after the first time-out. This is because after the first time out the flag is always set and the timer won't reset.

### Which device-register (or registers) must be written to define the time between time-out events? Describe the function of that register (or of those registers).
The period register defines the number that the timer should count to before timing out. You must also configure the prescaling with T2CON (bits 4-6). The prescaling was chosen to be 1:256 so that the period would fit in the 16-bit (PR2) period register.

### If you press BTN3 quickly, does the time update reliably? Why, or why not? If not, would that be easy to change? If so, how?
It will only update the time when it gets to the next tick. This is because it only displays the time every tenth timeout. We could perhaps add the display functions to every if statement where the button and switch states are checked so that the change was made immediately.


## Assignment 3: Interrupts
### When the time-out event-flag is a "1", how does your code reset it to "0"?
The interrupt flag is cleared like the time-out flag; with `IFSCLR(0)` and by specifying what bit we want to clear `IFSCLR(0) = 0x100;`.

### What would happen if the time-out event-flag was not reset to "0" by your code? Why?
There would be no more time out events and the desired delay would not be there after the first time-out. This is because after the first time out the flag is always set and the timer won't reset.  
Other than that, prime will stop ticking as well. This is because the `user_isr()` function will keep being called (since the flag is forever set and that function is called every time the flag is set). This means that the code within that function will keep on going (the time will keep getting displayed, but without the period in between) but the code in ``labwork()`` will not. 

### From which part of the code is the function user_isr called? Why is it called from there?
It is called in the vectors assembly file.

### Why are registers saved before the call to  user_isr? Why are only some registers saved?
Registers need to be saved before handling an interrupt since the interrupt handler might use those registers. Only the caller saved registers are saved in the caller since callee saved registers should be saved in the callee, following MIPS calling conventions.

### Which device-register (or registers), and which processor-register (or registers) must be written to enable interrupts from the timer? Describe the functions of the relevant registers.
The device register IEC (Interrupt Enable Control) must be written to by setting the bit for the interrupt that should be enabled (depends on the timer that is used). The interrupt must also be given a priority and subpriority with the IPC (Interrupt Priority Control) register.
