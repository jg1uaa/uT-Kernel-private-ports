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
 *	cpu_support.S (H8S2212)
 *	Device-Dependent CPU Operation
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/errno.h>
#include <tk/sysdef.h>
#include <tk/asm.h>
#include <sys/sysinfo.h>

#include "config.h"
#include "utk_config.h"
#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
#include "isysconf.h"
#endif
#include "tkdev_conf.h"
#include "offset.h"

/* ------------------------------------------------------------------------ */
/*
 * Dispatcher
 *	dispatch_to_schedtsk:
 *		Throw away the current contexts and forcibly dispatch to
 *		'schedtsk.'
 *		Called directly by jump (jmp) but do not return.
 *		Called on the undefined stack state (undefined 'ssp').
 *		Called on the interrupt disable state.
 *	dispatch_entry:
 *		Normal dispatch processing.
 *	_ret_int_dispatch:
 *		Called when dispatch is required by 'tk_ret_int().'
 *
 *	Contexts to save
 *	Save registers except for SP to a stack. Save SP to TCB.
 *
 *		   High Address	+---------------+
 *				| SPC(24bit)	|
 *				| 		| Saved by interrupt entry
 *				| SCCR(8bit)	| routines until here.
 *				+---------------+
 *				| ER0		|
 *				| ER1		|
 *				| ER2		|
 *				| ER3		|
 *				| ER4		|
 *				| ER5		|
 *	Saved to TCB  SP =>	| ER6		|
 *		    Low Address	+---------------+
 */

/* Temporal stack used when 'dispatch_to_schedtsk' is called */

	.h8300s


	.text
	.balign	2
	.globl	Csym(knl_dispatch_to_schedtsk)
	.globl	Csym(knl_dispatch_entry)

Csym(knl_dispatch_to_schedtsk):
	/* During interrupt disable CCR.I=1 */
	mov.l	#(Csym(knl_tmp_stack) + TMP_STACK_SZ), er7	// Set temporal stack

	sub.w	r0, r0
	inc.w	#1, r0				// r0 := 1
	mov.w	r0, @Csym(knl_dispatch_disabled)	// Dispatch disable

	mov.l	#Csym(knl_ctxtsk), er6		// er6 := &ctxtsk
#if USE_DBGSPT
	mov.l	@er6, er1		// er1 := ctxtsk
#endif
	sub.l	er0, er0
	mov.l	er0, @er6		// ctxtsk = NULL

	andc	#CCR_EI_MASK, ccr		// Interrupt enable
	bra	l_dispatch0

Csym(knl_dispatch_entry):
	/* During interrupt disable CCR.I=1 */
#if USE_TRAP
	mov.w	@er7, r0
	and.b	#CCR_EI_MASK, r0h
	mov.w	r0, @er7		// SSR compensation (I=0)
#else
	mov.w	@er7, r0
	stc	ccr, r0h
	and.b	#CCR_EI_MASK, r0h
	mov.w	r0, @er7
#endif

_ret_int_dispatch:
	/* During interrupt disable CCR.I=1 */

	sub.w	r0, r0
	inc.w	#1, r0				// r0 := 1
	mov.w	r0, @Csym(knl_dispatch_disabled)	// Dispatch disable
	andc	#CCR_EI_MASK, ccr		// Interrupt enable

	stm.l	(er0-er3), @-er7		// Context save
	stm.l	(er4-er6), @-er7

	mov.l	#Csym(knl_ctxtsk), er6		// er6 := &ctxtsk
	mov.l	@er6, er1
	mov.l	er7, @(TCB_tskctxb + CTXB_ssp, er1)	// Save SSP to TCB

  l_dispatch0:
	/* During interrupt enable CCR.I=0 */
#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_stop_jmp, er0		// Hook processing
	jmp	@er0
  ret_hook_stop:
