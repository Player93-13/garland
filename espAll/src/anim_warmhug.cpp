#include "anim.h"
#include "color.h"
#include "palette.h"
#include <FastLED.h>

void Anim::animWarmHug_SetUp() {
    period = 40;
    phase = random(0, 255);
    inc = random(1,3);
    palCut = 2;
    glowSetUp();
}

void Anim::animWarmHug_Run() {
    phase += inc;
    // soft flowing warm gradient with gentle pulse
    for (int i = 0; i < LEDS_ALL; i++) {
        uint8_t x = (i * 256 / LEDS_ALL + phase/3) & 0xFF;
        Color c = palette->getPalColor(x);
        // gentle per-led pulsing using sin8
        uint8_t pulse = sin8((phase>>1) + (i * 3));
        uint8_t b = 170 + (pulse >> 1); // 170..~300 but capped in brightness
        leds[i] = c.brightness(b);
    }

    // occasional tiny warm sparkles
    if (random(120) == 0) {
        int p = random(0, LEDS_ALL);
        Color sc = palette->getPalColor(rngb());
        leds[p].r = min(255, leds[p].r + sc.r);
        leds[p].g = min(255, leds[p].g + sc.g);
        leds[p].b = min(255, leds[p].b + sc.b);
    }

    for (int i = 0; i < LEDS_ALL; i++) {
        glowForEachLed(i);
    }
    glowRun();
}