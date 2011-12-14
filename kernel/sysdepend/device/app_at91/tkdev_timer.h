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
 *	tkdev_timer.h (AT91)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

#if USE_TRAP
#define ENAINT	Asm("msr cpsr_c, %0":: "i"(PSR_SVC))
#define DISINT	Asm("msr cpsr_c, %0":: "i"(PSR_SVC|PSR_DI))
#else
#define ENAINT	Asm("msr cpsr_c, %0":: "i"(PSR_IRQ))
#define DISINT	Asm("msr cpsr_c, %0":: "i"(PSR_IRQ|PSR_DI))
#endif

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

IMPORT UW	knl_TimerClkDiv;	/* Dividing rate of timer clock */

/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	UW	n, mode;
	UINT	imask;
	UW	channel = TCB0_BASE | TC_C0;

	/* Determine dividing rate */
	n = (UW)CFN_TIMER_PERIOD * (TMCLK * 1000) / 65536;
	mode = ( n <  2 )? (knl_TimerClkDiv =  2, TC_CLKS_MCK2 ):  /*  2 dividing */
	      ( n <  8 )? (knl_TimerClkDiv =  8, TC_CLKS_MCK8 ):  /*  8 dividing */
			  (knl_TimerClkDiv = 32, TC_CLKS_MCK32);  /* 32 dividing */
	mode |= TC_CPCTRG;

	DI(imask);

	/* Set timer mode/Stop timer */
	out_w(channel | TC_CCR, TC_CLKDIS);
	out_w(channel | TC_CMR, mode);

	/* Set counter */
	n = (UW)(CFN_TIMER_PERIOD * (TMCLK * 1000) / knl_TimerClkDiv - 1);
	out_w(channel | TC_RC, n);

	out_w(channel | TC_IER, 1 << 4);

	/* Start timer count */
	out_w(channel | TC_CCR, TC_SWTRG | TC_CLKEN);

	EI(imask);
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
IMPORT	void	knl_timer_handler_startup( void );

	/* Set timer */
	knl_init_hw_timer();

	/* Interrupt handler definition */
	knl_define_inthdr(TC0IRQ, knl_timer_handler_startup);

	/* Timer interrupt enable */
	SetIntMode(TC0IRQ, IM_LEVEL|IM_HI);
	EnableInt(TC0IRQ);
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
	_UW dummy;
	/* Enter on interrupt enable state */

	/* Disable its own interrupt to enable a multiplexed interrupt */
	out_w(AIC_BASE | AIC_IDCR, 1 << 6); /* Timer 0 disable */

	/* Clear timer interrupt */
	dummy = in_w(TCB0_BASE | TC_C0 | TC_SR);

	/* Multiplexed interrupt enable */
	ENAINT;
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* Interrupt disable */
	DISINT;

	out_w(AIC_BASE | AIC_EOICR, 1);

	/* Enable its own interrupt */
	out_w(AIC_BASE | AIC_IECR, 1 << 6); /* Timer 0 enable */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	/* Timer interrupt disable */
	DisableInt(TC0IRQ);
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
	UW	ofs, max, unf;
	UINT	imask;

	DI(imask);

	max = (in_w(TCB0_BASE | TC_C0 | TC_RC) & 0xffff) + 1;
	do {
		unf = in_w(AIC_BASE | AIC_IPR) & (1U << 6);
		ofs = in_w(TCB0_BASE | TC_C0 | TC_CVR) & 0xffffU;
	} while ( unf != (in_w(AIC_BASE | AIC_IPR) & (1U << 6)) );
	if ( unf != 0 ) {
		ofs += max;
	}

	EI(imask);

	return ofs * (1000 * knl_TimerClkDiv) / TMCLK;
}

#endif /* _TKDEV_TIMER_ */
