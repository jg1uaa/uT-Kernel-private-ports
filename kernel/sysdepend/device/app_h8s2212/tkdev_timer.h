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
 *	tkdev_timer.h (H8S2212)
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
#define MAX_TIMER_PERIOD	50


/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	/* module power on */
	*MSTP_CRA &= ~(1U << 5);

	*TPU0_TIER &= ~(1U << 0);

	/* Stop timer */
	*TPU_TSTR &= ~(1U << 0);

	*TPU0_TSR &= ~(TMR0_TGFA);

	/* Set timer mode */ 
	*TPU0_TCR = ( TCNT_TGRA | CLK_EDGE_L | TM0_CKS_16 );

	/* Set counter */
	*TPU0_TGRA = CFN_TIMER_PERIOD * (SYSTEM_CLOCK / 1000 / 16) - 1;
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
	/* Set timer */
	knl_init_hw_timer();

	/* Timer interrupt enable */
	*TPU0_TIER |= (1U << 0);

	/* Start timer count */
	*TPU_TSTR |= (1U << 0);
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
	*TPU0_TSR &= ~TMR0_TGFA;
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
	*TPU0_TIER &= ~TMR0_TGIEA;

	*TPU0_TSR  &= ~TMR0_TGFA;

	/* Stop timer */
	*TPU_TSTR &= ~(1U << 0);

	/* module power off */
	*MSTP_CRA |= (1U << 5);
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

	UW	ofs, max;
	UH	unf;

	UINT	imask;

	DI(imask);

	max = *TPU0_TGRA + 1;
	do {
		unf = *TPU0_TSR & TMR0_TGFA;
		ofs = max - *TPU0_TCNT;
	} while ( unf != (*TPU0_TSR & TMR0_TGFA) );
	if ( unf != 0 ) {
		ofs += max;
	}

	EI(imask);

	return ofs * (1000 * 16) / SYSTEM_CLOCK;

}

#endif /* _TKDEV_TIMER_ */
