#include "anim.h"

void Anim::animVideo_SetUp() {
  for (int i = 0; i < LEDS; i++) {
    leds[i] = Color();
  }
}

void Anim::animVideo_Run() {
  //занавес
  for (int i = 0; i < WALL_WIDTH; i++)
  {
    bool reverse = i % 2 == 1;
    int k = reverse ? WALL_HEIGHT - 1 : 0; // для расчета навески змейкой
    
    for (int j = 0; j < WALL_HEIGHT; j++)
    {
      leds[WALL_OFFSET + i * WALL_HEIGHT + k] = getMatrix(i, j);
      if(reverse)
        k--;
      else
        k++;
    }
  }

  //елка
  for (int i = 0; i < GARL; i++)
  {
    
  }
}
