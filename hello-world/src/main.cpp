#include "gba.h"

#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

#define RGB15(r, g, b) (r | (g<<5) | (b<<10))

void setDisplayMode(u32 mode) {
  REG_DISPCNT = mode;
}

void setPixel(int x, int y, u16 color) {
  VIDEO_MEM[x + y * SCREEN_HEIGHT] = color;
}

int main() {
  setDisplayMode(DCNT_MODE3 | DCNT_BG2);

  for(int x = 0; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      setPixel(x, y, RGB15(0, 31, 0));
    }
  }

  return 0;
}
