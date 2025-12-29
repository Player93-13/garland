#include "anim.h"
#include "config.h"
// количество колец гирлянды на елке
#define GARL_N 11

// количество диодов на верхнем кольце (>= 0)
#define GARL_KN 1

// часть гирлянды которая транспонируется на елку (1 / K)
#define GARL_WALL_K 4

// #define PI 3.14159265

struct Point
{
  Point(uint16_t x, uint16_t y)
  {
    X = x;
    Y = y;
  }

  Point()
  {
    X = 0;
    Y = 0;
  }

  uint16_t X;
  uint16_t Y;
};

// координаты строк в соотнесении кольцам гирлянды
static uint16_t GarlandRounds[GARL_N] = {22, 20, 18, 17, 15, 13, 10, 8, 6, 5, 4};
static Point Garland[GARL];
static Point Star[STAR] =
    {
        Point(1, 2),
        Point(2, 3),
        Point(2, 2),
        Point(2, 1),
        Point(1, 1),
        Point(1, 3),
        Point(3, 4),
        Point(3, 1),
        Point(2, 0),
        Point(0, 1),
        Point(0, 4)};

extern TreeRound rounds[];
extern uint8_t rounds_count;

void TreeCalibrate()
{
  int offset = 0;
  uint16_t k = (VIDEO_WIDTH / GARL_WALL_K) - 1;
  for (uint8_t i = 0; i < rounds_count; i++)
  {
    uint16_t width = rounds[i].r_width;
    uint16_t azimuth = rounds[i].r_azimuth;
    uint16_t ypos = rounds[i].r_ypos;
    for (uint16_t j = 0; j < width; j++)
    {
      if(offset + j >= GARL) break;

      float angle = (azimuth - j) * 2.0 * PI / width + PI;
      uint16_t x = round(((1.0 - cos(angle)) / 2.0) * k);
      Garland[offset + j] = Point(x, ypos);
    }

    offset += width;
  }
}

void Anim::animVideo_SetUp()
{
  // затемняем
  memset(leds, 0, sizeof(Color) * LEDS_ALL);
  memset(Garland, 0, sizeof(Point) * GARL);
  LoadCalibration();
  TreeCalibrate();
}

void Anim::animVideo_Run()
{
  // занавес
  showWallFrame();

  //елка
  for (uint16_t i = 0; i < GARL; i++)
  {
    Point p = Garland[i];
    leds[i] = getMatrix(p.X, p.Y);
  }

  //звезда
  for (uint16_t i = 0; i < STAR; i++)
  {
    Point p = Star[i];
    leds[i + GARL] = getMatrix(p.X, p.Y);
  }
}
