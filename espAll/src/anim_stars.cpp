#include "anim.h"
#include "color.h"
#include "palette.h"

//seq keeps phases: 0..127 increasing, 128..255 decreasing, ends at 255 (steady off)
//ledstmp keeps color of stars

void Anim::animStars_SetUp() {
    //inc is (average) interval between appearance of new stars
    inc = random (1, 3);

    //reset all phases
    memset(seq, 255, LEDS_ALL); 
}

void Anim::animStars_Run() {   
    for (int i=0;i<LEDS_ALL;i++) {
        byte phi = seq[i];
        if (phi < 254) {
            Color col = ledstmp[i];
            if (phi <= 127) {
                leds[i] = col.brightness(phi << 1);
            } else {
                leds[i] = col.brightness((255-phi) << 1);
            }
            seq[i]+=2;
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    }

    if (random(inc) == 0) {
        int pos = random(LEDS_ALL);
        if (seq[pos] > 250) {
            seq[pos] = 0;
            ledstmp[pos] = palette->getPalColor(rngb());
        }
    }
}
