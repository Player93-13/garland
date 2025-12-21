#include "anim.h"
#include "color.h"
#include "palette.h"
#include <FastLED.h>

#define AURORA_TAIL 6
#define AURORA_SPEED_MIN 1
#define AURORA_SPEED_MAX 4

void Anim::animAurora_SetUp() {
    period = 30; // base frame period
    phase = random(0, 255);
    pos = random(0, LEDS_ALL);
    inc = random(AURORA_SPEED_MIN, AURORA_SPEED_MAX);
    palCut = random(1, 4);
    glowSetUp();
}

void Anim::animAurora_Run() {
    // base aurora bands
    phase += inc;
    pos += (inc > 1) ? (inc - 1) : 1;
    if (pos >= LEDS_ALL) pos -= LEDS_ALL;

    for (int i = 0; i < LEDS_ALL; i++) {
        // moving sine wave for soft banding
        uint8_t wave = sin8((i * 6) + phase);
        uint8_t x = (i * 256 / LEDS_ALL + phase / 2) & 0xFF;
        Color c = palette->getPalColor(x);
        uint8_t b = 110 + (wave >> 1); // 110..~235
        leds[i] = c.brightness(b);
    }

    // add a moving comet-like brighter tail for motion accent
    for (int t = 0; t < AURORA_TAIL; t++) {
        int idx = pos - t;
        if (idx < 0) idx += LEDS_ALL;
        Color c = palette->getPalColor((phase + t * 32) & 0xFF);
        uint8_t b = 255 - t * (255 / AURORA_TAIL);
        // additive blend to keep colors vivid
        int nr = leds[idx].r + (c.r * b) / 255;
        int ng = leds[idx].g + (c.g * b) / 255;
        int nb = leds[idx].b + (c.b * b) / 255;
        leds[idx].r = (nr > 255) ? 255 : nr;
        leds[idx].g = (ng > 255) ? 255 : ng;
        leds[idx].b = (nb > 255) ? 255 : nb;
    }

    // apply soft glow to each led
    for (int i = 0; i < LEDS_ALL; i++) {
        glowForEachLed(i);
    }
    glowRun();
}