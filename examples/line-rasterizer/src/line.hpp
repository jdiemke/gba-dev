#include "gba.hpp"

struct Point {
  int x;
  int y;
};

void drawLineDDA(Point &start, Point &end, u16 &color);
void drawLineDDA2(Point &start, Point &end, u16 &color);
