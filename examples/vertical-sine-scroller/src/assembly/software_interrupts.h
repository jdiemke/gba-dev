/* Software Interrupt Definitions */

#define	SWI_SOFT_RESET          0x00
#define	SWI_REGISTER_RAM_RESET  0x01
#define SWI_HALT                0x02
#define SWI_STOP                0x03
#define SWI_INTR_WAIT           0x04
#define SWI_V_BLANK_INTR_WAIT   0x05
#define	SWI_DIV                 0x06
#define SWI_DIV_ARM             0x07
#define	SWI_SQRT                0x08
#define SWI_ARC_TAN             0x09
#define SWI_ARC_TAN_2           0x0A
#define SWI_CPU_SET             0x0B
#define SWI_CPU_FAST_SET        0x0C
#define SWI_BIOS_CHECKSUM       0x0D
#define SWI_BG_AFFINE_SET       0x0E
#define SWI_OBJ_AFFINE_SET      0x0F