#endif

  l_dispatch1:
	orc	#CCR_I, ccr			// Interrupt disable

	mov.l	@Csym(knl_schedtsk), er5	// ER5 = schedtsk
						// Is there 'schedtsk'?
	bne	l_dispatch2

	/* Because there is no task that should be executed, move to the power-saving mode */
	mov.w	@Csym(knl_lowpow_discnt), r0	// Is 'low_pow' disabled?
	bne	l_dispatch11

	jsr	@Csym(knl_low_pow)		// call low_pow()
  l_dispatch11:

	andc	#CCR_EI_MASK, ccr		// Interrupt enable
	bra	l_dispatch1

  l_dispatch2:					// Switch to 'schedtsk'
	/* During interrupt disable CCR.I=1 */
	mov.l	er5, @er6			// ctxtsk = schedtsk
	mov.l	@(TCB_tskctxb + CTXB_ssp, er5), er7	// Restore SSP from TCB

#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_exec_jmp, er0		// Hook processing
	jmp	@er0
  ret_hook_exec:
#endif

	sub.w	r1, r1
	mov.w	r1, @Csym(knl_dispatch_disabled)	// Dispatch enable 

	ldm.l	@er7+, (er4-er6)		// Context restore
	ldm.l	@er7+, (er0-er3)

	rte

#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * Task dispatcher hook routine call
 *	void stop( ID tskid, INT lsid, UINT tskstat )
 *	void exec( ID tskid, INT lsid )
 */
	.text
	.balign	2
hook_stop:
	mov.l	er1, er1	// er1 = ctxtsk
	beq	ret_hook_stop	// no task

	mov.b	@(TCB_state, er1), r2l		// tskstat
	shll.b	r2l
	extu.w	r2

	mov.w	@(TCB_tskid, er1), r0		// tskid

	mov.l	@Csym(knl_hook_stopfn), er3
	jsr	@er3			// call stop(tskid, lsid, tskstat)

	jmp	ret_hook_stop

hook_exec:
	stc	ccr, r4l			// CCR save
	andc	#CCR_EI_MASK, ccr
	/* CCR.I=0 */

	mov.l	@er6, er1			// er1 = ctxtsk
	mov.w	@(TCB_tskid, er1), r0		// tskid

	mov.l	@Csym(knl_hook_execfn), er3
	jsr	@er3			// call exec(tskid, lsid)

	ldc	r4l, ccr			// CCR restore
	jmp	ret_hook_exec


/*
 * Set/Free task dispatcher hook routine
 */
	.text
	.balign	2
	.globl	Csym(knl_hook_dsp)
Csym(knl_hook_dsp):
	mov.l	#hook_exec, er2
	mov.l	#hook_stop, er3
	mov.l	er2, @hook_exec_jmp
	mov.l	er3, @hook_stop_jmp
	rts

	.globl	Csym(knl_unhook_dsp)
Csym(knl_unhook_dsp):
	mov.l	#ret_hook_exec, er2
	mov.l	#ret_hook_stop, er3
	mov.l	er2, @hook_exec_jmp
	mov.l	er3, @hook_stop_jmp
	rts

			.data
			.balign	4
  hook_exec_jmp:	.long	ret_hook_exec
  hook_stop_jmp:	.long	ret_hook_stop

#endif /* USE_DBGSPT & USE_HOOK_TRACE */

#if USE_HLL_INTHDR
/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for interrupt handler
 *	Called by interrupt entry routine on the state saved in the interrupt 
 *	stack as shown below.
 *	   High Address	+---------------+
 *			| SPC(24bit)	|
 *			| SCCR(8bit)	|
 *			| er0		|
 *		ISP =>	| er1		|
 *	    Low Address	+---------------+
 *
 *	er0 = intno
 */
	.text
	.balign	2
	.globl	Csym(knl_inthdr_startup)
