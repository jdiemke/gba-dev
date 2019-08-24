#include "line.hpp"
#include <math.h>
#include "gba.hpp"
#include "mode3.hpp"

static inline int sign(int val) { return (val > 0) ? 1 : ((val < 0) ? -1 : 0); }

extern "C" {
s32 biosDivisionAsm(s32 numerator, s32 denominator, s32 *result);
}

// todo: optimize with assembly language
void drawLineDDA(Point &start, Point &end, u16 &color) {
  s32 xDistance = (end.x - start.x);
  s32 yDistance = (end.y - start.y);
  s32 dx, dy, length;

  if (abs(xDistance) > abs(yDistance)) {
    dx = sign(xDistance) << 8;
    biosDivisionAsm(yDistance << 8, abs(xDistance), &dy);
    length = abs(xDistance);
  } else {
    biosDivisionAsm(xDistance << 8, abs(yDistance), &dx);
    dy = sign(yDistance) << 8;
    length = abs(yDistance);
  }

  int xPosition = (start.x << 8) + 128;
  int yPosition = (start.y << 8) + 128;

  for (int i = 0; i <= length; i++) {
    setPixel(xPosition >> 8, yPosition >> 8, color);
    xPosition += dx;
    yPosition += dy;
  }
}
