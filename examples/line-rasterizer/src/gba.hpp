#ifndef GBA_HEADER
#define GBA_HEADER

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/**
 * IO Registers
 */
#define REG_DISPCNT   *(u32*)0x04000000
#define REG_VCOUNT    *(u16*)0x04000006

#define DCNT_MODE3    0x0003
#define DCNT_BG2      0x0400

#define MEM_VRAM      ((u32*)0x06000000)
#define VIDEO_MEM     ((u16*)MEM_VRAM)

#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

#define RGB15(r, g, b) ((r) | ((g) << 5) | ((b) << 10))

#endif
