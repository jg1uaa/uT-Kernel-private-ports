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
 *	@(#)machine_depend.h (sys/H8S2212)
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

/* ----- T-Engine Appliance H8S/2212 definition ----- */
#undef _APP_H8S2212_

#define _APP_H8S2212_		1
#define CPU_H8S		1
#define CPU_H8300		1
#define CPU_H8S2212		1
#define APP_H8S2212		1
#define ALLOW_MISALIGN		0
#define BIGENDIAN		1
#define VIRTUAL_ADDRESS		0
#define ALLOCA_NOSPT		0
#define INT_BITWIDTH		16

#ifndef _Csym
#define _Csym			1
#endif

#endif /* __SYS_MACHINE_DEPEND_H__ */
