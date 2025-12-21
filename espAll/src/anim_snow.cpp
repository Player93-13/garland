#include "anim.h"
#include "color.h"
#include "palette.h"

#define SNOW_LIFE 24

void Anim::animFallingSnow_SetUp() {
    period = 30;
    memset(seq, 0, LEDS_ALL);
    memset(ledstmp, 0, sizeof(Color) * LEDS_ALL);
}

void Anim::animFallingSnow_Run() {
    // fade base slightly
    for (int i = 0; i < LEDS_ALL; i++) {
        leds[i].fade(245);
    }

    // move flakes down the strip (higher index -> moves forward)
    for (int i = LEDS_ALL-1; i >= 0; i--) {
        if (seq[i] > 0) {
            int life = seq[i];
            // draw bright white-ish flake
            Color c = Color(255,255,255);
            // fade by life
            int br = (life * 255) / SNOW_LIFE;
            leds[i].r = min(255, leds[i].r + (c.r * br) / 255);
            leds[i].g = min(255, leds[i].g + (c.g * br) / 255);
            leds[i].b = min(255, leds[i].b + (c.b * br) / 255);
            seq[i]--;
            if (i + 1 < LEDS_ALL && seq[i] == 0) {
                // fall to next position
                seq[i+1] = SNOW_LIFE;
            }
        }
    }

    // spawn new flakes occasionally at the start
    if (random(6) == 0) {
        int p = random(0, LEDS_ALL/6); // spawn near start making 'top'
        seq[p] = SNOW_LIFE;
    }

    // soft glow for atmosphere
    for (int i = 0; i < LEDS_ALL; i++) {
        glowForEachLed(i);
    }
    glowRun();
}