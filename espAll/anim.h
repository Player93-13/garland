#ifndef anim_h
#define anim_h
#include <Adafruit_NeoPixel.h>
#include "palette.h"

#define PIN 5 // WS2812 pin number
#define PIN_WALL 4 // window wall pin
#define PIN_PS_ON 12 // power switch
#define GARL 494
#define STAR 11
#define WALL_WIDTH 16
#define WALL_HEIGHT 25
#define WALL (WALL_WIDTH * WALL_HEIGHT + 1)
#define WALL_OFFSET (GARL + STAR + 1)
#define LEDS (GARL + STAR + WALL)  // number of LEDs in the strip.

#define BRIGHTNESS 255// brightness adjustment, up to 255
#define STARBRIGHTNESS 215
#define WALLBRIGHTNESS 215

#define ANIMS 9 //number of animations
#define PALS 8 //number of palettes
#define PALCUSTOM_ID 7
#define ANIM_VIDEO_ID 99
#define ANIM_FILL_ID 100

#define TRANSITION_MS 500 // transition time between animations, ms

// brigthness animation amplitude shift. true BrA amplitude is calculated as (0..127) value shifted right by this amount
#define BRA_AMP_SHIFT 1
// brigthness animation amplitude offset
#define BRA_OFFSET (255-80)

//probability of spark when in idle plase
#define SPARK_PROB 3

class Anim {

  private:
    //Color arrays - two for making transition
    static Color leds1[LEDS];
    static Color leds2[LEDS];
    //auxiliary colors array
    static Color ledstmp[LEDS];
    
    void animStart();

    // length of animation timeslot (period)
    byte period;
    // array of Color to work with
    Color *leds;
    Palette *palette;

    // millis for next timeslot
    unsigned long nextms;
    // millis to transition end
    unsigned long transms;

    int phase;
    int pos;
    int inc;
    int palCut;

    bool off;

    //whether to call SetUp on palette change
    //(some animations require full transition with fade, otherwise the colors would change in a step, some not)
    bool setUpOnPalChange;

    Color curColor = Color(0);
    Color prevColor = Color(0);
    uint8_t curColorOffset = 127;
    uint8_t prevColorOffset = 0;

    Color sparkleColor = Color(0xFFFFFF);

    static byte seq[LEDS];
    static uint16_t positions[LEDS];

    //brigthness animation (BrA) current initial phase
    byte braPhase;
    //braPhase change speed
    byte braPhaseSpd = 5;
    //BrA frequency (spatial)
    byte braFreq = 150;

    //glow animation setup
    void glowSetUp();

    //glow animation - must be called for each LED after it's BASIC color is set
    //note this overwrites the LED color, so the glow assumes that color will be stored elsewhere (not in leds[])
    //or computed each time regardless previous leds[] value
    void glowForEachLed(int i);

    //glow animation - must be called at the end of each animaton run
    void glowRun();

    void setUp();

    //run and setup handlers
    void (Anim::*runImpl)();
    void (Anim::*setUpImpl)();


    //animation implementations
    void animStart_SetUp();
    void animStart_Run();

    void animRun_SetUp();
    void animRun_Run();

    void animPixieDust_SetUp();
    void animPixieDust_Run();

    void animPixieDustGrad_SetUp();
    void animPixieDustGrad_Run();

    void animSparkr_SetUp();
    void animSparkr_Run();

    void animSparkrGrad_SetUp();
    void animSparkrGrad_Run();

    void animRandCyc_SetUp();
    void animRandCyc_Run();

    void animRandCycGlow_SetUp();
    void animRandCycGlow_Run();

    void animStars_SetUp();
    void animStars_Run();

    void animSpread_SetUp();
    void animSpread_Run();

    void animFly_SetUp();
    void animFly_Run();

    void animOff_SetUp();
    void animOff_Run();

    void animFill_SetUp();
    void animFill_Run();

    void animVideo_SetUp();
    void animVideo_Run();

    Color GetGradientColor(int pos, uint8_t colorOffset, int paletteCut);

  public:
    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    void setPaletteById(int id);
    void setAnim(byte animInd);
    bool run();//returns true if actual change has completed, or false if it's dummy call (previous call was too recent in time)
    void doSetUp();

    Color getMatrix(int i, int j);
};

unsigned int rng();

byte rngb();

void AnimSetup();

#endif
