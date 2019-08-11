#include "line.hpp"
#include <math.h>
#include "gba.hpp"
#include "mode3.hpp"

inline int sign(int val) { return (val > 0) ? 1 : ((val < 0) ? -1 : 0); }

inline int fixedPointRound(int val) { return (val + 128) >> 8; }

void drawLineDDA(Point &start, Point &end, u16 &color) {
  int xDistance = (end.x - start.x);
  int yDistance = (end.y - start.y);

  int dx, dy, length;

  if (abs(xDistance) > abs(yDistance)) {
    dx = sign(xDistance) << 8;
    dy = yDistance << 8 / abs(xDistance);
    length = abs(xDistance);
  } else {
    dx = xDistance << 8 / abs(yDistance);
    dy = sign(yDistance) << 8;
    length = abs(yDistance);
  }

  int xPosition = start.x * 256;
  int yPosition = start.y * 256;

  for (int i = 0; i <= length; i++) {
    setPixel(fixedPointRound(xPosition), fixedPointRound(yPosition), color);
    xPosition += dx;
    yPosition += dy;
  }
}
