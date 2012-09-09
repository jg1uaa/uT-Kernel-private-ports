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
 *	@(#)machine_common.h (sys)
 *
 *	Machine type definition
 */

#ifndef __SYS_MACHINE_COMMON_H__
#define __SYS_MACHINE_COMMON_H__

#ifdef _APP_AT91_
#  include <sys/sysdepend/app_at91/machine_depend.h>
#endif

#ifdef _APP_LPC2138_
#  include <sys/sysdepend/app_lpc2138/machine_depend.h>
#endif

#ifdef _APP_LPC2388_
#  include <sys/sysdepend/app_lpc2388/machine_depend.h>
#endif

#ifdef _APP_H83052_
#  include <sys/sysdepend/app_h83052/machine_depend.h>
#endif

#ifdef _APP_H83069_
#  include <sys/sysdepend/app_h83069/machine_depend.h>
#endif

#ifdef _APP_H8S2212_
#  include <sys/sysdepend/app_h8s2212/machine_depend.h>
#endif

#endif /* __SYS_MACHINE_COMMON_H__ */
