#include "anim.h"
#include "color.h"
#include "palette.h"

void Anim::animChina_SetUp()
{
  pos = 0;
  inc = random(80, 200);
  phase = 0;
}

void Anim::animChina_Run()
{
  if (phase++ % inc == 0)
  {
    int n = palette->numColors;
    for (int i = 0; i < LEDS; i++)
    {
      if ((i + pos) % n >= n / 2)
      {
        leds[i] = palette->colors[i % n];
      }
      else
      {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
      }
    }
    pos++;
  }
}
