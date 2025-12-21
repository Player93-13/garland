#include <FastLED.h>
#include "color.h"
#include "palette.h"
#include "anim.h"
#include "config.h"

extern LastState State;

Color PalCustom_ [64];
Palette PalCustom = { 1, PalCustom_ };

Color *leds;
CRGB leds_FastLed[LEDS_ALL];

//video
uint8_t wallBytes[WALL * 3];
Color (*matrix)[WALL_WIDTH][WALL_HEIGHT] = (Color (*)[WALL_WIDTH][WALL_HEIGHT]) wallBytes;
bool wallBytesReady = false;
bool runWallVideo = false;

uint16_t frames = 0;
unsigned long secTime;

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
}

void Anim::setPaletteById(int id)
{
  this->setPalette(pals[id]);
}

bool Anim::run()
{
  if (off) {
    digitalWrite(PIN_PS_ON, HIGH);
    return false;
  }
  else {
    digitalWrite(PIN_PS_ON, LOW);
  }
  
  if ( millis() < nextms || off || (runWallVideo && !wallBytesReady)) {
    return false;
  }

  nextms = millis() + (runWallVideo ? 0 : period);

  if (secTime < millis())
  {
    Serial.print("fps: ");
    Serial.println(frames);
    frames = 0;
    secTime += 1000;
  }
  frames++;

  if (runImpl != NULL)
  {
    (this->*runImpl)();
  }

  if (runWallVideo) {
    wallBytesReady = false;
  }

  int transc = 0;
  bool tran = millis() < transms;

  if (tran)
  {
    transc = ((long)transms - (long)millis()) * 255 / TRANSITION_MS;
  }

  Color * leds_prev = (leds == leds1) ? leds2 : leds1;

  for (int i = 0; i < LEDS_ALL; i++)
  {
    Color c = leds[i];

    if (tran)  //transition is in progress
    {
      c = c.interpolate(leds_prev[i], transc);
    }

    if (!runWallVideo) {
      c.gammaCorrection();
    }

    if(i >= GARL && i < STAR + GARL) //смена порядка цветов, и обратный ход для звезды
    {
      leds_FastLed[(STAR + GARL) - (i - GARL) - 1] = CRGB(c.g, c.r, c.b);
    }
    else
    {
      leds_FastLed[i] = CRGB(c.r, c.g, c.b);
    }
  }
  
  FastLED.show();

  return true;
}

void Anim::setUp()
{
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
  runWallVideo = false;

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
    case 9:
      setUpImpl = &Anim::animAurora_SetUp;
      runImpl = &Anim::animAurora_Run;
      setUpOnPalChange = true;
      break;
    case 10:
      setUpImpl = &Anim::animWarmHug_SetUp;
      runImpl = &Anim::animWarmHug_Run;
      setUpOnPalChange = true;
      break;
    case 11:
      setUpImpl = &Anim::animFallingSnow_SetUp;
      runImpl = &Anim::animFallingSnow_Run;
      setUpOnPalChange = true;
      break;
    case 12:
      setUpImpl = &Anim::animHeartbeat_SetUp;
      runImpl = &Anim::animHeartbeat_Run;
      setUpOnPalChange = true;
      break;
    case ANIM_FILL_ID:
      setUpImpl = &Anim::animFill_SetUp;
      runImpl = &Anim::animFill_Run;
      setUpOnPalChange = true;
      break;
    case ANIM_VIDEO_ID:
      setUpImpl = &Anim::animVideo_SetUp;
      runImpl = &Anim::animVideo_Run;
      setUpOnPalChange = true;
      runWallVideo = true;
      break;
    case ANIM_CALIBRATE_ID:
      setUpImpl = &Anim::animCalibrate_SetUp;
      runImpl = &Anim::animCalibrate_Run;
      setUpOnPalChange = true;
      break;
    default:
      setUpImpl = &Anim::animOff_SetUp;
      runImpl = &Anim::animOff_Run;
      setUpOnPalChange = true;
      break;
  }
}

Color Anim::getMatrix(int i, int j)
{
  return (*matrix)[i][j];
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

Color Anim::GetGradientColor(int pos, uint8_t colorOffset, int paletteCut)
{
  uint8_t x = ((256 * pos / LEDS_ALL / paletteCut) + colorOffset) % 256;

  return palette->getPalColor(x);
}

Anim anim = Anim();

void AnimSetup()
{
  FastLED.addLeds<APA106, PIN_ORANGE_2, GRB>(leds_FastLed, GARL + STAR, WALL);
  FastLED.addLeds<APA106, PIN_ORANGE_1, GRB>(leds_FastLed, 0, GARL1);
  FastLED.addLeds<APA106, PIN_BLUE_1, GRB>(leds_FastLed, GARL1, GARL2);
  FastLED.addLeds<APA106, PIN_GREEN_1, GRB>(leds_FastLed, GARL1 + GARL2, GARL3 + STAR);

  //FastLED.addLeds<APA106, PIN_BROWN_1>(leds_FastLed, GARL, STAR);
  
  LoadConfig();
  anim.setAnim(State.animId);
  anim.setPaletteById(State.palId);
  anim.setPeriod(20);
  anim.doSetUp();
}

Color Anim::leds1[LEDS_ALL];
Color Anim::leds2[LEDS_ALL];
Color Anim::ledstmp[LEDS_ALL];
byte Anim::seq[LEDS_ALL];
uint16_t Anim::positions[LEDS_ALL];
