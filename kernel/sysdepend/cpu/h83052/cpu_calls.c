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
 *	cpu_calls.c (H83052)
 *	CPU-Dependent System Call
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "check.h"
#include "cpu_task.h"

#include <sys/sysinfo.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"
#include "cpu_calls.h"
/** [END Common Definitions] */

#ifdef USE_FUNC_TK_DIS_DSP
/*
 * Dispatch enable/disable 
 */
SYSCALL ER tk_dis_dsp_impl( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_DISABLE;

	return E_OK;
}
#endif /* USE_FUNC_TK_DIS_DSP */

#ifdef USE_FUNC_TK_ENA_DSP
/*
 * Dispatch enable 
 */
SYSCALL ER tk_ena_dsp_impl( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_ENABLE;
	if ( knl_ctxtsk != knl_schedtsk ) {
		knl_dispatch();
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_ENA_DSP */

/* ------------------------------------------------------------------------ */

/*
 * High level programming language
 */

#ifdef USE_FUNC_HLL_INTHDR
/* High level programming language interrupt handler entry */
Noinit(EXPORT FP knl_hll_inthdr[N_INTVEC]);
#endif /* USE_FUNC_HLL_INTHDR */

#ifdef USE_FUNC_TK_DEF_INT
IMPORT FP knl_hll_inthdr[];

/* High level programming language-supported routine (General) */
IMPORT void knl_inthdr_startup();

/*
 * Interrupt handler definition
 */
SYSCALL ER tk_def_int_impl( UINT dintno, T_DINT *pk_dint )
{
#if USE_DYNAMIC_INTHDR
	FP	inthdr;

	CHECK_PAR(dintno < N_INTVEC);
#if !USE_HLL_INTHDR
	CHECK_PAR((pk_dint->intatr & TA_HLNG) == 0);
#endif

	if ( pk_dint != NULL ) {
		/* Set interrupt handler */
		CHECK_RSATR(pk_dint->intatr, TA_HLNG);

		inthdr = pk_dint->inthdr;

		BEGIN_CRITICAL_SECTION;
#if USE_HLL_INTHDR
		if ( (pk_dint->intatr & TA_HLNG) != 0 ) {
			knl_hll_inthdr[dintno] = inthdr;
			inthdr = knl_inthdr_startup;
		}
#endif
		knl_define_inthdr(dintno, inthdr);
		END_CRITICAL_SECTION;
	} else {
		/* Clear interrupt handler */
		switch ( dintno ) {
		  default:		inthdr = NULL;
		}

		BEGIN_CRITICAL_SECTION;
		knl_define_inthdr(dintno, inthdr);
#if USE_HLL_INTHDR
		knl_hll_inthdr[dintno] = NULL;
#endif
		END_CRITICAL_SECTION;
	}

	return E_OK;
#else
	return E_NOSPT;
#endif /* USE_DYNAMIC_INTHDR */
}
#endif /* USE_FUNC_TK_DEF_INT */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_SET_REG
/*
 * Set task register contents
 */
EXPORT void knl_set_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = tcb->tskctxb.ssp;

	if ( cregs != NULL ) {
		ssp = cregs->ssp;
	}

	if ( regs != NULL ) {
		for ( i = 0; i < 7; ++i ) {
			ssp->er[i] = regs->er[i];
		}
	}

	if ( eit != NULL ) {
		ssp->sccr_spc = (UW)eit->pc;
		ssp->sccr_spc &= 0x00ffffffUL; /* 24bit */
		ssp->sccr_spc |= (UW)eit->ccr << 24;
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp = cregs->ssp;
	}
}
#endif /* USE_FUNC_SET_REG */

#ifdef USE_FUNC_TK_SET_REG
/*
 * Set task register contents
 */
SYSCALL ER tk_set_reg_impl( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_REG */

#ifdef USE_FUNC_GET_REG
/*
 * Get task register contents
 */
EXPORT void knl_get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		for ( i = 0; i < 7; ++i ) {
			regs->er[i] = ssp->er[i];
		}
	}

	if ( eit != NULL ) {
		eit->pc = (VP)(ssp->sccr_spc & 0x00ffffffUL);
		eit->ccr = (VB)(ssp->sccr_spc >> 24);
	}

	if ( cregs != NULL ) {
		cregs->ssp = tcb->tskctxb.ssp;
	}
}
#endif /* USE_FUNC_GET_REG */

#ifdef USE_FUNC_TK_GET_REG
/*
 * Get task register contents
 */
SYSCALL ER tk_get_reg_impl( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_REG */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#ifdef USE_FUNC_TD_SET_REG
/*
 * Set task register
 */
SYSCALL ER td_set_reg_impl( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SET_REG */

#ifdef USE_FUNC_TD_GET_REG
/*
 * Get task register
 */
SYSCALL ER td_get_reg_impl( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_GET_REG */

#endif /* USE_DBGSPT */
