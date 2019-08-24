@ This is a fast integer divison using the GBA Bios implementation
@ Usage: void biosDivisionAsm(s32 dividend, s32 divisor, s32* quotient);

#include "software_interrupts.h"

.align
.thumb_func
.global biosDivisionAsm

biosDivisionAsm:
	swi		SWI_DIV
    str     r0, [r2]
	bx		lr
