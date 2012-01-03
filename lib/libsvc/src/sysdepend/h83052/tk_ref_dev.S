/*
 *	micro T-Kernel SVC interface library (h83052)
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tkfncd.h>
#include "utk_config.h"

	.h8300h

#if USE_TRAP || (USE_DBGSPT && USE_HOOK_TRACE)
	.text
	.balign	2
	.globl	Csym(tk_ref_dev)
Csym(tk_ref_dev):
	mov.w	r0, @-er7
	mov.w	#TFN_REF_DEV, r0
#if USE_TRAP
	trapa	#TRAP_SVC
#else
	jsr	Csym(knl_call_entry)
#endif
	inc.l	#2, er7
	rts
#endif