Csym(knl_inthdr_startup):
	/* During interrupt disable CCR.I=1 */
	stm.l	(er2-er3), @-er7	// er0-er1 are already saved
	stm.l	(er4-er6), @-er7

	mov.w	@Csym(knl_taskindp), r6	// enter task independent mode
	inc.w	#1, r6
	mov.w	r6, @Csym(knl_taskindp)

	mov.w	@Csym(knl_int_nest), r2	// interrupt nest count
	mov.w	r2, r5
	inc.w	#1, r2
	mov.w	r2, @Csym(knl_int_nest)

	mov.w	r5, r5
	bne	l_no_change_sp		// multiple interrupt

	mov.l	er7, er2
	mov.l	#RI_INTSTACK, er7	// change to ISP
	mov.l	er2, @-er7		// SSP save
  l_no_change_sp:

#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_ienter_jmp, er3
	jmp	@er3
  ret_hook_ienter:
#endif

	shll.l	#2, er0			// offset = dintno << 2
	mov.l	@(Csym(knl_hll_inthdr), er0), er3	// call hll_inthdr[n](dintno)
	jsr	@er3

#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_ileave_jmp, er3
	jmp	@er3
  ret_hook_ileave:
#endif

	orc.b	#CCR_I, ccr		// Interrupt disable

	/* During interrupt disable CCR.I=1 */
	mov.w	r5, r5
	bne	l_no_change_sp2		// multiple interrupt

	mov.l	@er7+, er2		// er2 = SSP
	mov.l	er2, er7
l_no_change_sp2:

	dec.w	#1, r6
	mov.w	r6, @Csym(knl_taskindp)

	ldm.l	@er7+, (er4-er6)
	ldm.l	@er7+, (er2-er3)

#if USE_TRAP
	trapa	#TRAP_RETINT		// tk_ret_int()
#else
	jmp	@Csym(tk_ret_int_impl)
#endif

#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * Interrupt handler hook routine call
 */
	.text
	.balign	2
hook_ienter:
	mov.l	er0, @-er7	// Save dintno

	mov.l	@Csym(knl_hook_ienterfn), er3
	jsr	@er3		// call enter(dintno)

	mov.l	@er7, er0	// er0 restore

	jmp	ret_hook_ienter

hook_ileave:
	mov.l	@er7+, er0	// dintno

	mov.l	@Csym(knl_hook_ileavefn), er3
	jsr	@er3		// call leave(dintno)

	jmp	ret_hook_ileave

/*
 * Set/Free interrupt handler hook routine
 */
	.text
	.balign	2
	.globl	Csym(knl_hook_int)
Csym(knl_hook_int):
	mov.l	#hook_ienter, er2
	mov.l	#hook_ileave, er3
	mov.l	er2, @hook_ienter_jmp
	mov.l	er3, @hook_ileave_jmp
	rts

	.globl	Csym(knl_unhook_int)
Csym(knl_unhook_int):
	mov.l	#ret_hook_ienter, er2
	mov.l	#ret_hook_ileave, er3
	mov.l	er2, @hook_ienter_jmp
	mov.l	er3, @hook_ileave_jmp
	rts

			.data
			.balign	4
  hook_ienter_jmp:	.long	ret_hook_ienter
  hook_ileave_jmp:	.long	ret_hook_ileave

#endif /* USE_DBGSPT & USE_HOOK_TRACE */
#endif /* USE_HLL_INTHDR */

/*
 * _tk_ret_int()
 *
 * tk_ret_int() processing
 *	When called, the interrupt stack is configured as shown below.
 *	   High Address	+---------------+
 *			| SPC(24bit)	| Saved when interrupt occur
 *			| SCCR(8bit)	|
 *			| ER0(32bit)	|
 *		SSP =>	| ER1(32bit)	|
 *			+---------------+
 *			| SPC(24bit)	| Save by 'trapa TRAP_RETINT'
 *		SSP =>	| SCCR(8bit)	|
 *	    Low Address	+---------------+
 */
	.text
	.balign	2
	.globl	Csym(tk_ret_int_impl)
