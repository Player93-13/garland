#ifndef anim_h
#define anim_h
#include "palette.h"

#define PIN_ORANGE_1 4  // garl 1
#define PIN_BLUE_1 2    // garl 2
#define PIN_GREEN_1 8   // garl 3
#define PIN_BROWN_1 10  // star
#define PIN_ORANGE_2 36 // wall
#define PIN_BLUE_2 38
#define PIN_GREEN_2 21
#define PIN_BROWN_2 17
#define PIN_PS_ON 14 // power switch


#define GARL1 400
#define GARL2 200
#define GARL3 300
#define GARL (GARL1 + GARL2 + GARL3)
#define STAR 11
#define WALL_WIDTH 16
#define WALL_HEIGHT 25
#define WALL (WALL_WIDTH * WALL_HEIGHT + 1)
#define WALL_OFFSET (GARL + STAR + 1)
#define LEDS_ALL (GARL + STAR + WALL)  // number of LEDs in the strip.

#define BRIGHTNESS 255// brightness adjustment, up to 255
#define STARBRIGHTNESS 215
#define WALLBRIGHTNESS 215

#define ANIMS 9 //number of animations
#define PALS 8 //number of palettes
#define PALCUSTOM_ID 7
#define ANIM_CALIBRATE_ID 98
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
    static Color leds1[LEDS_ALL];
    static Color leds2[LEDS_ALL];
    //auxiliary colors array
    static Color ledstmp[LEDS_ALL];
    
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

    static byte seq[LEDS_ALL];
    static uint16_t positions[LEDS_ALL];

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

    void animCalibrate_SetUp();
    void animCalibrate_Run();

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
