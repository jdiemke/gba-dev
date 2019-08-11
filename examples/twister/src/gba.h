#ifndef GBA_H
#define GBA_H

typedef unsigned char	u8;
typedef unsigned short	u16;


typedef u16 COLOR;

#define MEM_IO		0x04000000
#define MEM_VRAM	0x06000000

#define REG_DISPCNT	*((volatile u32*)(MEM_IO+0x000))

#define DCNT_MODE0	0x0000
#define DCNT_MODE1	0x0001
#define DCNT_MODE2	0x0002
#define DCNT_MODE3	0x0003
#define DCNT_MODE4	0x0004
#define DCNT_MODE5	0x0005

#define DCNT_BG0	0x0100
#define DCNT_BG1	0x0200
#define DCNT_BG2	0x0400
#define DCNT_BG3	0x0500
#define DCNT_OBJ	0x1000

#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	160

#define vid_mem		((u16*)MEM_VRAM)

static inline void mode3Plot(int x, int y, COLOR clr) {
	vid_mem[y*SCREEN_WIDTH+x] = clr;
}

static inline COLOR RGB15(u32 red, u32 green, u32 blue) {
	return red | (green << 5) | (blue << 10);
}

#endif
