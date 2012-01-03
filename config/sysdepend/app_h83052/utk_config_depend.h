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
 *	utk_config_depend.h (h83052)
 *	System Configuration Definition
 */

/* ROMINFO */
#define SYSTEMAREA_TOP	0x00ffe100	/* RAM system area top */
#define SYSTEMAREA_END	0x00fffe50	/* RAM system area end */

/* User definition */
#define RI_USERAREA_TOP	0x00ffff10	/* RAM user area top */
#define RI_USERINIT	NULL	/* User initialization program */

/* Stacks */
#define RI_INTSTACK	0x00ffff10 /* Interrupt stack top(internal RAM end) */


/* SYSCONF */
#define CFN_TIMER_PERIOD	10
#define CFN_MAX_TSKID		2
#define CFN_MAX_SEMID		1
#define CFN_MAX_FLGID		1
#define CFN_MAX_MBXID		1
#define CFN_MAX_MTXID		0
#define CFN_MAX_MBFID		0
#define CFN_MAX_PORID		0
#define CFN_MAX_MPLID		0
#define CFN_MAX_MPFID		0
#define CFN_MAX_CYCID		0
#define CFN_MAX_ALMID		0
#define CFN_MAX_SSYID		0
#define CFN_MAX_SSYPRI		0

#define CFN_MAX_REGDEV		(0)
#define CFN_MAX_OPNDEV		(0)
#define CFN_MAX_REQDEV		(0)
#define CFN_DEVT_MBFSZ0		(-1)
#define CFN_DEVT_MBFSZ1		(-1)

#define CFN_VER_MAKER		0
#define CFN_VER_PRID		0
#define CFN_VER_SPVER		0x6101
#define CFN_VER_PRVER		0x0101
#define CFN_VER_PRNO1		0
#define CFN_VER_PRNO2		0
#define CFN_VER_PRNO3		0
#define CFN_VER_PRNO4		0

#define CFN_REALMEMEND		((VP)0x00ffff10)

/*
 * Initial task priority
 */
#define INIT_TASK_PRI	(MAX_PRI-2)

/*
 * Use non-clear section
 */
#define USE_NOINIT	(1)

/*
 * Use dynamic memory allocation
 */
#define USE_IMALLOC	(1)

/*
 * Use program trace function (in debugger support)
 */
#define USE_HOOK_TRACE	(0)

/*
 * Use clean-up sequence
 */
#define USE_CLEANUP	(1)


/*
 * Use full interrupt vector
 */
#define USE_FULL_VECTOR	(1)

/*
 * Use high level programming language support routine
 */
#define USE_HLL_INTHDR	(1)

/*
 * Use dynamic interrupt handler change
 *   USE_FULL_VECTOR must be set to 0 if this macro is set to 0.
 */
#define USE_DYNAMIC_INTHDR	(1)


