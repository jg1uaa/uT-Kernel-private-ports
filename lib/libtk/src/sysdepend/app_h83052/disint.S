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

/*
 *	@(#)disint.S (libtk/H83052)
 *
 *	Enable/disable external interrupt
 */

#include <machine.h>
#include <tk/sysdef.h>

	.h8300h

/*
 * Disable external interrupt
 * UINT disint( void )
 */
	.text
	.balign	2
	.globl	Csym(disint)
	.type	Csym(disint), %function
Csym(disint):
	stc	ccr, r0l
	orc	#CCR_I, ccr
	rts


/*
 * Enable external interrupt (restore to original state) 
 * UINT enaint( UINT intsts )
 */
	.text
	.balign	2
	.globl	Csym(enaint)
	.type	Csym(enaint), %function
Csym(enaint):
	mov.b	r0l, r1l
	and.b	#CCR_I, r1l
	stc	ccr, r0l
	mov.b	r0l, r0h
	and.b	#CCR_EI_MASK, r0h
	or.b	r0h, r1l
	ldc	r1l, ccr
	rts
