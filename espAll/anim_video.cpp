#include "anim.h"
//количество колец гирлянды на елке
#define GARL_N 11

//количество диодов на верхнем кольце (>= 0)
#define GARL_KN 1

//часть гирлянды которая транспонируется на елку (1 / K)
#define GARL_WALL_K 4

#define PI 3.14159265

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

//координаты строк в соотнесении кольцам гирлянды
static uint16_t GarlandRounds[GARL_N] = { 22, 20, 18, 17, 15, 13, 10, 8, 6, 5, 4 };
static Point Garland[GARL];
static Point Star[STAR] =
{
  Point(2, 2),
  Point(1, 3),
  Point(1, 2),
  Point(1, 1),
  Point(2, 1),
  Point(2, 3),
  Point(0, 4),
  Point(0, 1),
  Point(1, 0),
  Point(3, 1),
  Point(3, 4)
};

// расчет транспонирования края занавеса на елку
void TreeCalculate()
{
  //координаты начал колец на елке
  uint16_t roundStarts[GARL_N];

  ////координаты диодов на кольцах ближайших к занавесу
  //int roundWalls[GARL_N];

  //расcчитываем размеры колец на елке
  float Xa = (GARL_N - 1.0) / 2;
  float Xb = GARL_N - 1;
  float Ya = (float)GARL / GARL_N;
  float Yb = GARL_KN;
  float A = (Yb - Ya) / (Xb - Xa); // коэффициенты уравнения размера колец (y = Ax + B)
  float B = Ya - Xa * A;           // коэффициенты уравнения размера колец (y = Ax + B)

  roundStarts[0] = 0;
  uint16_t sum = 0;
  for (uint16_t i = GARL_N - 1; i > 0; i--)
  {
    sum += round(i * A + B);
    roundStarts[i] = GARL - sum;
  }

  ////этот код надо будет заменить на ручную калибровку
  //for (uint16_t i = 0; i < GARL_N; i++)
  //{
  //    roundWalls[i] = (roundStarts[i] + (i == GARL_N - 1 ? GARL - 1 : roundStarts[i + 1])) / 2;
  //}

  //коэффициент для косинуса распределения
  float z = ((float)WALL_WIDTH / GARL_WALL_K - 1) / 2;
  for (uint16_t i = 0; i < GARL_N; i++)
  {
    uint16_t roundNextStart = i == GARL_N - 1 ? GARL : roundStarts[i + 1];
    uint16_t roundwidth = roundNextStart - roundStarts[i];

    for (uint16_t j = 0; j < roundwidth; j++)
    {
      uint16_t x = round(z - z * cos(j * 2.0 * PI / roundwidth));
      Garland[roundStarts[i] + j] = Point(x, GarlandRounds[i]);
    }
  }
}

void Anim::animVideo_SetUp() {
  //затемняем
  for (uint16_t i = 0; i < LEDS; i++) {
    leds[i] = Color();
  }

  TreeCalculate();
}

void Anim::animVideo_Run() {
  //занавес
  for (uint16_t i = 0; i < WALL_WIDTH; i++)
  {
    bool reverse = i % 2 == 1;
    uint16_t k = reverse ? WALL_HEIGHT - 1 : 0; // для расчета навески змейкой

    for (uint16_t j = 0; j < WALL_HEIGHT; j++)
    {
      leds[WALL_OFFSET + i * WALL_HEIGHT + k] = getMatrix(i, j);
      if (reverse)
        k--;
      else
        k++;
    }
  }

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
