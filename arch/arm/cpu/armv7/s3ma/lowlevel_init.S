/*
 * A lowlevel_init function that sets up the stack to call a C function to
 * perform further init.
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *
 * Author :
 *	Aneesh V	<aneesh@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <linux/linkage.h>

ENTRY(lowlevel_init)
		/*
		 * Only CPU0 can continue execution
		 */
		mrc     p15,0,r11,c0,c0,5
		and     r11, r11, #0x7
		cmp		r11, #0
		BEQ     _continue

		/* Secondary CPUs: configure the GIC CPU Interface:
		 * - enable interrupts at all priorities
		 * - set enable bit in ICCICR */
		ldr	r0, =0x3BE00100
		ldr	r1, =0xFFFF
		str	r1, [r0, #0x04]
		mov	r1, #1
		str	r1, [r0]

		/* Other cpus halt here.  Branch to 0 on wakeup */
		/* IRQs ignored, IRQ/FIQ are still set from earlier */
		wfi
		mov r0, #0
		bx r0

_continue:
	/*
	 * Setup a temporary stack
	 */
	ldr	sp, =CONFIG_SYS_INIT_SP_ADDR
	bic	sp, sp, #7 /* 8-byte alignment for ABI compliance */
#ifdef CONFIG_SPL_BUILD
	ldr	r9, =gdata
#else
	sub	sp, sp, #GD_SIZE
	bic	sp, sp, #7
	mov	r9, sp
#endif
	/*
	 * Save the old lr(passed in ip) and the current lr to stack
	 */
	push	{ip, lr}

	/*
	 * go setup pll, mux, memory
	 */
	bl	s_init
	pop	{ip, pc}
ENDPROC(lowlevel_init)
