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
 *	cpu_support.S (H83052)
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

#if	USE_DBGSPT | USE_HOOK_TRACE
#error	not supported
#endif

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

	.h8300h


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

	push.l	er0				// Context save
	push.l	er1
	push.l	er2
	push.l	er3
	push.l	er4
	push.l	er5
	push.l	er6

	mov.l	#Csym(knl_ctxtsk), er6		// er6 := &ctxtsk
	mov.l	@er6, er1
	mov.l	@Csym(CTXB_ssp), er0
	add.l	er0, er1			// er1 = &ctxtsk.ssp
	mov.l	er7, @er1			// Save SSP to TCB

  l_dispatch0:
	/* During interrupt enable CCR.I=0 */

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
	mov.l	@Csym(CTXB_ssp), er1
	add.l	er5, er1
	mov.l	@er1, er7			// Restore SSP from TCB

	sub.w	r1, r1
	mov.w	r1, @Csym(knl_dispatch_disabled)	// Dispatch enable 

	pop.l	er6				// Context restore
	pop.l	er5
	pop.l	er4
	pop.l	er3
	pop.l	er2
	pop.l	er1
	pop.l	er0

	rte

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
	push.l	er2			// er0-er1 are already saved
	push.l	er3
	push.l	er4
	push.l	er5
	push.l	er6

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

	shll.l	er0			// offset = dintno << 2
	shll.l	er0
	mov.l	@(Csym(knl_hll_inthdr), er0), er3	// call hll_inthdr[n](dintno)
	jsr	@er3

	orc.b	#CCR_I, ccr		// Interrupt disable

	/* During interrupt disable CCR.I=1 */
	mov.w	r5, r5
	bne	l_no_change_sp2		// multiple interrupt

	mov.l	@er7+, er2		// er2 = SSP
	mov.l	er2, er7
l_no_change_sp2:

	dec.w	#1, r6
	mov.w	r6, @Csym(knl_taskindp)

	pop.l	er6
	pop.l	er5
	pop.l	er4
	pop.l	er3
	pop.l	er2

#if USE_TRAP
	trapa	#TRAP_RETINT		// tk_ret_int()
#else
	jmp	@Csym(tk_ret_int_impl)
#endif

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

	pop.l	er1			// ER0, ER1 restore
	pop.l	er0
	jmp	_ret_int_dispatch	// To dispatch processing

  l_nodispatch:				// Dispatch not required
	pop.l	er1			// ER0, ER1 restore
	pop.l	er0
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
 *	High Address	    +-------------------+
 *			+16 | 5th arg (low)	|	
 *			+14 | 5th arg (high)	|	
 *			+12 | 4th arg (low)	|
 *			+10 | 4th arg (high)	|
 *			+ 8 | PC (low)		|
 *			+ 6 | PC (high)		| saved by I/F call
 *			+ 4 | R0		|	
 *			+ 2 | PC (low)		| saved by trapa
 *	er7, er5 =>	+ 0 | CCR:PC (high)	|
 *	Low Address	    +-------------------+
 *			     <-- 16bit width -->	
 *
 *	Function code is set in R0
 */
	.text
	.balign	2
	.globl	Csym(knl_call_entry)
Csym(knl_call_entry):

	push.l	er4			// er4, er6 = working register
	push.l	er5			// er6 is used for pass fncd
	push.l	er6			//   between hook_enter and hook_leave

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

	mov.w	r0, r4		// r4 := Function code
	bpl	l_esvc_function

#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
	/* micro T-Kernel system call */
	and.b	#0x0f, r4l		// Number of arguments
	cmp.b	#3, r4l
	ble	l_nocopy
	cmp.b	#4, r4l
	ble	l_copy4
	mov.l	@(3*4 + 14, er5), er4	// Copy fifth argument
	mov.l	er4, @-er7
  l_copy4:
	mov.l	@(3*4 + 10, er5), er4	// Copy fourth argument
	mov.l	er4, @-er7
  l_nocopy:

	and.w	#0x0ff0, r0	// r0 = function No. << 4
	shlr.w	r0		// r0 = function No. << 3
	shlr.w	r0		// r0 = function No. << 2
	mov.w	r0, r4
	extu.l	er4		// er4 = function No. * 4
	mov.w	@(3*4 + 4, er5), r0	// restore first argument
	mov.l	@(_svctbl, er4), er4
	jsr	@er4			// micro T-Kernel system call
#else
	bra	l_illegal_svc
#endif

  l_retsvc:
	// Interrupt disable
	orc	#CCR_I, ccr

	mov.l	er5, er7
	pop.l	er6
	pop.l	er5
	pop.l	er4
	rte

  l_illegal_svc:
	mov.l	#E_RSFN, er0
	bra	l_retsvc

  l_esvc_function:
#if CFN_MAX_SSYID > 0
/*
 *	High Address	    +-------------------+
 *			+38 | 5th arg (low)	|
 *			+36 | 5th arg (high)	|
 *			+34 | 4th arg (low)	|
 *			+32 | 4th arg (high)	|
 *			+30 | PC (low)		| saved by I/F call
 *			+28 | PC (high)		|
 *			+26 | 2nd arg (low)	|
 *			+24 | 2nd arg (high)	|
 *			+22 | 1st arg (low)	|
 *	pk_para(r1) =>	+20 | 1st arg (high)	|
 *			+18 | 3rd arg (low)	|
 *			+16 | 3rd arg (high)    |
 *			+14 | PC (low)		| saved by trapa
 *			+12 | CCR:PC (high)	|
 *			+10 | R4		|
 *			+ 8 | E4		|
 *			+ 6 | R5		|
 *			+ 4 | E5		|
 *			+ 2 | R6		|
 *	er7, er5 =>	+ 0 | E6		|
 *	Low Address	    +-------------------+
 *			     <-- 16bit width -->	
 *
 *	Function code is set in R0
 */
	/* Extended SVC */
	mov.l	@(28, er7), er4		// save ret-address(I/F) to ER4
	mov.l	@(16, er7), er2		// arg3
	mov.l	er2, @(28, er7)
	mov.l	er1, er2		// save ER1 (pk_para) to ER2
	mov.w	r0, r1			// R1 = Function code
	mov.l	er2, er0		// restore pk_para to ER0
	jsr	Csym(knl_svc_ientry)	// svc_ientry(pk_para, fncd)

	mov.l	er4, @(28, er7)		// restore ret-address(I/F)

	bra	l_retsvc
#else
	mov.l	#E_SYS, er0
	bra	l_retsvc
#endif /* CFN_MAX_SSYID > 0 */

#endif /* USE_TRAP | USE_DBGSPT | CFN_MAX_SSYID > 0 */

/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for timer handler
 */
	.text
	.balign	2
	.globl	Csym(knl_timer_handler_startup)
Csym(knl_timer_handler_startup):
	/* During interrupt disable CCR.I=1 */
	push.l	er0
	push.l	er1
	push.l	er2
	push.l	er3
	push.l	er4
	push.l	er5
	push.l	er6

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

	pop.l	er6
	pop.l	er5
	pop.l	er4
	pop.l	er3
	pop.l	er2

#if USE_TRAP
	trapa	#TRAP_RETINT		// tk_ret_int()
#else
	jmp	@Csym(tk_ret_int_impl)
#endif

