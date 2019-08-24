#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "gba.h"
#include "girl.h"

#define REG_VCOUNT *(volatile u16 *)0x04000006
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#define IWRAM_DATA __attribute__((section(".iwram")))

u16 backbuffer[240 * 160] EWRAM_DATA;

u32 time = 0;
 static inline void biosDivision(s32 numerator, s32 denominator, s32* result, s32* remainder) {
//---------------------------------------------------------------------------------
	s32 __result, __remainder;

    asm volatile (
        "mov   r0, %[numerator]   \n\t"
        "mov   r1, %[denominator]   \n\t"
        "swi   6        \n\t" // Put 6 here for Thumb C Compiler mode.
        "mov   %[result], r0\n\t"
        "mov   %[remainder], r1\n\t"

        : [result] "=l" (__result), [remainder] "=l" (__remainder)

        : [numerator] "l" (numerator), [denominator] "l" (denominator)

		: "r0","r1","r2","r3"
    );
	
	*result = __result;
	*remainder = __remainder;
}

static inline void drawSpan(int x1, int x2, int y) {
  int xpos = 0;
  s32 forwardDiff, dummy;
  

  biosDivision(((8 << 8) << 8) , ((x2 - x1) << 8), &forwardDiff, &dummy);

  u16 *fb = backbuffer + 240 * y + x1;

  char const *text = "GENESIS - HOODLUM - RAZOR1911 - RELOADED - ";

  const int time2 = time * 3;
  const int textPos = (((y + (((time2)))) >> 2) >> 3) & 0xFF;
  int character = text[textPos % strlen(text)];
  u32 start = character - ' ';

  int color;
  while ((x1++) < x2) {
    color = fontBitmap[(start << 3) + (((u32)xpos) >> 8) +
                       ((((y + ((time2))) >> 2)) & 7) * 512];

    if ((color & 0x1F) != 14) {
      *(fb) = color;
    }
    fb++;
    xpos += forwardDiff;
  }
}

int cosT[512];
int sinT[512];

void createTrigLut(void) {
  int i;
  for (i = 0; i < 512; i++) {
    cosT[i] = cosf(2 * M_PI / 512 * i) * 256;
    sinT[i] = sinf(2 * M_PI / 512 * i) * 256;
  }
}

static inline void twistscroller(u32 time) {
  for (u32 y = 20; y < 140; y++) {
    int sine = cosT[((time * 21) + (y << 2)) & 0x1ff];
    int x1 = ((20 << 8) + 10 * sine) >> 8;
    int posx = 40 + ((20 * sine) >> 8);

    drawSpan(posx - x1, posx + x1, y);
  }
}

#define REG_DMA3SAD *(volatile u32 *)0x40000D4
#define REG_DMA3DAD *(volatile u32 *)0x40000D8
#define REG_DMA3CNT *(volatile u32 *)0x40000DC
#define DMA_ON 1 << 31
#define DMA_32 1 << 26

void IWRAM_CODE DMAcopy(u32 dest, u32 src, u32 count) {
  REG_DMA3CNT = 0;
  REG_DMA3SAD = src;
  REG_DMA3DAD = dest;
  REG_DMA3CNT = count | DMA_ON | DMA_32;
}

#define BLDY *(volatile u16 *)0x4000054
#define BLDCNT *(volatile u16 *)0x4000050
#define BG2CNT *(volatile u32 *)0x400000c
#define MOSAIC *(volatile u32 *)0x400004c

#define WIN0H *(volatile u32 *)0x4000040
#define WIN0V *(volatile u32 *)0x4000044
#define WININ *(volatile u32 *)0x4000048
#define WINOUT *(volatile u32 *)0x400004A

#define BG2X_L *(volatile u16 *)0x4000028
#define BG2Y_L *(volatile u16 *)0x400002c

u32 smoothstep(int start, int end, int pos) {
  if (pos < start) return 0;
  if (pos > end) return 1 << 8;
  return ((pos - start) << 8) / (end - start);
}

int IWRAM_CODE main(void) {
  irqInit();
  irqEnable(IRQ_VBLANK);
  REG_IME = 1;

  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2 | 1 << 13;
  WIN0H = 240 | 0 << 8;
  WIN0V = 140 | 20 << 8;
  WININ = 1 << 2 | 1 << 5;
  WINOUT = WININ;
  // memcpy(vid_mem, logoBitmap, logoBitmapLen);

  BLDCNT = 1 << 2 | 3 << 6;
  BLDY = 16;
  BG2CNT = 1 << 6;
  createTrigLut();

  while (true) {
    DMAcopy((u32)(backbuffer + 240 * 20), (u32)(girlBitmap + 240 * 20),
            (240 * 120) / 2);
    twistscroller(time);

    VBlankIntrWait();

    // int scale = (1 - smoothstep(16, 96, time)) * 256;

    // BG2X_L = ((sinT[(time<<1)&0x1ff])*100)+120*256;
    // BG2Y_L =
    //   ((abs(cosT[(((time * time) >> 3) + 256) & 0x1ff])) * 160 * scale) >> 8;
    //  int size = ((sinT[(time << 0) & 0x1ff] + 256) * 15) >> 9;
    // MOSAIC = size | size<<4;
    BLDY = (((1<<8) - smoothstep(20, 80, time)) * 16)>>8;
    DMAcopy((u32)(vid_mem + 240 * 20), (u32)(backbuffer + 240 * 20),
            (240 * 120) / 2);

    time++;
  }

  return 0;
}
