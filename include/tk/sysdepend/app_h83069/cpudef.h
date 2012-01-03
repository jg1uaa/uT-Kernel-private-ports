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
 *	@(#)cpudef.h  (tk/H83069)
 *
 *	H8/300H dependent definition
 */

#ifndef __TK_CPUDEF_H__
#define __TK_CPUDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	er[7];	/* General purpose register ER0-ER6 */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	VP	pc;	/* Program counter */
	VB	ccr;	/* Status register */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	VP	ssp;	/* System stack pointer ER7 */
} T_CREGS;


#ifdef __cplusplus
}
#endif
#endif /* __TK_CPUDEF_H__ */
