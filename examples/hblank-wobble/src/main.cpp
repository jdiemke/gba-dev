#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>  // for memcpy
#include "gba.h"

#include "background.h"
#include "backgroundfont.h"

#define REG_VCOUNT *(volatile u16 *)0x04000006
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#define IWRAM_DATA __attribute__((section(".iwram")))
u16 backbuffer[240 * 160] EWRAM_DATA;

volatile u32 time = 0;

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

#define DCNT_OBJ_1D 0x0040
#define oam_mem ((OBJ_ATTR *)0x07000000)
#define oam_mem_aff ((OBJ_AFFINE *)0x07000000)
#define block5 0x06014000
#define pal 0x05000200

typedef u16 SCR_ENTRY;
typedef u16 CHR_ENTRY;

typedef SCR_ENTRY SCREENBLOCK[1024];
typedef CHR_ENTRY CHARBLOCK[8192];
#define se_mem ((SCREENBLOCK *)MEM_VRAM)
#define ch_mem ((CHARBLOCK *)MEM_VRAM)
#define MEM_PAL_BG (void *)0x05000000
#define REG_BG0CNT *(volatile u16 *)0x04000008
#define REG_BG1CNT *(volatile u16 *)0x0400000A
#define REG_BG_OFX *(volatile s16 *)0x04000010
#define REG_BG_OFY *(volatile s16 *)(0x04000010 + 2)

#define SCROLL_LEN 10
char scroller[10][31] = {
    "               1              ", "      TRSI STRIKES BACK       ",
    "      -----------------       ", "               1              ",
    "  CODE:.............TRIGGER   ", "  GFX:................KENET   ",
    "  MUSIC:............HOFFMAN   ", "               4              ",
    "               4              ", "               4              "};

#define REG_VCOUNT *(volatile u16 *)0x04000006
#define BG2X_L *(volatile u16 *)0x4000028

void hBlankInterrupt() {
  u32 scale = REG_VCOUNT >> 1;
  u32 disp = ((sinT[((scale) + time) & 0x1ff]) * 50);
 
  REG_BG_OFX = (disp >> 8); // split rows: * ((REG_VCOUNT % 2) ? 1 : -1);
}

int IWRAM_CODE main(void) {
  irqInit();
  irqEnable(IRQ_HBLANK | IRQ_VBLANK);
  irqSet(IRQ_HBLANK, hBlankInterrupt);
  REG_IME = 1;

  REG_BG1CNT = 1 << 7 | 0 << 2 | 16 << 8 | 0 << 14;
  REG_BG0CNT = 1 << 7 | 3 << 2 | 30 << 8 | 0 << 14;

  setMode(DCNT_MODE0 | DCNT_BG0 | DCNT_BG1);

  memcpy(MEM_PAL_BG, backgroundPal, backgroundPalLen);
  memcpy(ch_mem[3], backgroundfontTiles, backgroundfontTilesLen);
  memcpy(ch_mem[0], backgroundTiles, backgroundTilesLen);

  for (int y = 0; y < 160 / 8; y++) {
    for (int x = 0; x < 240 / 8; x++) {
      se_mem[16][x + y * 32] = backgroundMap[x + y * (240 / 8)];
    }
  }

  createTrigLut();

  while (1) {
    int x;
    int y = (20 + (time >> 2) / 8) % 32;
    VBlankIntrWait();
    for (x = 0; x < 30; x++) {
      se_mem[30][(x + y * 32)] =
          scroller[((time >> 2) / 8) % SCROLL_LEN][x] - ' ';
    }

    REG_BG_OFY = (time >> 2) % 256;
    time += 3;
  }

  return 0;
}
