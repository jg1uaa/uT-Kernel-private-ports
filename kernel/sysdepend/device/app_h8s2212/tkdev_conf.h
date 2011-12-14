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
 *	tkdev_conf.h (H8S2212)
 *	Target System Configuration 
 */

#ifndef _TKDEV_CONF_
#define _TKDEV_CONF_
/* Also included from assembler source */

/* System clock*/
#define SYSTEM_CLOCK		(UW)(4000000UL)		/* 4MHz	*/

/* Timer0 counter */
#define TM0_COUNT			(UH)(SYSTEM_CLOCK/16/1000)

/* TCR */
#define TCNT_NOCLR			(UB)(0x00)
#define TCNT_TGRA			(UB)(0x20)
#define TCNT_TGRB			(UB)(0x40)
#define TCNT_MAX			(UH)(0xFFFF)

#define CLK_EDGE_H			(UB)(0x00)
#define CLK_EDGE_L			(UB)(0x08)
#define CLK_EDGE_HL			(UB)(0x10)
#define TM0_CKS_64			(UB)(0x03)
#define TM0_CKS_16			(UB)(0x02)
#define TM0_CKS_4			(UB)(0x01)
#define TM0_CKS_1			(UB)(0x00)

#define	TCR_CCLR_MASK		(UB)~(0x60U)
#define TM0_TPSC_MSK		(UB)~(0x07U)

/* TIER */
#define TMR0_TCIEV			(UB)(0x10)
#define TMR0_TGIEA			(UB)(0x01)

/* TSR */
#define	TMR0_TCFD			(UB)(0x80)
#define TMR0_TCFV			(UB)(0x10)
#define TMR0_TGFA			(UB)(0x01)

#define	TMP_STACK_SZ	(128)

#endif /* _TKDEV_CONF_ */
