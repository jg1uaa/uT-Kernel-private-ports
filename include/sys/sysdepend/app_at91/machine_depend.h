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
 *	@(#)machine_depend.h (sys/AT91)
 *
 *	Machine type definition (system dependent)
 */

#ifndef __SYS_MACHINE_DEPEND_H__
#define __SYS_MACHINE_DEPEND_H__

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * BIGENDIAN		1 if big endian 
 * VIRTUAL_ADDRESS	1 if virtual memory 
 * ALLOCA_NOSPT		1 if alloca() is not supported 
 */

/* ----- T-Engine Appliance AT91M55800A (ARM7TDMI) definition ----- */
#undef _APP_AT91_

#define _APP_AT91_		1
#define CPU_ARM4T		1
#define CPU_ARM720T		1
#define CPU_AT91		1
#define APP_AT91		1
#define ALLOW_MISALIGN		0
#define BIGENDIAN		0
#define VIRTUAL_ADDRESS		0
#define ALLOCA_NOSPT		0
#define INT_BITWIDTH		32

#ifndef _Csym
#define _Csym			0
#endif

#endif /* __SYS_MACHINE_DEPEND_H__ */
