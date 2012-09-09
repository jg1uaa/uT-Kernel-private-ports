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
 *	@(#)syslib_depend.h (tk/LPC2388)
 *
 *	micro T-Kernel LPC2388 Library
 */

#ifndef __TK_SYSLIB_DEPEND_H__
#define __TK_SYSLIB_DEPEND_H__

#include <tk/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CPU interrupt control
 *	'intsts' is the value of CPSR in CPU
 *	disint()  Set CPSR.I = 1 and F = 1 and return the original CPSR
 *		  to the return value.
 *	enaint()  Set CPSR.I,F = intsts.I,F. Do not change except
 *		  for CPSR.I,F.
 *		  Return the original CPSR to the return value.
 */
IMPORT UINT disint( void );
IMPORT UINT enaint( UINT intsts );
#define DI(intsts)	( (intsts) = disint() )
#define EI(intsts)	( enaint(intsts) )
#define isDI(intsts)	( ((intsts) & 0x00c0U) != 0 )

/*
 * Interrupt vector
 *	The interrupt vector is the index number of the vector table.
 */
typedef UINT	INTVEC;

/* Convert to interrupt definition number */
#define DINTNO(intvec)	(intvec)

/*
 * Interrupt enable
 *	Enable the interrupt specified by 'intvec.'
 *	intpri is interrupt priority, 0 (Highest) ... 15 (Lowest).
 */
IMPORT void EnableInt( INTVEC intvec, INT intpri );

/*
 * Interrupt disable
 *	Disable the interrupt specified by 'intvec.'
 */
IMPORT void DisableInt( INTVEC intvec );

/*
 * Check for existence of interrupt request
 *	Check whether there is an interrupt request specified by 'intvec.'
 *	If there is an interrupt request, return TRUE (except 0).
 */
IMPORT BOOL CheckInt( INTVEC intvec );

/*
 * Issue EOI(End Of Interrupt)
 */
IMPORT void EndOfInt( INTVEC intvec );

/*
 * Interrupt priority mask
 *	Mask all interrupts which have equal or lower priority.
 *	intpri is priority mask, 0 ... 16.
 *	0/16 masks/unmasks all interrupts, 7 masks priority 7 ... 15.
 *	Return value is old priority mask (intpri value).
 */
IMPORT INT SetIntLevel( INT intpri );

/* ------------------------------------------------------------------------ */

/*
 * I/O port access
 *	Only memory mapped I/O for Arm
 */
Inline void out_w( UW port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( UW port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( UW port, UB data )
{
	*(_UB*)port = data;
}

Inline UW in_w( UW port )
{
	return *(_UW*)port;
}
Inline UH in_h( UW port )
{
	return *(_UH*)port;
}
Inline UB in_b( UW port )
{
	return *(_UB*)port;
}

#ifdef __cplusplus
}
#endif
#endif /* __TK_SYSLIB_DEPEND_H__ */
