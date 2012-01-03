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
 * Vector table
 */

#include "utk_config.h"


#define int_ill loop_trap

        .section .vector, "a"
        .align 2

        .global __reset

#if !USE_FULL_VECTOR
__reset:
	.long   Csym(start)         /* 0: reset */
	.long   int_ill             /* 1: reserved */
	.long   int_ill             /* 2: reserved */
	.long   int_ill             /* 3: reserved */
	.long   int_ill             /* 4: reserved */
	.long   int_ill             /* 5: reserved */
	.long   int_ill             /* 6: reserved */
	.long	int_ill             /* 7: NMI */
#if USE_TRAP
	.long   Csym(knl_dispatch_entry) /* 8: trap (#0) */
#else
	.long	int_ill                  /* 8: trap (#0) */
#endif
#if USE_TRAP | CFN_MAX_SSYID > 0
	.long   Csym(knl_call_entry)     /* 9: trap (#1) */
#else
	.long	int_ill                  /* 9: trap (#1) */
#endif
#if USE_TRAP
	.long   Csym(tk_ret_int_impl)    /* 10: trap (#2) */
#else
	.long	int_ill                  /* 10: trap (#2) */
#endif
	.long	int_ill             /* 11: trap (#3) */
	.long   int_ill             /* 12: IRQ0 */
	.long   int_ill             /* 13: IRQ1 */
	.long   int_ill             /* 14: IRQ2 */
	.long   int_ill             /* 15: IRQ3 */
	.long   int_ill             /* 16: IRQ4 */
	.long   int_ill             /* 17: IRQ5 */
	.long   int_ill             /* 18: reserved */
	.long   int_ill             /* 19: reserved */
	.long   int_ill             /* 20: WOVI */
	.long   int_ill             /* 21: CMI */
	.long   int_ill             /* 22: reserved */
	.long   int_ill             /* 23: ADI */
	.long   Csym(knl_timer_handler_startup) /* 24: IMIA0 */
	.long   int_ill             /* 25: IMIB0 */
	.long   int_ill             /* 26: OVI0 */
	.long   int_ill             /* 27: reserved */
	.long   int_ill             /* 28: IMIA1 */
	.long   int_ill             /* 29: IMIB1 */
	.long   int_ill             /* 30: OVI1 */
	.long   int_ill             /* 31: reserved */
	.long   int_ill             /* 32: IMIA2 */
	.long   int_ill             /* 33: IMIB2 */
	.long   int_ill             /* 34: OVI2 */
	.long   int_ill             /* 35: reserved */
	.long   int_ill             /* 36: CMIA0 */
	.long   int_ill             /* 37: CMIB0 */
	.long   int_ill             /* 38: CMIA1/CMIB1 */
	.long   int_ill             /* 39: TOVI0/TOVI1 */
	.long   int_ill             /* 40: CMIA2 */
	.long   int_ill             /* 41: CMIB2 */
	.long   int_ill             /* 42: CMIA3/CMIB3 */
	.long   int_ill             /* 43: TOVI2/TOVI3 */
	.long   int_ill             /* 44: DEND0A */
	.long   int_ill             /* 45: DEND0B */
	.long   int_ill             /* 46: DEND1A */
	.long   int_ill             /* 47: DEND1B */
	.long   int_ill             /* 48: reserved */
	.long   int_ill             /* 49: reserved */
	.long   int_ill             /* 50: reserved */
	.long   int_ill             /* 51: reserved */
	.long   int_ill             /* 52: ERI0 */
	.long   int_ill             /* 53: RXI0 */
	.long   int_ill             /* 54: TXI0 */
	.long   int_ill             /* 55: TEI0 */
	.long   int_ill             /* 56: ERI1 */
	.long   int_ill             /* 57: RXI1 */
	.long   int_ill             /* 58: TXI1 */
	.long   int_ill             /* 59: TEI1 */
	.long   int_ill             /* 60: ERI2 */
	.long   int_ill             /* 61: RXI2 */
	.long   int_ill             /* 62: TXI2 */
	.long   int_ill             /* 63: TEI2 */

#else /* USE_FULL_VECTOR */

__reset:
	.long   Csym(start)           /* 0: reset */
	.long   knl_inthdr_entry1     /* 1: reserved */
	.long   knl_inthdr_entry2     /* 2: reserved */
	.long   knl_inthdr_entry3     /* 3: reserved */
	.long   knl_inthdr_entry4     /* 4: reserved */
	.long   knl_inthdr_entry5     /* 5: reserved */
	.long   knl_inthdr_entry6     /* 6: reserved */
	.long	knl_inthdr_entry7     /* 7: NMI */
