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
 *	tkdev_timer.h (H8/3069)
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
#define	MSTCRL		((_UB *)0x00fee01d)

#define	TSTR		((_UB *)0x00ffff60)
#define	TSNC		((_UB *)0x00ffff61)
#define	TMDR		((_UB *)0x00ffff62)
#define	TISRA		((_UB *)0x00ffff64)
#define	TISRB		((_UB *)0x00ffff65)
#define	TISRC		((_UB *)0x00ffff66)

#define	TCR		((_UB *)0x00ffff68)	// 16bit Timer #0
#define	TIOR		((_UB *)0x00ffff69)
#define	TCNT		((_UH *)0x00ffff6a)
#define	GRA		((_UH *)0x00ffff6c)
#define	GRB		((_UH *)0x00ffff6e)

#define	MSTPL4		(1 << 4)

#define	TSTR_STR	(1 << 0)
#define	TSNC_SYNC	TSTR_STR
#define	TMDR_PWM	TSTR_STR

#define	TISRA_IMFA	TSTR_STR
#define	TISRB_IMFB	TSTR_STR
#define	TISRC_OVF	TSTR_STR

#define	TISRA_IMIEA	(TSTR_STR << 4)
#define	TISRB_IMIEB	TISRA_IMIEA
#define	TISRC_OVIE	TISRA_IMIEA

#define	TCR_CCLR0	(1 << 5)
#define	TCR_TPSC1	(1 << 1)
#define	TCR_TPSC0	(1 << 0)

/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	/* module power on */
	*MSTCRL &= ~MSTPL4;

	/* disable timer interrupt */
	*TISRA &= ~TISRA_IMIEA;
	*TISRB &= ~TISRB_IMIEB;
	*TISRC &= ~TISRC_OVIE;

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
	*TISRA &= ~TISRA_IMFA;
	*TISRB &= ~TISRB_IMFB;
	*TISRC &= ~TISRC_OVF;
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
	*TISRA |= TISRA_IMIEA;
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
	*TISRA &= ~TISRA_IMFA;
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
	*TISRA &= ~TISRA_IMIEA;
	*TISRB &= ~TISRB_IMIEB;
	*TISRC &= ~TISRC_OVIE;

	/* stop timer */
	*TSTR &= ~TSTR_STR;

	/* clear interrupt */
	*TISRA &= ~TISRA_IMFA;
	*TISRB &= ~TISRB_IMFB;
	*TISRC &= ~TISRC_OVF;

	/* module power off */
	*MSTCRL |= MSTPL4;
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
