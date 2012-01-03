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
 *	@(#)sysinfo_common.h (sys)
 *
 *	System shared information  
 */

#ifndef __SYS_SYSINFO_COMMON_H__
#define __SYS_SYSINFO_COMMON_H__

#ifdef APP_AT91
#  include <sys/sysdepend/app_at91/sysinfo_depend.h>
#endif

#ifdef APP_H83052
#  include <sys/sysdepend/app_h83052/sysinfo_depend.h>
#endif

#ifdef APP_H83069
#  include <sys/sysdepend/app_h83069/sysinfo_depend.h>
#endif

#ifdef APP_H8S2212
#  include <sys/sysdepend/app_h8s2212/sysinfo_depend.h>
#endif

#endif /* __SYS_SYSINFO_COMMON_H__ */
