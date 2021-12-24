#include "anim.h"
#include "color.h"
#include "palette.h"

#define DUST_LENGTH 20
void Anim::animPixieDustGrad_SetUp() {
    phase = 0;
    curColorOffset = 128;
    prevColorOffset = 0;
    palCut = random(1,4);
    inc = random(2)*2-1;
    if (inc > 0) {
        phase = -DUST_LENGTH/2;
    } else {
        phase = LEDS + DUST_LENGTH/2;
    }
    glowSetUp();
}

void Anim::animPixieDustGrad_Run() {

    if (inc > 0) {
        for (int i=0;i<LEDS;i++) {
            leds[i] = (i > phase) ? GetGradientColor(i, prevColorOffset, palCut) : GetGradientColor(i, curColorOffset, palCut);
            glowForEachLed(i);
        }
        phase++;
        if (phase >= 4*LEDS) {
            phase = -DUST_LENGTH/2;
            prevColorOffset = curColorOffset;
            curColorOffset = (float)rngb()/256;    
        }
    } else {
        for (int i=0;i<LEDS;i++) {
            leds[i] = (i < phase) ? GetGradientColor(i, prevColorOffset, palCut) : GetGradientColor(i, curColorOffset, palCut);
            glowForEachLed(i);
        }
        phase--;
        if (phase <= -3*LEDS) {
            phase = LEDS + DUST_LENGTH/2;
            prevColorOffset = curColorOffset;
            curColorOffset = rngb();    
        }
    }
    glowRun();
    
    for (int k = phase-DUST_LENGTH/2; k < (phase + DUST_LENGTH/2); k++ ) {
        if (k >= 0 && k < LEDS) {
            int mix = abs(k-phase) * 255 / DUST_LENGTH + random(-100, 100);
            if (mix < 0) { 
                mix = 0;
            } else if (mix > 255) {
                mix = 255;
            }
            leds[k] = sparkleColor.interpolate(leds[k], mix);
        }
    }
    
}
