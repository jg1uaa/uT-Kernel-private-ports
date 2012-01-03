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
 *	cpu_task.h (H83052)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

#include "cpu_insn.h"

/*
 * System stack configuration at task startup
 */
typedef struct {
	UW	er[7]; /* er6-er0 */
	UW	sccr_spc;
} SStackFrame;


/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 1 )	/* To 'sccr_spc' position */

/*
 * Initial value for task startup
 */
#define INIT_CCR	( 0 )

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;
	UW		ccr;

	ssp = tcb->isstack;
	ssp--;

	ccr = INIT_CCR;

	/* CPU context initialization */
	/* Initial CCR */ /* Task startup address */
	ssp->sccr_spc = (ccr << 24) | ((UW)tcb->task & 0x00ffffffUL);
	tcb->tskctxb.ssp = ssp;			/* System stack */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->er[6] = stacd;				/* er0 */
	ssp->er[5] = (VW)tcb->exinf;	/* er1 */
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
}

#endif /* _CPU_TASK_ */
