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
 *	tkdev_timer.h (H8/3052)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/sysdef.h>
#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	20

/*
 * Timer definition
 */
#define	MSTCR		((_UB *)0x00ffff5e)

#define	TSTR		((_UB *)0x00ffff60)
#define	TSNC		((_UB *)0x00ffff61)
#define	TMDR		((_UB *)0x00ffff62)

#define	TCR		((_UB *)0x00ffff64)	// 16bit Timer #0
#define	TIOR		((_UB *)0x00ffff65)
#define	TIER		((_UB *)0x00ffff66)
#define	TSR		((_UB *)0x00ffff67)
#define	TCNT		((_UH *)0x00ffff68)
#define	GRA		((_UH *)0x00ffff6a)
#define	GRB		((_UH *)0x00ffff6c)

#define	MSTOP5		(1 << 5)

#define	TSTR_STR	(1 << 0)
#define	TSNC_SYNC	TSTR_STR
#define	TMDR_PWM	TSTR_STR

#define	TIER_IMIEA	(1 << 0)
#define	TIER_IMIEB	(1 << 1)
#define	TIER_OVIE	(1 << 2)

#define	TSR_IMFA	TIER_IMIEA
#define	TSR_IMFB	TIER_IMIEB
#define	TSR_OVF		TIER_OVIE

#define	TCR_CCLR0	(1 << 5)
#define	TCR_TPSC1	(1 << 1)
#define	TCR_TPSC0	(1 << 0)

/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	/* module power on */
	*MSTCR &= ~MSTOP5;

	/* disable timer interrupt */
	*TIER = 0x00;

	/* initialize timer */
	*TSTR &= ~TSTR_STR;
	*TSNC &= ~TSNC_SYNC;
	*TMDR &= ~TMDR_PWM;

	*TCR = TCR_CCLR0 | TCR_TPSC1 | TCR_TPSC0;
	*TIOR = 0x00;
	*TCNT = 0x0000;
	*GRA = (CPU_CLOCK / (1000 * 8)) * CFN_TIMER_PERIOD - 1;
	*GRB = ~0;

	/* clear interrupt */
	*TSR &= ~(TSR_IMFA | TSR_IMFB | TSR_OVF);
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
	/* Set timer */
	knl_init_hw_timer();

	/* start timer */
	*TSTR |= TSTR_STR;

	/* enable interrupt */
	*TIER |= TIER_IMIEA;
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both according to hardware.
 */
Inline void knl_clear_hw_timer_interrupt( void )
{
	*TSR &= ~TSR_IMFA;
}
Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* Nothing required to do at this point */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	/* disable timer interrupt */
	*TIER = 0x00;

	/* stop timer */
	*TSTR &= ~TSTR_STR;

	/* clear interrupt */
	*TSR &= ~(TSR_IMFA | TSR_IMFB | TSR_OVF);

	/* module power off */
	*MSTCR |= MSTOP5;
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < CFN_TIMER_PERIOD * 2
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	// XXX not supported
	return 0;
}

#endif /* _TKDEV_TIMER_ */
