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
 *	@(#)int.c (libtk/LPC2388)
 *
 *	Interrupt controller
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intvec.
 *	intpri is interrupt priority, 0 (Highest) ... 15 (Lowest).
 */
EXPORT void EnableInt( INTVEC intvec, INT intpri )
{
	UINT	imask;

	DI(imask);

	out_w(VICVectPriority(intvec), intpri);
	out_w(VICIntEnable, 1 << intvec);

	EI(imask);
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

/*
 * Notify end of interrupt
 */
EXPORT void EndOfInt( INTVEC intvec )
{
	/* write anything to notify */
	out_w(VICAddress, 0);
}

/*
 * Interrupt priority mask
 *	Masks all interrupts which have equal or lower priority.
 *	intpri is priority mask, 0 ... 16.
 *	0/16 masks/unmasks all interrupts, 7 masks priority 7 ... 15.
 *	Return value is old priority mask (intpri value).
 */
EXPORT INT SetIntLevel( INT intpri )
{
	UINT	imask, pmask;
	INT	lv;

	DI(imask);

	pmask = in_w(VICSWPriorityMask) & 0xffff;
	out_w(VICSWPriorityMask, 0xffff >> (16 - intpri));

	EI(imask);

	for (lv = 0; ; lv++, pmask >>= 1) if (!pmask) break;
	return lv;
}
