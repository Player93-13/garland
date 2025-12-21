#include "anim.h"
#include "color.h"
#include "palette.h"

#define SPREAD_MAX_WIDTH 50

void Anim::animSpread_SetUp() {
    inc = random(2,8);
    memset(seq, 0, LEDS_ALL);
    memset(ledstmp, 0, sizeof(Color) * LEDS_ALL);
}

void Anim::animSpread_Run() {
    memset(leds, 0, sizeof(Color) * LEDS_ALL);

    for (int i=0;i<LEDS_ALL;i++) {
        if (seq[i] > 0) {
            int width = SPREAD_MAX_WIDTH - seq[i];
            int start = max(0, i - width);
            int end = min(LEDS_ALL-1, i + width);
            for (int j = start; j <= end; j++) {
                Color c = ledstmp[i];
                if (j>=0 && j<LEDS_ALL) {
                    uint16_t t = leds[j].r + c.r;
                    leds[j].r = (t > 255) ? 255 : t;
                    t = leds[j].g + c.g;
                    leds[j].g = (t > 255) ? 255 : t;
                    t = leds[j].b + c.b;
                    leds[j].b = (t > 255) ? 255 : t;
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
