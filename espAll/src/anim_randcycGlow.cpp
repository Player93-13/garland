#include "anim.h"
#include "color.h"
#include "palette.h"

void Anim::animRandCycGlow_SetUp()
{
  for (int i = 0; i < LEDS_ALL; i++) {
    seq[i] = rngb();
  }
  glowSetUp();
}

void Anim::animRandCycGlow_Run()
{
  for (int i = 0; i < LEDS_ALL; i++) {
    leds[i] = palette->getPalColor(seq[i]);
    seq[i] += (rngb() >> 6) + 1;
    glowForEachLed(i);
  }

  glowRun();
}
