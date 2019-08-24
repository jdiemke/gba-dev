/**
 * A Simple Thumb Assembly Language Example:
 *
 * int my_func(int a, int b);
 */

.text               @ Put the content of this file into the code section
.align 2            @ Align this function to 1<<2 bytes
.thumb              @ Use Thumb code
.thumb_func         @ Make this a thumb function
.global my_func     @ Make this function accessible to the outside world

my_func:
    add     r0, r1
    bx      lr
