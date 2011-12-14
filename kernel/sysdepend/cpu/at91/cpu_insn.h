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
 *	cpu_insn.h (AT91)
 *	AT91M55800A-Dependent Operation
 */

#ifndef _CPU_INSN_
#define _CPU_INSN_

#include <sys/sysinfo.h>

/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

/*
 * Get CPSR
 */
Inline UINT knl_getCPSR( void )
{
	UINT	cpsr;
	Asm("mrs %0, cpsr": "=r"(cpsr));
	return cpsr;
}

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */


/*
 * Set interrupt handler
 */
Inline void knl_define_inthdr( INT vecno, FP inthdr )
{
IMPORT FP knl_irq_handler;

	knl_intvec[vecno] = inthdr;
	if (vecno < 32) {
		out_w((AIC_BASE | AIC_SVR0) + vecno*4, (UW)&knl_irq_handler);
	}
}

/*
 * If it is the task-independent part, TRUE
 */
Inline BOOL knl_isTaskIndependent( void )
{
	return ( knl_taskindp > 0 )? TRUE: FALSE;
}

/*
 * Move to/Restore task independent part
 */
Inline void knl_EnterTaskIndependent( void )
{
	knl_taskindp++;
}
Inline void knl_LeaveTaskIndependent( void )
{
	knl_taskindp--;
}

/* ------------------------------------------------------------------------ */

#endif /* _CPU_INSN_ */
