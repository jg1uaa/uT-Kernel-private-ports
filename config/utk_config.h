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
 *	utk_config.h
 *	System Configuration Definition
 */

#ifndef _UTK_CONFIG_H_
#define _UTK_CONFIG_H_

#include "config.h"

#ifdef _APP_H83052_
#include "sysdepend/app_h83052/utk_config_depend.h"
#endif

#ifdef _APP_H83069_
#include "sysdepend/app_h83069/utk_config_depend.h"
#endif

#ifdef _APP_H8S2212_
#include "sysdepend/app_h8s2212/utk_config_depend.h"
#endif

#ifdef _APP_LPC2138_
#include "sysdepend/app_lpc2138/utk_config_depend.h"
#endif

#ifdef _APP_LPC2388_
#include "sysdepend/app_lpc2388/utk_config_depend.h"
#endif

#ifdef _APP_AT91_
#include "sysdepend/app_at91/utk_config_depend.h"
#endif

#include "utk_config_func.h"

#if !USE_TRAP && !(USE_DBGSPT && USE_HOOK_TRACE)
#include "tksvcproxy.h"
#include "tdsvcproxy.h"
#endif

#endif /* _UTK_CONFIG_H_ */