#if USE_TRAP
	.long   Csym(knl_dispatch_entry) /* 8: trap (#0) */
#else
	.long	knl_inthdr_entry8        /* 8: trap (#0) */
#endif
#if USE_TRAP | CFN_MAX_SSYID > 0
	.long   Csym(knl_call_entry)     /* 9: trap (#1) */
#else
	.long	knl_inthdr_entry9        /* 9: trap (#1) */
#endif
#if USE_TRAP
	.long   Csym(tk_ret_int_impl)    /* 10: trap (#2) */
#else
	.long	knl_inthdr_entry10       /* 10: trap (#2) */
#endif
	.long	knl_inthdr_entry11    /* 11: trap (#3) */
	.long   knl_inthdr_entry12    /* 12: IRQ0 */
	.long   knl_inthdr_entry13    /* 13: IRQ1 */
	.long   knl_inthdr_entry14    /* 14: IRQ2 */
	.long   knl_inthdr_entry15    /* 15: IRQ3 */
	.long   knl_inthdr_entry16    /* 16: IRQ4 */
	.long   knl_inthdr_entry17    /* 17: IRQ5 */
	.long   knl_inthdr_entry18    /* 18: reserved */
	.long   knl_inthdr_entry19    /* 19: reserved */
	.long   knl_inthdr_entry20    /* 20: WOVI */
	.long   knl_inthdr_entry21    /* 21: CMI */
	.long   knl_inthdr_entry22    /* 22: reserved */
	.long   knl_inthdr_entry23    /* 23: ADI */
	.long   Csym(knl_timer_handler_startup)    /* 24: IMIA0 */
	.long   knl_inthdr_entry25    /* 25: IMIB0 */
	.long   knl_inthdr_entry26    /* 26: OVI0 */
	.long   knl_inthdr_entry27    /* 27: reserved */
	.long   knl_inthdr_entry28    /* 28: IMIA1 */
	.long   knl_inthdr_entry29    /* 29: IMIB1 */
	.long   knl_inthdr_entry30    /* 30: OVI1 */
	.long   knl_inthdr_entry31    /* 31: reserved */
	.long   knl_inthdr_entry32    /* 32: IMIA2 */
	.long   knl_inthdr_entry33    /* 33: IMIB2 */
	.long   knl_inthdr_entry34    /* 34: OVI2 */
	.long   knl_inthdr_entry35    /* 35: reserved */
	.long   knl_inthdr_entry36    /* 36: CMIA0 */
	.long   knl_inthdr_entry37    /* 37: CMIB0 */
	.long   knl_inthdr_entry38    /* 38: CMIA1/CMIB1 */
	.long   knl_inthdr_entry39    /* 39: TOVI0/TOVI1 */
	.long   knl_inthdr_entry40    /* 40: CMIA2 */
	.long   knl_inthdr_entry41    /* 41: CMIB2 */
	.long   knl_inthdr_entry42    /* 42: CMIA3/CMIB3 */
	.long   knl_inthdr_entry43    /* 43: TOVI2/TOVI3 */
	.long   knl_inthdr_entry44    /* 44: DEND0A */
	.long   knl_inthdr_entry45    /* 45: DEND0B */
	.long   knl_inthdr_entry46    /* 46: DEND1A */
	.long   knl_inthdr_entry47    /* 47: DEND1B */
	.long   knl_inthdr_entry48    /* 48: reserved */
	.long   knl_inthdr_entry49    /* 49: reserved */
	.long   knl_inthdr_entry50    /* 50: reserved */
	.long   knl_inthdr_entry51    /* 51: reserved */
	.long   knl_inthdr_entry52    /* 52: ERI0 */
	.long   knl_inthdr_entry53    /* 53: RXI0 */
	.long   knl_inthdr_entry54    /* 54: TXI0 */
	.long   knl_inthdr_entry55    /* 55: TEI0 */
	.long   knl_inthdr_entry56    /* 56: ERI1 */
	.long   knl_inthdr_entry57    /* 57: RXI1 */
	.long   knl_inthdr_entry58    /* 58: TXI1 */
	.long   knl_inthdr_entry59    /* 59: TEI1 */
	.long   knl_inthdr_entry60    /* 60: ERI2 */
	.long   knl_inthdr_entry61    /* 61: RXI2 */
	.long   knl_inthdr_entry62    /* 62: TXI2 */
	.long   knl_inthdr_entry63    /* 63: TEI2 */

#include "inthdr_startup.S"

#endif /* !USE_FULL_VECTOR */
