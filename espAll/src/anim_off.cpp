#include "anim.h"

void Anim::animOff_SetUp() {
  for (int i = 0; i < LEDS_ALL; i++) {
    leds[i] = Color(0, 0, 0);
  }
  inc = 0;
}

void Anim::animOff_Run() {
    if(inc++ > 500)
    {
      off = true;
    }
}