Csym(tk_ret_int_impl):
	/* During interrupt disable CCR.I=1 */
#if USE_TRAP
	add.l	#4, er7		// Trash parts saved by 'trapa TRAP_RETINT'
#endif

	mov.w	@Csym(knl_int_nest), r0	// Is it a nesting interrupt?
	dec.w	#1, r0
	mov.w	r0, @Csym(knl_int_nest)
	bne	l_nodispatch

	mov.w	@(2*4, er7), r0		// CCR when EIT occurs
	and.b	#CCR_I, r0h		// Is it an exception during interrupt disable?
	bne	l_nodispatch

	mov.w	@Csym(knl_dispatch_disabled), r0	// Is it during dispatch disable?
	bne	l_nodispatch

	mov.l	@Csym(knl_ctxtsk), er0		// Is dispatch required?
	mov.l	@Csym(knl_schedtsk), er1
	cmp	er0, er1
	beq	l_nodispatch

	ldm.l	@er7+, (er0-er1)	// ER0, ER1 restore
	jmp	_ret_int_dispatch	// To dispatch processing

  l_nodispatch:				// Dispatch not required
	ldm.l	@er7+, (er0-er1)	// ER0, ER1 restore
	rte


/* ------------------------------------------------------------------------ */

#if USE_TRAP | USE_DBGSPT | CFN_MAX_SSYID > 0
/*
 * Unsupported system call
 */
	.text
	.balign	2
	.globl	Csym(knl_no_support)
Csym(knl_no_support):
	mov.l	#E_RSFN, er0
	rts

/*
 * System call entry table
 */
#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
	.text
	.balign	2
_svctbl:
	.long	Csym(knl_no_support)
#define	tk_ret_int_impl	knl_no_support
#include <sys/svc/tksvctbl.h>
#undef	tk_ret_int_impl
#endif /* USE_TRAP | USE_DBGSPT */

/*
 * System call entry
 *	Do not need to save the temporary register.
 *	The compiler saves the permanent register.
 *
 *	   High Address	+---------------+
 *			| 5th arg	|
 *			| 4th arg	|
 *			| SPC(24bit)	| saved by I/F call
 *			| SCCR(8bit)	|
 *			|  R0(16bit)	|
 *			| SPC(24bit)	| saved by trapa
 *	     ISP/SSP =>	| SCCR(8bit)	|
 *	    Low Address	+---------------+
 *
 *	Function code is set in R0
 */
	.text
	.balign	2
	.globl	Csym(knl_call_entry)
Csym(knl_call_entry):

	stm.l	(er4-er6), @-er7	// er4, er6 = working register
					// er6 is used for pass fncd
					//   between hook_enter and hook_leave
	mov.l	er7, er5		// er5 = frame pointer

#if USE_TRAP
	mov.w	@(3*4, er7), r4		// SCCR
	ldc	r4h, ccr
#else
#if CFN_MAX_SSYID > 0
	mov.w	r0, r0			//	< 0: System call
					//	>= 0: Extended SVC
	blt	l_set_retccr

	mov.w	@(3*4, er7), r4		// SCCR
	ldc	r4h, ccr
	bra	l_set_end
l_set_retccr:
#endif
	mov.w	@(3*4, er7), r4		// SCCR
	stc	ccr, r4h
	mov.w	r4, @(3*4, er7)
l_set_end:
#endif

	/* The interrupt disable state keeps the caller's state CCR.I=? */

#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_enter_jmp, er4	// Hook processing
	jmp	@er4
  ret_hook_enter:
#endif

	mov.w	r0, r4		// r4 := Function code
	bpl	l_esvc_function

