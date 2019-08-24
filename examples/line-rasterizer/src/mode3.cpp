#include "gba.hpp"
#include <gba_console.h>

void setDisplayMode(u32 mode) {
  REG_DISPCNT = mode;
}
