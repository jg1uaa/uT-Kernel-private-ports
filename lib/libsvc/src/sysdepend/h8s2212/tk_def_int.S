/*
 *	micro T-Kernel SVC interface library (h8s2212)
 *
 *	   created from /usr/local/te/include/tk/syscall.h
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tkfncd.h>
#include "utk_config.h"

	.h8300s

#if USE_TRAP || (USE_DBGSPT && USE_HOOK_TRACE)
	.text
	.balign	2
	.globl	Csym(tk_def_int)
Csym(tk_def_int):
	mov.w	r0, @-er7
	mov.w	#TFN_DEF_INT, r0
#if USE_TRAP
	trapa	#TRAP_SVC
#else
	jsr	Csym(knl_call_entry)
#endif
	inc.l	#2, er7
	rts
#endif