#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
	/* micro T-Kernel system call */
	and.b	#0x0f, r4l		// Number of arguments
	cmp.b	#3, r4l
	ble	l_nocopy
	cmp.b	#4, r4l
	ble	l_copy4
	mov.l	@(3*4 + 10 + 1*4, er5), er4	// Copy fifth argument
	mov.l	er4, @-er7
  l_copy4:
	mov.l	@(3*4 + 10, er5), er4	// Copy fourth argument
	mov.l	er4, @-er7
  l_nocopy:

	and.w	#0x0ff0, r0	// r0 = function No. << 4
	shlr.w	#2, r0		// r0 = function No. << 2
	mov.w	r0, r4
	extu.l	er4		// er4 = function No. * 4
	mov.w	@(3*4 + 4, er5), r0	// restore first argument
	mov.l	@(_svctbl, er4), er4
	jsr	@er4			// micro T-Kernel system call
#else
	bra	l_illegal_svc
#endif

  l_retsvc:
#if USE_DBGSPT & USE_HOOK_TRACE
	mov.l	@hook_leave_jmp, er4	// Hook processing
	jmp	@er4
  ret_hook_leave:
#endif

	// Interrupt disable
	orc	#CCR_I, ccr

	mov.l	er5, er7
	ldm.l	@er7+, (er4-er6)
	rte

  l_illegal_svc:
	mov.l	#E_RSFN, er0
	bra	l_retsvc

  l_esvc_function:
#if CFN_MAX_SSYID > 0
	/* Extended SVC */
	mov.l	@(7*4, er7), er4	// save ret-address(I/F) to ER4
	mov.l	@(4*4, er7), er2	// arg2
	mov.l	er2, @(7*4, er7)
	mov.l	er1, er2		// save ER1 (pk_para) to ER2
	mov.w	r0, r1			// R1 = Function code
	mov.l	er2, er0		// restore pk_para to ER0
	jsr	Csym(knl_svc_ientry)	// svc_ientry(pk_para, fncd)

	mov.l	er4, @(7*4, er7)	// restore ret-address(I/F)

	bra	l_retsvc
#else
	mov.l	#E_SYS, er0
	bra	l_retsvc
#endif /* CFN_MAX_SSYID > 0 */


#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * System call/Extended SVC hook routine call
 *	VP enter( FN fncd, TD_CALINF *calinf, ... )
 *	void leave( FN fncd, INT ret, VP exinf )
 *
 *	typedef struct td_calinf {
 *		VP	ssp;	System stack pointer
 *	} TD_CALINF;
 */
	.text
	.balign	2
