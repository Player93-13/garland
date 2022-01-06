#include "anim.h"

void Anim::animRandCyc_SetUp() {
  for (int i = 0; i < LEDS; i++) {
    seq[i] = rngb();
  }
}

void Anim::animRandCyc_Run() {
  for (int i = 0; i < LEDS; i++) {
    leds[i] = palette->getPalColor(seq[i]);
    seq[i] += ((rngb() >> 6) + 1);
  }
}
