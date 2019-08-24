@ DECL: s32 biosDivisionAsm(s32 numerator, s32 denominator);
@ DESC:
	.align
	.thumb_func
	.global biosDivisionAsm
biosDivisionAsm:
	swi		0x06
	bx		lr
