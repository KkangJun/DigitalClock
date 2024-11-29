#include "_led.h"

void LED_Clear() {
  L_RED_OFF();
  L_GREEN_OFF();
  L_BLUE_OFF();
  R_RED_OFF();
  R_GREEN_OFF();
  R_BLUE_OFF();
}