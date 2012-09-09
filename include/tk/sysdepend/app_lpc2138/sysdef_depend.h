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
 *	@(#)sysdef_depend.h (tk/lpc2138)
 *
 *	Definition about LPC2138
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * Program status register (PSR)
 */
#define PSR_N		0x80000000	/* Condition flag Negative */
#define PSR_Z		0x40000000	/* Zero */
#define PSR_C		0x20000000	/* Carry */
#define PSR_V		0x10000000	/* Overflow */

#define PSR_I		0x00000080	/* Interrupt (IRQ) disable */
#define PSR_F		0x00000040	/* Fast Interrupt (FIQ) disable */
#define PSR_T		0x00000020	/* Thumb mode */

#define PSR_DI		(PSR_I | PSR_F)	/* All Interrupts disable */

#define PSR_M(n)	( n )		/* Processor mode 0-31 */
#define PSR_USR		PSR_M(16)	/* User mode */
#define PSR_FIQ		PSR_M(17)	/* Fast Interrupt (FIQ) mode */
#define PSR_IRQ		PSR_M(18)	/* Interrupt (IRQ) mode */
#define PSR_SVC		PSR_M(19)	/* Supervisor mode */
#define PSR_ABT		PSR_M(23)	/* Abort mode */
#define PSR_UND		PSR_M(27)	/* Undefined order mode */
#define PSR_SYS		PSR_M(31)	/* System mode */

/* ------------------------------------------------------------------------ */

/*
 * Software interrupt number for micro T-Kernel 
 */
#define SWI_SVC		36	/* micro T-Kernel system call/extension SVC */
#define SWI_RETINT	37	/* tk_ret_int() system call */
#define SWI_DISPATCH	38	/* Task dispatcher */
#define SWI_DEBUG	39	/* Debugger support function */

/* ------------------------------------------------------------------------ */

/*
 * Vectored Interrupt Controller
 */
#define VICBase			0xfffff000
#define VICIRQStatus		(VICBase + 0x0000)
#define VICFIQStatus		(VICBase + 0x0004)
#define VICRawIntr		(VICBase + 0x0008)
#define VICIntSelect		(VICBase + 0x000c)
#define VICIntEnable		(VICBase + 0x0010)
#define VICIntEnClr		(VICBase + 0x0014)
#define VICSoftInt		(VICBase + 0x0018)
#define VICSoftIntClear		(VICBase + 0x001c)
#define VICProtection		(VICBase + 0x0020)
#define VICVectorAddr		(VICBase + 0x0030)		// LPC21xx/22xx
#define VICDefVectAddr		(VICBase + 0x0034)		// LPC21xx/22xx
#define VICVectAddr(x)		(VICBase + 0x0100 + ((x) * 4))
#define VICVectCntl(x)		(VICBase + 0x0200 + ((x) * 4))	// LPC21xx/22xx

/*
 * System Control Registers
 */
#define SystemControlBase	0xe01fc000
#define MEMMAP			(SystemControlBase + 0x0040)
#define PLLCON			(SystemControlBase + 0x0080)
#define PLLCFG			(SystemControlBase + 0x0084)
#define PLLSTAT			(SystemControlBase + 0x0088)
#define PLLFEED			(SystemControlBase + 0x008c)
#define PCONP			(SystemControlBase + 0x00c4)
#define APBDIV			(SystemControlBase + 0x0100)	// LPC21xx/22xx

#endif /* __TK_SYSDEF_DEPEND_H__ */
