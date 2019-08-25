/*
 * see http://nocash.emubase.de/gbatek.htm
 * see http://www.pouet.scene.org/topic.php?which=5346&page=1 for modplayer
 * http://static.patater.com/gbaguy/gba/ch2.htm
 * http://home.no/neogeo/HOVEDSIDE_INDEX/GBA_HOVEDSIDE_INDEX_ENGELSK/index.html
 * http://dev-scene.com/NDS/Tutorials
 * ftp://ftp.sys.net.pl/pub/Tech/08.Gameboy/GBA/The%20PERN%20Project%20-%20Day%203.htm
 * http://www.gbajunkie.co.uk/
 * http://www.flipcode.com/archives/The_Art_of_Demomaking-Issue_01_Prologue.shtml
 * http://zine.bitfellas.org/article.php?zine=14&id=24
 */

#include <string.h> // for memcpy
#include <stdlib.h>
#include <math.h>
#include "gba.h"
#include "tex1_128.h"
#include "logo2.h"
#include "logo.h"

#define REG_VCOUNT *(volatile u16*)0x04000006
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

u32 mLUT[240*160] EWRAM_DATA;

static inline void drawDeformation(u32 time) IWRAM_CODE;

void setMode(u32 mode) {
	REG_DISPCNT = mode;
}

void VSync(void) {
    while(REG_VCOUNT >= 160);   // wait till VDraw
    while(REG_VCOUNT < 160);    // wait till VBlank
}

void createLUT(void) {
	//mLUT = (u16*) malloc(2*240*160);
	//mLUTu = (u16*) malloc(2*240*160);
	

	u32 i,j;
	u32 k=16*240;
	u32 loaderstep = ((((u32)(118) << 8)<<8)/ ((u32)98 << 8)) ; // underflow error
	u32 pos =0;

		for(j=16; j<114;j++) {
	for(i=0; i<240; i++){

		float x = (-1.0f + 2.0f*(float)i/(float)(240))-0.4f;
		float y = (-1.0f + 2.0f*(float)j/(float)(160))+0.2f;
		float d = sqrtf(x*x+y*y);
		float a = atan2f(y,x);

			//float u = cosf(a)/d;//+4*3.14;
		//float v = sinf(a)/d;//+4*3.14;
//float u = x/fabs(y)+128;
//float v = 1/fabs(y) +128;
 	//float u = 0.02*y+0.03*cosf(a*3)/d;
//float v = 0.02*x+0.03*sinf(a*3)/d 	;

//float u = 0.5*a/3.14159265359f;
//float v = sinf(7*d); 
//float u = d*cosf(a+d);
//float v = d*sinf(a+d) ;
float u = 0.2/d;
float v = a/3.14159265359f;
	 	//float u = 1/(d+0.5+0.5*sin(5*a));
		//float v = a*3/3.14159265359;
		//float u = x*cosf(2*d) - y*sinf(2*d);
		//float v = y*cosf(2*d) + x*sinf(2*d) ;	

		u=round(fmodf(u,1.0f)*64);
		v=round(fmodf(v,1.0f)*64+0.5);

        // make this table fixed point??
		mLUT[k++] =((signed int)(u)&0x3f) | (((signed int)(v)&0x3f)<<8);
	
		

	}

		drawLoadingBar(pos>>8);
		pos += loaderstep;
}
}

void drawLoadingBar(u32 c) {
u32 height;

for(height=0; height < 6; height++) {
   mode3Plot(102+(c),height+122,RGB15(31,31,31));
}
}

static inline void drawDeformation(u32 time) {
	u32 y;
	u32 uv;
	u32 u;
	u32 v;
    u32* pLut = mLUT + (16*240);
    u16* pVid = vid_mem + (16*240);
	for(y=(16*240)/2; y<(114*240)/2; y++){
		uv = *pLut;
		u = (uv&0xff)+(time<<1);
		v = (uv>>8)+time;
		*pVid = tex1_128Bitmap[(u&0x3f)+((v&0x3f)<<6)];
        pLut++;
        pVid++;

        // Loop unrolling :-D
        uv = *pLut;
		u = (uv&0xff)+(time<<1);
		v = (uv>>8)+time;
		*pVid = tex1_128Bitmap[(u&0x3f)+((v&0x3f)<<6)];
        pLut++;
        pVid++;
	}
} 
int main(void) IWRAM_CODE;
int main(void) {

	setMode(DCNT_MODE3 | DCNT_BG2);

	memcpy(vid_mem, logo2Bitmap, logo2BitmapLen);
	
	createLUT();




u32 time=0;
	//memset(vid_mem, 0, logo2BitmapLen);
	memcpy(vid_mem, logoBitmap, logoBitmapLen);
	while(1) {

	//VSync();
	drawDeformation(time);
	time = (time+1)&0x3f; // (time+4)/255 to slow because % uses division
	}

	return 0;
}

