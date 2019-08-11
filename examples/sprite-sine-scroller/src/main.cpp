#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "dest.h"
#include "gba.h"
#include "girl.h"

#define REG_VCOUNT *(volatile u16 *)0x04000006
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#define IWRAM_DATA __attribute__((section(".iwram")))

u16 backbuffer[240 * 160] EWRAM_DATA;

void setMode(u32 mode) { REG_DISPCNT = mode; }

int cosT[512];
int sinT[512];
void createTrigLut(void) {
  int i;
  for (i = 0; i < 512; i++) {
    cosT[i] = cosf(2 * M_PI / 512 * i) * 256;
    sinT[i] = sinf(2 * M_PI / 512 * i) * 256;
  }
}

u32 SIN(u32 i) {
  int floor = (i >> 8);
  u32 val1 = sinT[(floor)&0x1ff];
  u32 val2 = sinT[(floor + 1) & 0x1ff];
  return val1 + ((val2 - val1) * (i - (floor << 8)) >> 8);
}

typedef struct test {
  u16 attr0;
  u16 attr1;
  u16 attr2;
  u16 fill;
} __attribute__((aligned(4))) OBJ_ATTR;

#define DCNT_OBJ_1D 0x0040
#define oam_mem ((OBJ_ATTR *)0x07000000)
#define block5 0x06014000
#define pal 0x05000200

int time = 0;

int IWRAM_CODE main(void) {
  irqInit();
  irqEnable(IRQ_VBLANK);
  REG_IME = 1;

  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;

  memcpy(vid_mem, girlBitmap, girlBitmapLen);
  memcpy((int *)block5, dest2Tiles, dest2TilesLen);
  memcpy((int *)pal, dest2Pal, dest2PalLen);

  createTrigLut();

  while (true) {
    time++;

    char *text =
        "                              "
        "HI BITCHES!! THIS IS TRIGGER TALKING :) ... WANT TO KNOW WHATS NEW? "
        "WELL, ITS RELEASE TIME AND GENESIS WILL RELEASE SOME NICE PC FIRST "
        "SOON.... ";
    int len = strlen(text);

    VBlankIntrWait();
    for (int i = 0; i < 31; i++) {
      const int center = (160 >> 1);
      const int sineWave =
          (SIN((((((i * (8 << 8)) - ((time & 7) << 8)) - (time << 8))))) *
           30) >>
          8;

      const int yPos = sineWave + center;
      const int xPos = i * 8 - (time & 7) & 0x1ff;

      oam_mem[i].attr0 = yPos | 1 << 13;
      oam_mem[i].attr1 = xPos | 0 << 14;
      oam_mem[i].attr2 = 512 + ((text[(i + ((time) / 8)) % len] - ' ') * 2);
    }
  }

  return 0;
}
