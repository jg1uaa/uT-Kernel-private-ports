/*
 *	micro T-Kernel SVC interface library (lpc2138)
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tkfncd.h>
#include "utk_config.h"

#if USE_TRAP || (USE_DBGSPT && USE_HOOK_TRACE)
	.text
	.balign	4
	.globl	Csym(tk_sta_cyc)
	.type	Csym(tk_sta_cyc), %function
Csym(tk_sta_cyc):
	stmfd	sp!, {r4}
	add	r4, sp, #4
	stmfd	sp!, {lr}
	ldr	ip, =TFN_STA_CYC
#if USE_TRAP
	swi	SWI_SVC
#else
	bl	Csym(knl_call_entry)
#endif
	ldmfd	sp!, {lr}
	ldmfd	sp!, {r4}
	bx	lr
#endif

