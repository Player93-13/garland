#include "anim.h"

void AnimSparkrGrad_initSeq(byte * seq)
{
    for (int i=0; i<LEDS; i++) {
        seq[i] = i;
    }
}

void AnimSparkrGrad_shuffleSeq(byte * seq) 
{
    for (int i=0; i<LEDS; i++) {
        byte ind = (unsigned int) ( rngb() * LEDS / 256);
        if (ind != i) {
            byte tmp = seq[ind];
            seq[ind] = seq[i];
            seq[i] = tmp;
        }
    } 
}

void Anim::animSparkrGrad_SetUp() {
    glowSetUp();
    phase = 0;
    curColorOffset = 0,5;
    prevColorOffset = 0;
    palCut = random(1,4);
    AnimSparkrGrad_initSeq(seq);
    AnimSparkrGrad_shuffleSeq(seq);
}

void Anim::animSparkrGrad_Run() {
    for (int i=0;i<LEDS;i++) {
        byte pos = seq[i];

        leds[pos] = (i > phase)
            ? GetGradientColor(pos, prevColorOffset, palCut) 
            : (i == phase) ? sparkleColor : GetGradientColor(pos, curColorOffset, palCut);
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
        AnimSparkrGrad_shuffleSeq(seq);
    }
}
