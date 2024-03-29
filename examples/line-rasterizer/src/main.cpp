#include <stdlib.h>
#include "line.hpp"
#include "mode3.hpp"

int main() {
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    Point start;
    Point end;
    u16 color;

    while (true) {
      start.x = rand() % SCREEN_WIDTH;
      start.y = rand() % SCREEN_HEIGHT;

      end.x = rand() % SCREEN_WIDTH;
      end.y = rand() % SCREEN_HEIGHT;

      color = rand() & 0xffff;

      drawLineDDA(start, end, color);
    }

    return 0;
}
