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
 *	tkdev_conf.h (AT91)
 *	Target System Configuration 
 */

#ifndef _TKDEV_CONF_
#define _TKDEV_CONF_
/* Also included from assembler source */

#define ARM_INT_MASK 0xC0
#define ARM_IRQ_BIT  0x80
#define ARM_FIQ_BIT  0x40
#define ARM_THM_BIT  0x20

#define ARM_MODE_MASK    0x1f
#define ARM_MODE_USER    0x10
#define ARM_MODE_FIQ     0x11
#define ARM_MODE_IRQ     0x12
#define ARM_MODE_SVC     0x13
#define ARM_MODE_ABORT   0x17
#define ARM_MODE_UNDEF   0x1F

/*
 * memories
 */
#define EXCEPTION_STACK_TOP     INTERNAL_RAM_END
#define APPLICATION_STACK_TOP   EXTERNAL_RAM_END

#define	TMP_STACK_SZ	(128)



#define TMCLK		16	/* Timer clock input (MHz) */

/*
 * Timer interrupt level
 */
#define TIMER_INTLEVEL		0

#endif /* _TKDEV_CONF_ */
