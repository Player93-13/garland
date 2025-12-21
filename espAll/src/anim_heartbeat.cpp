#include "anim.h"
#include "color.h"
#include "palette.h"

void Anim::animHeartbeat_SetUp() {
    period = 25;
    phase = 0;
    pos = LEDS_ALL / 2; // center
    inc = random(1,3);
    palCut = 1;
    glowSetUp();
}

void Anim::animHeartbeat_Run() {
    // create an expanding pulse from center
    phase += inc;
    int radius = (phase / 4) % (LEDS_ALL + 10);

    memset(leds, 0, sizeof(Color) * LEDS_ALL);

    for (int i = 0; i < LEDS_ALL; i++) {
        int dist = abs(i - pos);
        int d = abs(dist - radius);
        if (d < 30) {
            uint8_t fall = 255 - (d * 8);
            uint8_t x = (i * 3 + phase) & 0xFF;
            Color c = palette->getPalColor(x);
            leds[i].r = min(255, leds[i].r + (c.r * fall) / 255);
            leds[i].g = min(255, leds[i].g + (c.g * fall) / 255);
            leds[i].b = min(255, leds[i].b + (c.b * fall) / 255);
        }
    }

    // slow secondary pulse offset to give a heart-like feel
    if ((phase % 80) == 0) {
        // stronger beat: brighten center
        int center = pos;
        for (int j = -3; j <= 3; j++) {
            int idx = center + j;
            if (idx >= 0 && idx < LEDS_ALL) {
                Color c = palette->getPalColor((phase + j*20) & 0xFF);
                leds[idx].r = min(255, leds[idx].r + c.r);
                leds[idx].g = min(255, leds[idx].g + c.g);
                leds[idx].b = min(255, leds[idx].b + c.b);
            }
        }
    }

    for (int i = 0; i < LEDS_ALL; i++) {
        glowForEachLed(i);
    }
    glowRun();
}