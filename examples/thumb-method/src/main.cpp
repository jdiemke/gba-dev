
#include "gba.hpp"
#include "mode3.hpp"

extern "C" int my_func(int a, int b);

int main() {
  setDisplayMode(DCNT_MODE3 | DCNT_BG2);

  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      setPixel(x, y, RGB15(0, 31, 0));
    }
  }
  setPixel(0, 1, RGB15(31, 0, 31));
  setPixel(5, 1, RGB15(31, 0, 31));
  setPixel(my_func(15, 5), 1, RGB15(31, 0, 31));

  while (true) {
      // Do nothing!
  }
}
