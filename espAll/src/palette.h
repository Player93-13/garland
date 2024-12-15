#ifndef palette_h
#define palette_h

#include "color.h"


struct Palette
{
  int numColors;
  Color *colors;

  Color getPalColor(uint8_t i)
  {
    int k = ((int)i * numColors) >> 8;
    int i0 = (k) % (numColors);
    int i1 = (k + 1) % (numColors);

    uint8_t t0 = (numColors * i) - (i0 << 8);

    return colors[i0].interpolate(colors[i1], t0);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Palette definitions
////////////////////////////////////////////////////////////////////////////////
extern Palette PalRgb;
extern Palette PalRainbow;
extern Palette PalRainbowStripe;
extern Palette PalParty;
extern Palette PalHeat;
extern Palette PalFire;
extern Palette PalIceBlue;

#endif
