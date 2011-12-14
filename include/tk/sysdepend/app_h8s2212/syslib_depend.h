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
 *	@(#)syslib_depend.h (tk/H8S2212)
 *
 *	micro T-Kernel H8S Library
 */

#ifndef __TK_SYSLIB_DEPEND_H__
#define __TK_SYSLIB_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CPU interrupt control
 *	'intsts' is the value of CCR register in CPU
 *	disint()  Set CCR.I = 1 and return the original CCR to the return value.
 *	enaint()  Set CCR.I = intsts.I. Do not change except for CCR.I.
 *		  Return the original CCR to the return value.
 */
IMPORT UINT disint( void );
IMPORT UINT enaint( UINT intsts );
#define DI(intsts)	( (intsts) = disint() )
#define EI(intsts)	( enaint(intsts) )
#define isDI(intsts)	( ((intsts) & 0x0080U) != 0 )

/*
 * Interrupt vector
 *	The interrupt vector is the index number of the vector table.
 */
typedef UINT	INTVEC;

/* Convert to the interrupt definition number */
#define DINTNO(intvec)	(intvec)

/* ------------------------------------------------------------------------ */

/*
 * I/O port access
 *	Only memory mapped I/O for H8S
 */
Inline void out_w( UW port, UW data )
{
	Asm("mov.l %0, @%1":: "r"(data), "r"(port));
}
Inline void out_h( UW port, UH data )
{
	Asm("mov.w %0, @%1":: "r"(data), "r"(port));
}
Inline void out_b( UW port, UB data )
{
	Asm("mov.b %0l, @%1":: "r"(data), "r"(port));
}

Inline UW in_w( UW port )
{
	UW	data;
	Asm("mov.l @%1, %0": "=r"(data): "r"(port));
	return data;
}
Inline UH in_h( UW port )
{
	UH	data;
	Asm("mov.w @%1, %0": "=r"(data): "r"(port));
	return data;
}
Inline UB in_b( UW port )
{
	UB	data;
	Asm("mov.b @%1, %0l": "=r"(data): "r"(port));
	return data;
}

#ifdef __cplusplus
}
#endif
#endif /* __TK_SYSLIB_DEPEND_H__ */
