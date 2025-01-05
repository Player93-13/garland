#include "anim.h"
#include "color.h"
#include "palette.h"


void Anim::animFly_SetUp() {
    //length of particle tail
    pos = random(2, 15);
    //probability of the tail
    inc = random(5, 15);
    if (random(10) > 5) {
        inc = -inc;
    }
    phase = 0;
}

void Anim::animFly_Run() {
    
    byte launchpos;
    if (inc > 0) {
        launchpos = 0;
        for (int i=1;i<LEDS_ALL;i++) {
            leds[i-1] = leds[i];
        }
    } else {
        launchpos = 0;
        for (int i=LEDS_ALL-2;i>=0;i--) {
            leds[i+1] = leds[i];
        }
    }

    if (random(abs(inc)) == 0) {
        curColor = palette->getPalColor(rngb());
        phase = pos;
    }

    leds[launchpos] = Color( (int)curColor.r * phase / pos, (int)curColor.g * phase / pos, (int)curColor.b * phase / pos) ;
    if (phase > 0) phase--; 
}
