#include <Adafruit_NeoPixel.h>
#include "color.h"
#include "palette.h"
#include "anim.h"

//Adafruit's class to operate strip
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

Color PalCustom_ [64];
Palette PalCustom = { 1, PalCustom_ };

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue, &PalCustom};

Anim::Anim()
{
  nextms = millis();
}

void Anim::setPeriod(byte period) {
  this->period = period;
}

void Anim::setPalette(Palette * pal) {
  this->palette = pal;
  if (setUpOnPalChange) {
    setUp();
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void Anim::setPaletteById(int id)
{
  this->setPalette(pals[id]);
}

bool Anim::run()
{
  if ( millis() <= nextms || off) {
    //digitalWrite(LED_BUILTIN, LOW);
    return false;
  }
  //digitalWrite(LED_BUILTIN, HIGH);
  nextms = millis() + period;

  if (runImpl != NULL)
  {
    (this->*runImpl)();
  }

  //transition coef, if within 0..1 - transition is active
  //changes from 1 to 0 during transition, so we interpolate from current color to previous
  float transc = (float)((long)transms - (long)millis()) / TRANSITION_MS;
  Color * leds_prev = (leds == leds1) ? leds2 : leds1;

  bool tran = transc > 0;

  for (int i = 0; i < LEDS; i++)
  {
    Color c = leds[i];
    
    if (tran)  //transition is in progress
      c = c.interpolate(leds_prev[i], transc);     

    c.setbrightness(i < GARL ? BRIGHTNESS : STARBRIGHTNESS);
    c.gammaCorrection();

    if (i < GARL)
      pixels.setPixelColor(i, pixels.Color(c.r, c.g, c.b));
    else
      pixels.setPixelColor(LEDS - (i - GARL) - 1, pixels.Color(c.g, c.r, c.b));
  }

  pixels.show();

  return true;
}

void Anim::setUp()
{
  //pinMode(LED_BUILTIN, OUTPUT);
  transms = millis() + TRANSITION_MS;

  //switch operation buffers (for transition to operate)

  if (leds == leds1) {
    leds = leds2;
  } else {
    leds = leds1;
  }

  if (setUpImpl != NULL) {
    (this->*setUpImpl)();
  }
}

void Anim::doSetUp()
{
  if (!setUpOnPalChange) {
    setUp();
  }
}

void Anim::setAnim(byte animInd)
{
  off = false;
  
  switch (animInd) {
    case 0:
      setUpImpl = &Anim::animRun_SetUp;
      runImpl = &Anim::animRun_Run;
      setUpOnPalChange = true;
      break;
    case 1:
      setUpImpl = &Anim::animPixieDust_SetUp;
      runImpl = &Anim::animPixieDust_Run;
      setUpOnPalChange = true;
      break;
    case 2:
      setUpImpl = &Anim::animPixieDustGrad_SetUp;
      runImpl = &Anim::animPixieDustGrad_Run;
      setUpOnPalChange = true;
      break;
    case 3:
      setUpImpl = &Anim::animSparkr_SetUp;
      runImpl = &Anim::animSparkr_Run;
      setUpOnPalChange = true;
      break;
    case 4:
      setUpImpl = &Anim::animSparkrGrad_SetUp;
      runImpl = &Anim::animSparkrGrad_Run;
      setUpOnPalChange = true;
      break;
    case 5:
      setUpImpl = &Anim::animRandCyc_SetUp;
      runImpl = &Anim::animRandCyc_Run;
      setUpOnPalChange = true;
      break;
    case 6:
      setUpImpl = &Anim::animRandCycGlow_SetUp;
      runImpl = &Anim::animRandCycGlow_Run;
      setUpOnPalChange = true;
      break;
    case 7:
      setUpImpl = &Anim::animStars_SetUp;
      runImpl = &Anim::animStars_Run;
      setUpOnPalChange = true;
      break;
    case 8:
      setUpImpl = &Anim::animSpread_SetUp;
      runImpl = &Anim::animSpread_Run;
      setUpOnPalChange = true;
      break;
    case ANIM_FILL_ID:
      setUpImpl = &Anim::animFill_SetUp;
      runImpl = &Anim::animFill_Run;
      setUpOnPalChange = true;
      break;
    default:
      setUpImpl = &Anim::animOff_SetUp;
      runImpl = &Anim::animOff_Run;
      setUpOnPalChange = true;
      break;
  }
}



unsigned int rng() {
  static unsigned int y = 0;
  y += micros(); // seeded with changing number
  y ^= y << 2; y ^= y >> 7; y ^= y << 7;
  return (y);
}

byte rngb() {
  return (byte)rng();
}

Color Anim::GetGradientColor(int pos, float colorOffset, int paletteCut)
{
  float x = ((float)pos / LEDS / paletteCut) + colorOffset;
  if (x >= 1)
    x--;
    
  return palette->getPalColor(x);
}

Anim anim = Anim();

void AnimSetup()
{
  pixels.begin();
  anim.setAnim(-1);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
}

Color Anim::leds1[LEDS];
Color Anim::leds2[LEDS];
Color Anim::ledstmp[LEDS];
byte Anim::seq[LEDS];