hook_enter:
	stm.l	(er0-er2), @-er7	// Save argument

	mov.l	er7, er6		// er6 = ssp
	mov.l	er6, @-er7		// Create TD_CALINF
	mov.l	er7, er1		// er1 = &TD_CALINF

	mov.w	r0, r4			// r4 := fncd function code
	bpl	l_nocopy_para		// Extended SVC

	and.b	#0x0f, r4l		// Number of arguments
	cmp.b	#5, r4l
	beq	l_copy_para5
	cmp.b	#4, r4l
	beq	l_copy_para4
	cmp.b	#3, r4l
	beq	l_copy_para3
	cmp.b	#2, r4l
	beq	l_copy_para2
	cmp.b	#1, r4l
	beq	l_copy_para1
	bra	l_nocopy_para

  l_copy_para5:
	mov.l	@(3*4 + 10 + 1*4, er5), er4
	mov.l	er4, @-er7		// Fifth argument
  l_copy_para4:
	mov.l	@(3*4 + 10, er5), er4
	mov.l	er4, @-er7		// Fourth argument
  l_copy_para3:
	mov.l	@(0*4, er6), er4
	mov.l	er4, @-er7		// Third argument
  l_copy_para2:
	mov.l	@(1*4, er6), er4
	mov.l	er4, @-er7		// Second argument
  l_copy_para1:
	mov.l	@(2*4, er6), er4	// First argument(r0 & fncd)
	mov.w	@(3*4 + 4, er5), r4	// First argument(lower word, original)
	mov.l	er4, @-er7		// First argument

  l_nocopy_para:
	mov.l	er1, @-er7		// push &calinf

	mov.l	@Csym(knl_hook_enterfn), er4
	jsr	@er4			// exinf = enter(fncd, &calinf, ...)

  l_ret_hook_enter:
	mov.l	er6, er7		// Restore stack position
	mov.l	er0, er6		// Move 'exinf' to er6 temporally
	ldm.l	@er7+, (er0-er2)	// Restore argument
	mov.l	er6, @-er7		// Save 'exinf'
	mov.w	r0, r6			// Save fncd for hook_leave

	jmp	ret_hook_enter

  l_esvc_para:
	// Extended SVC
	mov.l	@(3*4 + 3*4 + 4 + 3*4, er6), er2	// ret-addr
	mov.l	@(3*4 + 3*4 + 4 + 1*4, er6), er4	// 3rd arg
	mov.l	er4, @(3*4 + 3*4 + 4 + 3*4, er6)	// 3rd arg
	mov.l	er2, @(3*4 + 3*4 + 4 + 1*4, er6)	// ret-addr

	mov.l	@(1*4, er6), er2	// er2 = pk_para
	mov.l	er2, @-er7		// push pk_para
	mov.l	er1, @-er7		// push &calinf

	mov.l	@Csym(knl_hook_enterfn), er4
	jsr	@er4			// exinf = enter(fncd, &calinf, ...)

	mov.l	@(3*4 + 3*4 + 4 + 3*4, er6), er2	// 3rd arg
	mov.l	@(3*4 + 3*4 + 4 + 1*4, er6), er4	// ret-addr
	mov.l	er4, @(3*4 + 3*4 + 4 + 3*4, er6)	// ret-addr
	mov.l	er2, @(3*4 + 3*4 + 4 + 1*4, er6)	// 3rd arg

	bra	l_ret_hook_enter


hook_leave:
	mov.w	r0, e6			// save ret to non-destructive register

	mov.w	r6, r4			// Function code
	bpl	l_hl1			// If fncd >= 0, extended SVC

	and.b	#0x0f, r4l		// Parameter count
	extu.w	r4
	sub.w	#3, r4
	bpl	l_hl2
  l_hl1:
	mov.b	#0, r4l
  l_hl2:
	mov.l	#0, er2			// er2(exinf) = NULL

	shll.b	#2, r4l			// r4l = used stack size for args
	extu.l	er4

	add.l	er7, er4		// Whether 'hook_enter' is executed
					// Check by stack usage
	sub.l	er5, er4
	beq	l_hl3
					// 'hook_enter' is executed
	mov.l	@(-1*4, er5), er2	// er2 = exinf

  l_hl3:
	mov.w	r0, r1			// ret
	mov.w	r6, r0			// fncd
	mov.l	@Csym(knl_hook_leavefn), er4
	jsr	@er4			// call leave(fncd, ret, exinf)

	mov.w	e6, r0			// r0 = ret restore

	jmp	ret_hook_leave

/*
 * Set/Free system call/extended SVC hook routine 
 */
	.text
	.balign	2
	.globl	Csym(knl_hook_svc)
Csym(knl_hook_svc):
	mov.l	#hook_enter, er2
	mov.l	#hook_leave, er3
	mov.l	er2, @hook_enter_jmp
	mov.l	er3, @hook_leave_jmp
	rts

	.globl	Csym(knl_unhook_svc)
Csym(knl_unhook_svc):
	mov.l	#ret_hook_enter, er2
	mov.l	#ret_hook_leave, er3
	mov.l	er2, @hook_enter_jmp
	mov.l	er3, @hook_leave_jmp
	rts

			.data
			.balign	4
  hook_enter_jmp:	.long	ret_hook_enter
  hook_leave_jmp:	.long	ret_hook_leave

#endif /* USE_DBGSPT & USE_HOOK_TRACE */

