#include <math.h>
#include <string.h>
#include "gba.h"

#define REG_VCOUNT *(volatile u16 *)0x04000006
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#define IWRAM_DATA __attribute__((section(".iwram")))

u16 backbuffer[240 * 160] EWRAM_DATA;

void createTrigLut(void);
void twister(u32 time);

int IWRAM_CODE main(void) {
  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

  u32 time = 0;
  createTrigLut();

  while (true) {
    memset(backbuffer, 0, 76800);
    twister(time);
    memcpy(vid_mem, backbuffer, 76800);

    time = (time + 1) & 0x1ff;
  }

  return 0;
}


static inline void drawSpan(int x1, int x2, int y, COLOR clr) {
  u16 *fb = backbuffer + 240 * y + x1;

  while ((x1++) < x2) {
    *(fb++) = clr;
  }
}

int cosT[512];
int sinT[512];

void createTrigLut(void) {
  for (int i = 0; i < 512; i++) {
    cosT[i] = cosf(2 * M_PI / 512 * i) * 256;
    sinT[i] = sinf(2 * M_PI / 512 * i) * 256;
  }
}

void twister(u32 time) {
  int posx = 120;

  u32 step = 512 / 4;
  u32 y = 0;
  for (y = 0; y < 160; y++) {
    int sub = (time << 2) +
              (int)((200 * cosT[+(int)((time << 4) +
                                       (512 * (cosT[(y >> 1) & 0x1ff]) >> 8)) &
                                0x1ff]) >>
                    8);
    int t1 = (step + sub) & 0x1ff;
    int t2 = (step * 2 + sub) & 0x1ff;
    int t3 = (step * 3 + sub) & 0x1ff;
    int t4 = (step * 4 + sub) & 0x1ff;
    int x1 = (25 * cosT[t1]) >> 8;
    int x2 = (25 * cosT[t2]) >> 8;
    int x3 = (25 * cosT[t3]) >> 8;
    int x4 = (25 * cosT[t4]) >> 8;
    posx = 125 + ((30 * cosT[(y + (time << 3)) & 0x1ff]) >> 8);

    if (x1 < x2) {
      drawSpan(posx + x1, posx + x2, y,
              RGB15(15 + ((15 * sinT[(t1 + 256) & 0x1ff]) >> 8), 0, 0));
    } else {
      drawSpan(posx + x3, posx + x4, y,
              RGB15(0, 0, 15 + ((15 * sinT[(t3 + 256) & 0x1ff]) >> 8)));
    }

    if (x2 < x3) {
      drawSpan(posx + x2, posx + x3, y,
              RGB15(0, 15 + ((15 * sinT[(t2 + 256) & 0x1ff]) >> 8), 0));
    } else {
      drawSpan(posx + x4, posx + x1, y,
              RGB15(15 + ((15 * sinT[(t4 + 256) & 0x1ff]) >> 8),
                    15 + ((15 * sinT[(t4 + 256) & 0x1ff]) >> 8), 0));
    }
  }
}
