#include "gba.hpp"
#include <gba_video.h>

void setDisplayMode(u32 mode);
inline void setPixel(int x, int y, u16 color) {
  VIDEO_MEM[x + y * SCREEN_WIDTH] = color;
}

inline void setPixelFast(int x, int y, u16 color) {
  VIDEO_MEM[x + (y << 7) + (y << 6) + (y << 5) + (y << 4)] = color;
}



