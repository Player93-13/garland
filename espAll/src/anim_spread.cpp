#include "anim.h"
#include "color.h"
#include "palette.h"

#define SPREAD_MAX_WIDTH 50

void Anim::animSpread_SetUp() {
    inc = random(2,8);
    memset(seq, 0, LEDS_ALL);
}

void Anim::animSpread_Run() {
    memset(leds, 0, 3*LEDS_ALL);

    for (int i=0;i<LEDS_ALL;i++) {
        if (seq[i] > 0) {
            int width = SPREAD_MAX_WIDTH - seq[i];
            for (int j=i-width;j<=(i+width);j++) {
                Color c = ledstmp[i];
                if (j>=0 && j<LEDS_ALL) {
                    leds[j].r += c.r;
                    leds[j].g += c.g;
                    leds[j].b += c.b;
                }
            }
            ledstmp[i].fade(255/SPREAD_MAX_WIDTH);
            seq[i]--;
        }
    }

    if (random(inc) == 0) {
        int pos = random(0,LEDS_ALL); 
        ledstmp[pos] = palette->getPalColor(rngb());
        seq[pos] = SPREAD_MAX_WIDTH;
    }        
}
