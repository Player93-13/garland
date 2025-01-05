#include "anim.h"

void AnimSparkrGrad_initPositions(uint16_t * positions)
{
    for (int i=0; i<LEDS_ALL; i++) {
        positions[i] = i;
    }
}

void AnimSparkrGrad_shufflePositions(uint16_t * positions) 
{
    for (int i=0; i<LEDS_ALL; i++) {
        uint16_t ind = (uint16_t)(rngb() * LEDS_ALL / 256);
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
    curColorOffset = 127;
    prevColorOffset = 0;
    palCut = random(1,4);
    AnimSparkrGrad_initPositions(positions);
    AnimSparkrGrad_shufflePositions(positions);
}

void Anim::animSparkrGrad_Run() {
    for (int i=0;i<LEDS_ALL;i++) {
        int pos = positions[i];

        leds[pos] = (i > phase)
            ? GetGradientColor(pos, prevColorOffset, palCut) 
            : (i > (phase - 2)) ? sparkleColor : GetGradientColor(pos, curColorOffset, palCut);
        glowForEachLed(pos);
    }
    glowRun();

    if (phase > LEDS_ALL) {
        if (random(SPARK_PROB) == 0) {
            int i = (int)rngb() * LEDS_ALL / 256;
            leds[i] = sparkleColor;
        }    
    }

    phase++;
    if (phase > 2*LEDS_ALL) {
        phase = 0;
        prevColorOffset = curColorOffset;
        curColorOffset = rngb();
        AnimSparkrGrad_shufflePositions(positions);
    }
}
