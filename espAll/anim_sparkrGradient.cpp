#include "anim.h"

void AnimSparkrGrad_initPositions(uint16_t * positions)
{
    for (int i=0; i<LEDS; i++) {
        positions[i] = i;
    }
}

void AnimSparkrGrad_shufflePositions(uint16_t * positions) 
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

void Anim::animSparkrGrad_SetUp() {
    glowSetUp();
    phase = 0;
    curColorOffset = 0,5;
    prevColorOffset = 0;
    palCut = random(1,4);
    AnimSparkrGrad_initPositions(positions);
    AnimSparkrGrad_shufflePositions(positions);
}

void Anim::animSparkrGrad_Run() {
    for (int i=0;i<LEDS;i++) {
        int pos = positions[i];

        leds[pos] = (i > phase)
            ? GetGradientColor(pos, prevColorOffset, palCut) 
            : (i > (phase - 2)) ? sparkleColor : GetGradientColor(pos, curColorOffset, palCut);
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
        prevColorOffset = curColorOffset;
        curColorOffset = (float)rngb()/256;
        AnimSparkrGrad_shufflePositions(positions);
    }
}
