#include "anim.h"

void Anim::glowSetUp()
{
    braPhaseSpd = random(3,8);
    braFreq = random(40,120);
}

void Anim::glowForEachLed(int i)
{
    int bra = (sint8_t) (braPhase + i * braFreq);
    bra = BRA_OFFSET + (abs(bra) >> BRA_AMP_SHIFT);
    leds[i] = leds[i].brightness((int)bra);
}

void Anim::glowRun()
{
    braPhase += braPhaseSpd;
}
