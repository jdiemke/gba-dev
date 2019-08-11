#include <math.h>
#include "gba.hpp"
#include "line.hpp"
#include "mode3.hpp"

int main() {
  setDisplayMode(DCNT_MODE3 | DCNT_BG2);

  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      setPixel(x, y, RGB15(0, 31, 0));
    }
  }

  while (true) {
    Point start = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
    Point end = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
    u16 color = rand() & 0xffff;

    drawLineDDA(start, end, color);
  }

  return 0;
}
