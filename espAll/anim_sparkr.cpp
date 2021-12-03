#include "anim.h"

void AnimSparkr_initPositions(uint16 * positions)
{
    for (int i=0; i<LEDS; i++) {
        positions[i] = i;
    }
}

void AnimSparkr_shufflePositions(uint16 * positions) 
{
    for (int i=0; i<LEDS; i++) {
        uint16 ind = (uint16)(rngb() * LEDS / 256);
        if (ind != i) {
            uint16 tmp = positions[ind];
            positions[ind] = positions[i];
            positions[i] = tmp;
        }
    }
}

void Anim::animSparkr_SetUp() {
    glowSetUp();
    phase = 0;
    curColor = palette->getPalColor((float)rngb()/256);
    prevColor = palette->getPalColor((float)rngb()/256);
    AnimSparkr_initPositions(positions);
    AnimSparkr_shufflePositions(positions);
}

void Anim::animSparkr_Run() {
    for (int i=0;i<LEDS;i++) {
        uint16 pos = positions[i];

        leds[pos] = (i > phase)
            ? prevColor 
            : (i > (phase - 2)) ? sparkleColor : curColor;
        glowForEachLed(pos);
    }
    glowRun();

    if (phase > LEDS) {
        if (random(SPARK_PROB) == 0) {
            int i = (int)rngb() * LEDS / 256;
            leds[i] = sparkleColor;
        }    
    }

    phase++;
    if (phase > 2*LEDS) {
        phase = 0;
        prevColor = curColor;
        byte x = 0;
        while (prevColor.isCloseTo(curColor) && x++ < 50) { 
          curColor = palette->getPalColor((float)rngb()/256);     
        }
        AnimSparkr_shufflePositions(positions);
    }
}
