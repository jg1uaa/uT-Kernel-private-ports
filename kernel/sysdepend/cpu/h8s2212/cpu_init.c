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
 *	cpu_init.c (H8S2212)
 *	CPU-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include "task.h"
#include "cpu_insn.h"
#include "tkdev_conf.h"


EXPORT	INT	knl_taskindp = 0;

#if USE_DYNAMIC_INTHDR
Noinit(EXPORT	FP	knl_intvec[N_INTVEC]);
#endif /* USE_DYNAMIC_INTHDR */

EXPORT INT	knl_int_nest = 0;

Noinit(EXPORT	UB	knl_tmp_stack[TMP_STACK_SZ]);

/*
 * CPU-dependent initialization
 */
EXPORT ER knl_cpu_initialize( void )
{
	return E_OK;
}

#if USE_CLEANUP
/*
 * CPU-dependent finalization
 */
EXPORT void knl_cpu_shutdown( void )
{
}
#endif /* USE_CLEANUP */
