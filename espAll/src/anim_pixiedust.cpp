#include "anim.h"
#include "color.h"
#include "palette.h"

#define DUST_LENGTH 20
void Anim::animPixieDust_SetUp() {
    phase = 0;
    curColor = palette->getPalColor(rngb());
    prevColor = palette->getPalColor(rngb());
    inc = random(2)*2-1;
    if (inc > 0) {
        phase = -DUST_LENGTH/2;
    } else {
        phase = LEDS_ALL + DUST_LENGTH/2;
    }
    glowSetUp();
}

void Anim::animPixieDust_Run() {

    if (inc > 0) {
        for (int i=0;i<LEDS_ALL;i++) {
            leds[i] = (i > phase) ? prevColor : curColor;
            glowForEachLed(i);
        }
        phase++;
        if (phase >= 4*LEDS_ALL) {
            phase = -DUST_LENGTH/2;
            prevColor = curColor;
            curColor = palette->getPalColor(rngb());     
        }
    } else {
        for (int i=0;i<LEDS_ALL;i++) {
            leds[i] = (i < phase) ? prevColor : curColor;
            glowForEachLed(i);
        }
        phase--;
        if (phase <= -3*LEDS_ALL) {
            phase = LEDS_ALL + DUST_LENGTH/2;
            prevColor = curColor;
            byte x = 0;
            while (prevColor.isCloseTo(curColor) && x++ < 50) 
            { 
              curColor = palette->getPalColor(rngb());     
            }
        }
    }
    glowRun();
    
    for (int k = phase-DUST_LENGTH/2; k < (phase + DUST_LENGTH/2); k++ ) {
        if (k >= 0 && k < LEDS_ALL) {
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
