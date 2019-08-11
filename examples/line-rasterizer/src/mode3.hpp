#include "gba.hpp"

void setDisplayMode(u32 mode);
inline void setPixel(int x, int y, u16 color) {
  VIDEO_MEM[x + y * SCREEN_WIDTH] = color;
}

