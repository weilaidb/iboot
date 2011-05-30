/*
 * main.c
 *
 *  Created on: 2011-5-11
 *      Author: sangwei
 */
#include "s3c6410.h"
#include "tiny6410.h"

    .section .init, "ax"
    .code 32

    .globl _start
_start:

    // Enable Instruction Cache
    mov     r0, #0
    mcr     p15, 0, r0, c7, c7, 0   // Invalidate Entire I&D Cache

    mrc     p15, 0, r0, c1, c0, 0   // Enable I Cache
    orr     r0, r0, #R1_I
    mcr     p15, 0, r0, c1, c0, 0

    // Peripheral Port Setup
    ldr     r0, =0x70000013         // Base Addres : 0x70000000, Size : 256 MB (0x13)
    mcr     p15, 0, r0, c15, c2, 4

    // setup stack
    ldr     sp, =0x0c005ff8
    bl      lcd_bl_close

    // disable vector interrupt
    mrc    p15, 0, r0, c1, c0, 0
    bic    r0, r0, #(1<<24)
    mcr    p15, 0, r0, c1, c0, 0

    // disable watch-dog
    ldr     r4, =WTCON
    ldr     r5, =0x0
    str     r5, [r4]

    // disable all interrupt
    ldr     r4, =VIC0INTENCLEAR
    ldr     r5, =0xFFFFFFFF;
    str     r5, [r4]
    ldr     r4, =VIC1INTENCLEAR
    str     r5, [r4]

    // Set the mem1drvcon to raise drive strength  for steploader ecc error
    ldr     r4, =MEM1DRVCON
    ldr     r5, =0x55555555
    str     r5, [r4]

    // setup clock
#ifdef CONFIG_SYNC_MODE
    bl      set_sync_mode
#else /* CONFIG_SYNC_MODE */
    bl      set_async_mode
#endif /* end of CONFIG_SYNC_MODE */
    bl      clock_setup

    // initialize DDR
    bl      mem_ctrl_asm_init

	// clear .bss
	mov    r0, #0                   // get a zero
	ldr    r1, =__bss_start         // bss start
	ldr    r2, =__bss_end           // bss end
_bss_loop:
	cmp    r1, r2                   // check if data to clear
	strlo  r0, [r1], #4             // clear 4 bytes
	blo    _bss_loop                // loop until done

	// start main
	bl		main

    // jump to BL2
    mov     r0, #0
    mcr     p15, 0, r0, c7, c7, 0   // Invalidate Entire I&D Cache
    ldr     r0, =BL2_ADDR
    blx     r0

/*/*}*/

