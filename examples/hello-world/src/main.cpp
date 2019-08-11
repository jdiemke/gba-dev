#include "gba.hpp"
#include "mode3.hpp"

int main() {
  setDisplayMode(DCNT_MODE3 | DCNT_BG2);

  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      setPixel(x, y, RGB15(0, 31, 0));
    }
  }

  while (true) { 
      // Do nothing
  }

  return 0;
}