#endif /* USE_TRAP | USE_DBGSPT | CFN_MAX_SSYID > 0 */

/* ------------------------------------------------------------------------ */

#if USE_DBGSPT & (USE_TRAP | USE_HOOK_TRACE)
/*
 * Debugger support function service call entry table
 */

	.text
	.balign	2
_tdsvctbl:
	.long	Csym(knl_no_support)
#include <sys/svc/tdsvctbl.h>

/*
 * Debugger support function service call entry
 */
	.text
	.balign	2
	.globl	Csym(knl_call_dbgspt)
Csym(knl_call_dbgspt):

	/* During interrupt disable CCR.I=1 */

	stm.l	(er4-er5), @-er7
	mov.l	er7, er5

#if USE_TRAP
	mov.w	@(2*4, er7), r4		// SCCR
	ldc	r4h, ccr
#else
	mov.w	@(2*4, er7), r4		// SCCR
	stc	ccr, r4h
	mov.w	r4, @(2*4, er7)
#endif
	/* The interrupt disable state keeps the caller's state CCR.I=? */

	/* micro T-Kernel system call (DS) */
	mov.w	r0, r4
	and.b	#0x0f, r4l		// Number of arguments(max is 4)
	cmp.b	#3, r4l
	ble	b_nocopy
	mov.l	@(2*4 + 10, er5), er4	// Copy fourth argument
	mov.l	er4, @-er7
  b_nocopy:

	and.w	#0x0ff0, r0
	shlr.w	#2, r0
	mov.w	r0, r4
	extu.l	er4			// er4 = svctbl index * 4
	mov.w	@(4 + 2*4, er5), r0	// restore first argument
	mov.l	@(_tdsvctbl, er4), er4
	jsr	@er4			// micro T-Kernel system call (DS)

  b_retsvc:
	orc	#CCR_I, ccr

	mov.l	er5, er7
	ldm.l	@er7+, (er4-er5)
	rte

  b_illegal_svc:
	mov.l	#E_RSFN, er0
	bra	b_retsvc
#endif /* USE_DBGSPT & (USE_TRAP | USE_HOOK_TRACE) */



/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for timer handler
 */
	.text
	.balign	2
	.globl	Csym(knl_timer_handler_startup)
Csym(knl_timer_handler_startup):
	/* During interrupt disable CCR.I=1 */
	stm.l	(er0-er3), @-er7
	stm.l	(er4-er6), @-er7

	mov.w	@Csym(knl_taskindp), r6	// enter task independent mode
	inc.w	#1, r6
	mov.w	r6, @Csym(knl_taskindp)

	mov.w	@Csym(knl_int_nest), r2	// interrupt nest count
	mov.w	r2, r5
	inc.w	#1, r2
	mov.w	r2, @Csym(knl_int_nest)

	mov.w	r5, r5
	bne	l_no_change_sp_timerhdr		// multiple interrupt

	mov.l	er7, er2
	mov.l	#RI_INTSTACK, er7	// change to ISP
	mov.l	er2, @-er7		// SSP save
  l_no_change_sp_timerhdr:

	jsr	@Csym(knl_timer_handler)	// call timer_handler()

	orc.b	#CCR_I, ccr		// Interrupt disable

	/* During interrupt disable CCR.I=1 */
	mov.w	r5, r5
	bne	l_no_change_sp2_timerhdr		// multiple interrupt

	mov.l	@er7+, er2		// ER2 = SSP
	mov.l	er2, er7
  l_no_change_sp2_timerhdr:

	dec.w	#1, r6
	mov.w	r6, @Csym(knl_taskindp)

	ldm.l	@er7+, (er4-er6)
	ldm.l	@er7+, (er2-er3)

#if USE_TRAP
	trapa	#TRAP_RETINT		// tk_ret_int()
#else
	jmp	@Csym(tk_ret_int_impl)
#endif

