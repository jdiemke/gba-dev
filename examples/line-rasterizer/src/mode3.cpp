#include "gba.hpp"

void setDisplayMode(u32 mode) { REG_DISPCNT = mode; }
