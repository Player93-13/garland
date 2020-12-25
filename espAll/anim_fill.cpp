#include "anim.h"

void Anim::animFill_SetUp() {
  for (int i = 0; i < LEDS; i++) {
    leds[i] = palette->getPalColor(0);
  }
}

void Anim::animFill_Run() {

}
