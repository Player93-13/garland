#include "anim.h"

void Anim::animFill_SetUp() {
  Color c = palette->getPalColor(0);
  for (int i = 0; i < LEDS_ALL; i++) {
    leds[i] = c;
  }
}

void Anim::animFill_Run() {

}
