#include "anim.h"
#include "color.h"
#include "palette.h"

void Anim::animRun_SetUp() {
  pos = 0;
  inc = 1 + (rngb() >> 5);
  if (random(10) > 5)
    inc = -inc;
}

void Anim::animRun_Run() {

  int p = pos;
  for (int i = 0; i < LEDS_ALL; i++)
  {
    Color c = palette->getPalColor(p);
    leds[i] = c;

    p += inc;
    if (p >= 256)
      p = p - 256;
    else if (p < 0)
      p = p + 256;
  }
  pos = pos + 1;
}
