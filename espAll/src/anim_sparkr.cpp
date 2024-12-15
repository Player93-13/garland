#include "anim.h"

void AnimSparkr_initPositions(uint16_t * positions)
{
    for (int i=0; i<LEDS; i++) {
        positions[i] = i;
    }
}

void AnimSparkr_shufflePositions(uint16_t * positions) 
{
    for (int i=0; i<LEDS; i++) {
        uint16_t ind = (uint16_t)(rngb() * LEDS / 256);
        if (ind != i) {
            uint16_t tmp = positions[ind];
            positions[ind] = positions[i];
            positions[i] = tmp;
        }
    }
}

void Anim::animSparkr_SetUp() {
    glowSetUp();
    phase = 0;
    curColor = palette->getPalColor(rngb());
    prevColor = palette->getPalColor(rngb());
    AnimSparkr_initPositions(positions);
    AnimSparkr_shufflePositions(positions);
}

void Anim::animSparkr_Run() {
    for (int i=0;i<LEDS;i++) {
        uint16_t pos = positions[i];

        if(i > phase)
        {
          leds[pos] = prevColor;
          glowForEachLed(pos);
        } 
        else if(i > (phase - 2))
        {
          leds[pos] = sparkleColor;
        } 
        else
        {
          leds[pos] = curColor;
          glowForEachLed(pos);
        }
    }
    glowRun();

    if (phase > LEDS) {
        if (random(SPARK_PROB) == 0) {
          int n = LEDS / 256 + 1;
          for(int i = 0; i < n; i++)
          {
            int k = (int)rngb() * LEDS / 256;
            leds[k] = sparkleColor;
          }
        }    
    }

    phase++;
    if (phase > 2*LEDS) {
        phase = 0;
        prevColor = curColor;
        byte x = 0;
        while (prevColor.isCloseTo(curColor) && x++ < 50) { 
          curColor = palette->getPalColor(rngb());     
        }
        AnimSparkr_shufflePositions(positions);
    }
}
