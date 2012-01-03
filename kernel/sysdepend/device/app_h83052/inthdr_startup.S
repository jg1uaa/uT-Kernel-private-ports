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
 * interrupt entry routines (included from vector.S)
 */

#include <tk/asm.h>

        .section .text
        .align 2

INT_ENTRY 1
INT_ENTRY 2
INT_ENTRY 3
INT_ENTRY 4
INT_ENTRY 5
INT_ENTRY 6
INT_ENTRY 7
#if !USE_TRAP
INT_ENTRY 8
#endif
#if (!USE_TRAP) & (CFN_MAX_SSYID <= 0)
INT_ENTRY 9
#endif
#if !USE_TRAP
INT_ENTRY 10
#endif
INT_ENTRY 11
INT_ENTRY 12
INT_ENTRY 13
INT_ENTRY 14
INT_ENTRY 15
INT_ENTRY 16
INT_ENTRY 17
INT_ENTRY 18
INT_ENTRY 19
INT_ENTRY 20
INT_ENTRY 21
INT_ENTRY 22
INT_ENTRY 23
//INT_ENTRY 24	/* timer */
INT_ENTRY 25
INT_ENTRY 26
INT_ENTRY 27
INT_ENTRY 28
INT_ENTRY 29
INT_ENTRY 30
INT_ENTRY 31
INT_ENTRY 32
INT_ENTRY 33
INT_ENTRY 34
INT_ENTRY 35
INT_ENTRY 36
INT_ENTRY 37
INT_ENTRY 38
INT_ENTRY 39
INT_ENTRY 40
INT_ENTRY 41
INT_ENTRY 42
INT_ENTRY 43
INT_ENTRY 44
INT_ENTRY 45
INT_ENTRY 46
INT_ENTRY 47
INT_ENTRY 48
INT_ENTRY 49
INT_ENTRY 50
INT_ENTRY 51
INT_ENTRY 52
INT_ENTRY 53
INT_ENTRY 54
INT_ENTRY 55
INT_ENTRY 56
INT_ENTRY 57
INT_ENTRY 58
INT_ENTRY 59
INT_ENTRY 60
