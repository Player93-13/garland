#include "anim.h"

void Anim::animOff_SetUp() {
  for (int i = 0; i < LEDS; i++) {
    leds[i] = Color(0, 0, 0);
  }
  inc = 0;
}

void Anim::animOff_Run() {
    if(inc++ > 1000)
    {
      off = true;
    }
}
