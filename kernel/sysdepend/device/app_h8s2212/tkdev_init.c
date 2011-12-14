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
 *	tkdev_init.c (H8S2212)
 *	micro T-Kernel Device-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include <tk/sysdef.h>
#include <tk/syslib.h>
#include <tm/tmonitor.h>




/*
 * Target system-dependent initialization
 */
EXPORT ER knl_tkdev_initialize( void )
{
	/* port settings */
	*PB_PCR = 0x00U;
	*PB_DR = 0x85U;
	*PB_DDR = 0xFBU;

	*MSTP_CRA |= (1U << 6);	/* DMA controller */
	*EXMDLSTP |= (1U << 1);	/* RTC */
	*EXMDLSTP |= (1U << 0);	/* USB */

	return E_OK;
}

#if USE_CLEANUP
/*
 * Target system-dependent finalization
 *	No return from this function.
 */
EXPORT void knl_tkdev_exit( void )
{
	disint();
	tm_exit(0);	/* Turn off power and exit */

	/* Not suppose to return from 'tm_exit,' but just in case */
	for ( ;; ) {
		tm_monitor();  /* To T-Monitor equivalent function */
	}
}
#endif /* USE_CLEANUP */
