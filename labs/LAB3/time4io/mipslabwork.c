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

/* Interrupt Service Routine */
void user_isr(void) {
    return;
}

/* Lab-specific initialization goes here */
void labinit(void) {
    // volatile pointer to value at TRISE address
    volatile int *trise = (volatile int *)0xBF886100;
    // setting first 8 bits of TRISE to 0 and leaving the rest unchanged
    *trise &= 0xFFFFFF00;
    // initilalizing Port D bits 11 through 5 to inputs
    // TRISD = TRISD | 0x00000FE0;
    TRISDSET = 0x00000FE0;
}

/* This function is called repetitively from the main program */
void labwork(void) {
    // volatile pointer to value at PORTE address
    volatile int *porte = (volatile int *)0xBF886110;
    delay(1000);
    int btn = getbtns();
    int sw = getsw();
    // 001, 011, 101, 111 (BTN2)
    if (btn == 1 || btn == 3 || btn == 5 || btn == 7) {
        mytime &= 0xFF0F;
        mytime = (sw << 4)| mytime;
    }
    // 010, 011, 110, 111 (BTN3)
    if (btn == 2 || btn == 3 || btn == 6 || btn == 7) {
        mytime &= 0xF0FF;
        mytime = (sw << 8)| mytime;
    }
    // 100, 101, 110, 111 (BTN4)
    if (btn == 4 || btn == 5 || btn == 6 || btn == 7) {
        mytime &= 0x0FFF;
        mytime = (sw << 12)| mytime;
    }
    time2string(textstring, mytime);
    display_string(3, textstring);
    display_update();
    tick(&mytime);
    // increment value at PORTE (track one more tick on the LEDs)
    (*porte)++;
    display_image(96, icon);
}
