/*
 *	micro T-Kernel SVC interface library (lpc2388)
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tkfncd.h>
#include "utk_config.h"

#if USE_TRAP || (USE_DBGSPT && USE_HOOK_TRACE)
	.text
	.balign	4
	.globl	Csym(tk_srea_dev)
	.type	Csym(tk_srea_dev), %function
Csym(tk_srea_dev):
	stmfd	sp!, {r4}
	add	r4, sp, #4
	stmfd	sp!, {lr}
	ldr	ip, =TFN_SREA_DEV
#if USE_TRAP
	swi	SWI_SVC
#else
	bl	Csym(knl_call_entry)
#endif
	ldmfd	sp!, {lr}
	ldmfd	sp!, {r4}
	bx	lr
#endif

