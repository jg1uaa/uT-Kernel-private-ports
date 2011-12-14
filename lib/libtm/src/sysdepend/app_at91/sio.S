/*
 *----------------------------------------------------------------------
 *    micro T-Kernel
 *
 *    Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
 *    micro T-Kernel is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Version:   1.01.01
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2011/12/12.
 *
 *----------------------------------------------------------------------
*/

#include <machine.h>
#include <tk/asm.h>

	.text

	/*
	 * init sio
	 */
	.global sio_init
sio_init:
	/* set to peripheral mode for USART0 transmit/receive */
	ldr     r1, =PIOA_BASE
	mov     r2, #(0x01 << 15 | 0x01 << 16)
	str     r2, [r1, #PIO_PDR]

	/* USART0 clock enable */
	ldr     r1, =APMC_BASE
	mov     r2, #(0x01 << 2)
	str     r2, [r1, #APMC_PCER]

	/* 115200 bps, 8bit, non-parity, 1 stop bit */
	ldr     r0, =USART0_BASE
	mov     r2, #0x08c0
	str     r2, [r0, #US_MR] 
	mov     r2, #0x1b /* CD = MCLK/(baud*16) = 27.30(38400bps) */
	str     r2, [r0, #US_BRGR]

	mov     r2, #0x10c
	str     r2, [r0, #US_CR]

	mov     pc, lr

	/*
	 * send frame
	 * r0: buffer address, r1: count in bytes
	 */
	.global sio_send_frame
sio_send_frame:
	ldr     r2, =USART0_BASE

	str     r0, [r2, #US_TPR]
	str     r1, [r2, #US_TCR]

	mov     r3, #(1 << 6)
	str     r3, [r2, #US_CR]

wait_ENDTX:
	ldr     r3, [r2, #US_CSR]
	tst     r3, #(1 << 4)
	beq     wait_ENDTX   /* if ENDTX == 0 */

	mov     pc, lr

	/*
	 * receive frame
	 * r0: buffer address, r1: count in bytes
	 */
	.global sio_recv_frame
sio_recv_frame:
	ldr     r2, =USART0_BASE

	str     r0, [r2, #US_RPR]
	str     r1, [r2, #US_RCR]

	mov     r3, #(1 << 4)
	str     r3, [r2, #US_CR]

wait_ENDRX:
	ldr     r3, [r2, #US_CSR]
	tst     r3, #(1 << 3)
	beq     wait_ENDRX   /* if ENDRX == 0 */

	mov     pc, lr

