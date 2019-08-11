#include "mode3.hpp"
#include <math.h>
#include "gba.hpp"
#include "line.hpp"

inline int sign(int val) { return (val > 0) ? 1 : ((val < 0) ? -1 : 0); }

inline int fixedPointRound(int val) {
    return (val + 128) >> 8;
}

void drawLineDDA(Point &start, Point &end, u16 &color) {
  int xDistance = (end.x - start.x);
  int yDistance = (end.y - start.y);

  int dx, dy, length;

  if (abs(xDistance) > abs(yDistance)) {
    dx = sign(xDistance) * 256;
    dy = yDistance * 256 / abs(xDistance);
    length = abs(xDistance);
  } else {
    dx = xDistance * 256 / abs(yDistance);
    dy = sign(yDistance) * 256;
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
