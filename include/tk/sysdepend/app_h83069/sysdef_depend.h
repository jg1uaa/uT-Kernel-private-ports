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
 *	@(#)sysdef_depend.h (tk/H83069)
 *
 *	Definition about H8/300H
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * Use specify register
 */
#define SP	er7	/* Stack pointer */

/*
 * CCR register
 */
#define CCR_I		( 0x01 << 7 )	/* Interrupt mask (mode 0) */
#define CCR_EI_MASK	( 0x7f )		/* Interrupt mask (mode 0) */


#define TRAP_DISPATCH	0x0	/* Dispatch */
#define TRAP_SVC	0x1	/* System call, Extension SVC */
#define TRAP_RETINT	0x2	/* System call, Extension SVC */
#define TRAP_DEBUG	0x3	/* Debugger support function */

/*----------------------------------------------------------*/

#endif /* __TK_SYSDEF_DEPEND_H__ */
