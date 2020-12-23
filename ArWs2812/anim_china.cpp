#include "anim.h"
#include "color.h"
#include "palette.h"

void Anim::animChina_SetUp()
{
  for (int i = 0; i < LEDS; i++) {
    seq[i] = rngb();
  }
  glowSetUp();
}

void Anim::animChina_Run()
{
  for (int i = 0; i < LEDS; i++) {
    leds[i] = palette->getPalColor((float)seq[i] / 256);
    seq[i] += rngb() >> 6;
    glowForEachLed(i);
  }

  glowRun();
}
