#include "anim.h"
#include "config.h"

extern uint8_t wallBytes[];
extern TreeRound rounds[];
extern uint8_t rounds_count;
Color colrs[3] = {Color(0x800000), Color(0x000080), Color(0x008000)};

void Anim::animCalibrate_SetUp()
{
}

void Anim::animCalibrate_Run()
{
  memset(leds, 0, sizeof(Color) * LEDS_ALL);
  memset(wallBytes, 0, VIDEO_WIDTH * VIDEO_HEIGHT * sizeof(Color));

  size_t index = 0;
  for (size_t i = 0; i < rounds_count; i++)
  {
    Color c = colrs[i % 3];
    for (size_t j = 0; j < rounds[i].r_width; j++)
    {
      leds[index++] = j == rounds[i].r_azimuth ? Color(0xFFFFFF) : c;
      if (i > LEDS_ALL - 1)
        break;
    }

    int y = rounds[i].r_ypos;
    if (y > -1 && y < VIDEO_HEIGHT)
    {
      for (size_t k = 0; k < VIDEO_WIDTH; k++)
      {
        setMatrix(c, k, y);
      }
    }

    if (i > LEDS_ALL - 1)
      break;
  }

  showWallFrame();
}