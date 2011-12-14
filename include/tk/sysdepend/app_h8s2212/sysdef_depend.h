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
 *	@(#)sysdef_depend.h (tk/H8S2212)
 *
 *	Definition about H8S
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

#define MSTP_CRA	((_UB*)(0x00fffde8UL))
#define MSTP_CRB	((_UB*)(0x00fffde8UL + 1UL))
#define EXMDLSTP	((_UB*)(0x00ffff40UL))

#define PB_DDR		((_UB*)(0x00fffe3aUL))
#define PB_PCR		((_UB*)(0x00fffe41UL))
#define PB_DR		((_UB*)(0x00ffff0aUL))

#define SCI2_SMR	((_UB*)(0x00ffff88UL))
#define SCI2_BRR	((_UB*)(0x00ffff89UL))
#define SCI2_SCR	((_UB*)(0x00ffff8aUL))
#define SCI2_TDR	((_UB*)(0x00ffff8bUL))
#define SCI2_SSR	((_UB*)(0x00ffff8cUL))
#define SCI2_RDR	((_UB*)(0x00ffff8dUL))
#define SCI2_SCMR	((_UB*)(0x00ffff8eUL))

#define TPU0_TCR	((_UB*)(0x00ffff10UL))
#define TPU0_TIER	((_UB*)(0x00ffff14UL))
#define TPU0_TSR	((_UB*)(0x00ffff15UL))
#define TPU0_TCNT	((_UH*)(0x00ffff16UL))
#define TPU0_TGRA	((_UH*)(0x00ffff18UL))

#define TPU_TSTR	((_UB*)(0x00fffeb0UL))


#endif /* __TK_SYSDEF_DEPEND_H__ */
