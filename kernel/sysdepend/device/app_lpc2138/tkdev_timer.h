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
 *	tkdev_timer.h (LPC2138)
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

/*
 * Timer definition
 */
#define	TimerBase	0xe0004000	// Timer0

#define	TnIR		(TimerBase + 0x0000)
#define	TnTCR		(TimerBase + 0x0004)
#define	TnTC		(TimerBase + 0x0008)
#define	TnPR		(TimerBase + 0x000c)
#define	TnPC		(TimerBase + 0x0010)
#define	TnMCR		(TimerBase + 0x0014)
#define	TnMR0		(TimerBase + 0x0018)
#define	TnMR1		(TimerBase + 0x001c)
#define	TnMR2		(TimerBase + 0x0020)
#define	TnMR3		(TimerBase + 0x0024)
#define	TnCCR		(TimerBase + 0x0028)
#define	TnCR0		(TimerBase + 0x002c)
#define	TnCR1		(TimerBase + 0x0030)
#define	TnCR2		(TimerBase + 0x0034)
#define	TnCR3		(TimerBase + 0x0038)
#define	TnEMR		(TimerBase + 0x003c)
#define	TnCTCR		(TimerBase + 0x0070)

#define	TimerIntVec	4

/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	UINT	imask;

	DI(imask);

	/* Power-up timer */
	out_w(PCONP, in_w(PCONP) | (1 << 1));

	/* Stop timer */
	out_w(TnTCR, 0);

	/* Set timer mode */
	out_w(TnCTCR, 0);		// timer mode
	out_w(TnEMR, 0);		// external match: do nothing
	out_w(TnCCR, 0);		// capture control: all disable
	out_w(TnPR, 0);			// prescaler: PCLK/1

	/* Set counter */
	out_w(TnMR0, (CPU_CLOCK / (1000 * 4)) * CFN_TIMER_PERIOD - 1);
	out_w(TnPC, 0);			// reset prescale count
	out_w(TnTC, 0);			// reset timer count
	out_w(TnIR, 0xff);		// clear all interrupts
	out_w(TnMCR, 3);		// interrupt enable, reset on MR

	/* Start timer count */
	out_w(TnTCR, 1);

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
	knl_define_inthdr(TimerIntVec, knl_timer_handler_startup);

	/* Timer interrupt enable */
	EnableInt(TimerIntVec);
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
	/* Enter on interrupt enable state */

	/* Disable its own interrupt to enable a multiplexed interrupt */
	out_w(VICIntEnClr, 1 << TimerIntVec);

	/* Clear timer interrupt */
	out_w(TnIR, 0xff);

	/* Multiplexed interrupt enable */
	ENAINT;
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* Interrupt disable */
	DISINT;

	/* Enable its own interrupt */
	out_w(VICIntEnable, 1 << TimerIntVec);
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	/* Timer interrupt disable */
	DisableInt(TimerIntVec);
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
