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
 *	@(#)int.c (libtk/LPC2138)
 *
 *	Interrupt controller
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intvec.
 */
EXPORT void EnableInt( INTVEC intvec )
{
	out_w(VICIntEnable, 1 << intvec);
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intvec.
 */
EXPORT void DisableInt( INTVEC intvec )
{
	out_w(VICIntEnClr, 1 << intvec);
}

/*
 * Check for interrupt requests 
 *	Checks for intvec interrupt  requests.
 *	If an interrupt request is found, returns TRUE (other than 0).
 */
EXPORT BOOL CheckInt( INTVEC intvec )
{
	if (intvec < 32) {
		return (BOOL)((in_w(VICRawIntr) & (1 << intvec)) != 0);
	}

	return FALSE;
}
