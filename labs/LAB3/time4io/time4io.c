/* Created by:
   William Berg & Gustav Henningsson */

#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

/* returns status of toggle-switches */
int getsw(void) {
  int switchState = (PORTD >> 8);
  return switchState & 0x0000000F;
}

/* returns status of push-buttons */
int getbtns(void) {
  int buttonState = (PORTD >> 5);
  return buttonState & 0x00000007;
}
