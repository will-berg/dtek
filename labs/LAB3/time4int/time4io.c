/* Created by:
   William Berg & Gustav Henningsson */

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void) {
  int switchState = PORTD;
  switchState >> 8;
  switchState & 0x000F;
  return switchState;
}

int getbtns(void) {
  int buttonState = PORTD;
  buttonState >> 5;
  buttonState & 0x0007;
  return buttonState;
}
