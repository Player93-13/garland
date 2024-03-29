#ifndef color_h
#define color_h
#include <Arduino.h>
#include "brightness.h"

struct Color
{
  union
  {
    struct
    {
      byte r;
      byte g;
      byte b;
    };
    byte raw[3];
  };

  inline Color() __attribute__((always_inline)) : r(0), g(0), b(0) {}

  // allow construction from R, G, B
  inline Color( uint8_t ir, uint8_t ig, uint8_t ib)  __attribute__((always_inline))
    : r(ir), g(ig), b(ib)
  {
  }

  // allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
  inline Color( uint32_t colorcode)  __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
  {
  }

  Color interpolate(Color color, uint8_t x)
  {
    int r0 = (int)x * (color.r - r) / 255 + r;
    int g0 = (int)x * (color.g - g) / 255 + g;
    int b0 = (int)x * (color.b - b) / 255 + b;
    return Color(r0, g0, b0);
  }

  //creates color with decreased brightness
  Color brightness(byte k) {
    int r0 = r * (int)k / 255;
    int g0 = g * (int)k / 255;
    int b0 = b * (int)k / 255;
    return Color(r0, g0, b0);
  }

  void gammaCorrection()
  {
    r = pgm_read_byte_near(BRI + r);
    g = pgm_read_byte_near(BRI + g);
    b = pgm_read_byte_near(BRI + b);
  }

  void setbrightness(byte k)
  {
    int r0 = r * (int)k / 255;
    int g0 = g * (int)k / 255;
    int b0 = b * (int)k / 255;
    r = r0;
    g = g0;
    b = b0;
  }

  //fades (decreases all RGB channels brightness) this color by k
  void fade(byte k)
  {
    if (r >= k) {
      r = r - k;
    } else {
      r = 0;
    }
    if (g >= k) {
      g = g - k;
    } else {
      g = 0;
    }
    if (b >= k) {
      b = b - k;
    } else {
      b = 0;
    }
  }

  //fades color separately for each channel
  void fade3(byte dr, byte dg, byte db)
  {
    if (r >= dr) {
      r = r - dr;
    } else {
      r = 0;
    }
    if (g >= dg) {
      g = g - dg;
    } else {
      g = 0;
    }
    if (b >= db) {
      b = b - db;
    } else {
      b = 0;
    }
  }

  //checks whether this color is visually close to given one
  byte isCloseTo(Color c) {
    int diff = abs(r - c.r) + abs(g - c.g) + abs(b - c.b);
    return diff <= 220; //220 is magic number. Low values give "true" on closer colors, while higher can cause infinite loop while trying to find different color
  }

  void println() {
    Serial.print(F("r=")); Serial.print(r);
    Serial.print(F("g=")); Serial.print(g);
    Serial.print(F("b=")); Serial.println(b);
  }
} ;

#endif
