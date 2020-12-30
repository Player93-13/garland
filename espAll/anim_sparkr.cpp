#include "anim.h"

void AnimSparkr_initSeq(byte * seq)
{
    for (int i=0; i<LEDS; i++) {
        seq[i] = i;
    }
}

void AnimSparkr_shuffleSeq(byte * seq) 
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

void Anim::animSparkr_SetUp() {
    glowSetUp();
    phase = 0;
    curColor = palette->getPalColor((float)rngb()/256);
    prevColor = palette->getPalColor((float)rngb()/256);
    AnimSparkr_initSeq(seq);
    AnimSparkr_shuffleSeq(seq);


}

void Anim::animSparkr_Run() {
    for (int i=0;i<LEDS;i++) {
        byte pos = seq[i];

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
        AnimSparkr_shuffleSeq(seq);
    }
}
