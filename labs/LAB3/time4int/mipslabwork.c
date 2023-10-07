/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2020-09-21 by Gustav Henningsson & William Berg.

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

int timeoutcount = 0;

int prime = 1234567;

/* Interrupt Service Routine, execute
this code when an interrupt occurs */
void user_isr(void) {
    timeoutcount++;
    if(timeoutcount == 10) {
        time2string(textstring, mytime);
        display_string(3, textstring);
        display_update();
        tick(&mytime);
        timeoutcount = 0;
      }
    // clear interrupt flag
    IFSCLR(0) = 0x100;
}

/* Lab-specific initialization goes here */
void labinit(void) {
    // volatile pointer to value at TRISE address
    volatile int *trise = (volatile int *)0xBF886100;
    // setting first 8 bits of TRISE to 0 and leaving the rest unchanged
    *trise &= 0xFFFFFF00;
    // initilalizing Port D bits 11 through 5 to inputs
    TRISDSET = 0x00000FE0;

    // initialize Timer 2
    T2CON = 0x0;                        // stop timer and clear con register
    TMR2 = 0;                           // start timer from 0
    T2CONSET = 0x70;                    // prescaling set to 1:256
    PR2 = ((80000000 / 256) / 10);      // period set to 31250

    // enable interrupts
    IPCSET(2) = 0x0000000C;     // set priority level = 3
    IPCSET(2) = 0x00000001;     // set subpriority level = 1
    IFSCLR(0) = 0x100;          // clear the timer interrupt status flag
    IECSET(0) = 0x100;          // enable timer interrupts
    enable_interrupt();         // enable timer interrupts globally

    // start the timer
    T2CONSET = 0x8000;
}

/* This function is called repetitively from the main program */
void labwork(void) {
    prime = nextprime(prime);
    display_string(0, itoaconv(prime));
    display_update();
}
