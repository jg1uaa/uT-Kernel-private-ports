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

        .section .text
        .align 2

        .global __reset

#if !USE_FULL_VECTOR
__reset:
	.long   Csym(start)         /* 0: reset */
__manual_reset:
	.long   int_ill             /* 1: manual reset */
reserved:
	.long   int_ill             /* 2: reserved */
	.long   int_ill             /* 3: reserved */
	.long   int_ill             /* 4: reserved */
trace:
	.long   int_ill             /* 5: trace */
direct_transition:
	.long   int_ill             /* 6: direct transition */
NMI:
	.long	int_ill             /* 7: NMI */
trap:
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
#if USE_TRAP & USE_DBGSPT
	.long   Csym(knl_call_dbgspt)    /* 11: trap (#3) */
#else
	.long	int_ill                  /* 11: trap (#3) */
#endif
reserved2:
	.long   int_ill             /* 12: reserved */
	.long   int_ill             /* 13: reserved */
	.long   int_ill             /* 14: reserved */
	.long   int_ill             /* 15: reserved */
irq:
	.long   int_ill             /* 16: IRQ0 */
	.long   int_ill             /* 17: IRQ1 */
	.long   int_ill             /* 18: IRQ2 */
	.long   int_ill             /* 19: IRQ3 */
	.long   int_ill             /* 20: IRQ4 */
	.long   int_ill             /* 21: IRQ5(RTC) */
	.long   int_ill             /* 22: IRQ6(USB) */
	.long   int_ill             /* 23: IRQ7 */

internal_interrupt:
	.long   int_ill             /* 24:  */
	.long   int_ill             /* 25: WDT */
	.long   int_ill             /* 26:  */
	.long   int_ill             /* 27:  */
	.long   int_ill             /* 28: A/D */
	.long   int_ill             /* 29:  */
	.long   int_ill             /* 30:  */
	.long   int_ill             /* 31:  */
	.long   Csym(knl_timer_handler_startup) /* 32: TGI0A */
	.long   int_ill             /* 33:  */
	.long   int_ill             /* 34:  */
	.long   int_ill             /* 35:  */
	.long   int_ill             /* 36:  */
	.long   int_ill             /* 37:  */
	.long   int_ill             /* 38:  */
	.long   int_ill             /* 39:  */
	.long   int_ill             /* 40: TGI1A */
	.long   int_ill             /* 41:  */
	.long   int_ill             /* 42:  */
	.long   int_ill             /* 43:  */
	.long   int_ill             /* 44: TGI2A */
	.long   int_ill             /* 45:  */
	.long   int_ill             /* 46:  */
	.long   int_ill             /* 47:  */
	.long   int_ill             /* 48:  */
	.long   int_ill             /* 49:  */
	.long   int_ill             /* 50:  */
	.long   int_ill             /* 51:  */
	.long   int_ill             /* 52:  */
	.long   int_ill             /* 53:  */
	.long   int_ill             /* 54:  */
	.long   int_ill             /* 55:  */
	.long   int_ill             /* 56:  */
	.long   int_ill             /* 57:  */
	.long   int_ill             /* 58:  */
	.long   int_ill             /* 59:  */
	.long   int_ill             /* 60:  */
	.long   int_ill             /* 61:  */
	.long   int_ill             /* 62:  */
	.long   int_ill             /* 63:  */
	.long   int_ill             /* 64:  */
	.long   int_ill             /* 65:  */
	.long   int_ill             /* 66:  */
	.long   int_ill             /* 67:  */
	.long   int_ill             /* 68:  */
	.long   int_ill             /* 69:  */
	.long   int_ill             /* 70:  */
	.long   int_ill             /* 71:  */
	.long   int_ill             /* 72:  */
	.long   int_ill             /* 73:  */
	.long   int_ill             /* 74:  */
	.long   int_ill             /* 75:  */
	.long   int_ill             /* 76:  */
	.long   int_ill             /* 77:  */
	.long   int_ill             /* 78:  */
	.long   int_ill             /* 79:  */
	.long   int_ill             /* 80:  */
	.long   int_ill             /* 81:  */
	.long   int_ill             /* 82:  */
	.long   int_ill             /* 83:  */
	.long   int_ill             /* 84:  */
	.long   int_ill             /* 85:  */
	.long   int_ill             /* 86:  */
	.long   int_ill             /* 87:  */
	.long   int_ill             /* 88:  */
	.long   int_ill             /* 89:  */
	.long   int_ill             /* 90:  */
	.long   int_ill             /* 91:  */
	.long   int_ill             /* 92:  */
	.long   int_ill             /* 93:  */
	.long   int_ill             /* 94:  */
	.long   int_ill             /* 95:  */
	.long   int_ill             /* 96:  */
	.long   int_ill             /* 97:  */
	.long   int_ill             /* 98:  */
	.long   int_ill             /* 99:  */
	.long   int_ill             /* 100:  */
	.long   int_ill             /* 101:  */
	.long   int_ill             /* 102:  */
	.long   int_ill             /* 103:  */
	.long   int_ill             /* 104:  */
	.long   int_ill             /* 105:  */
	.long   int_ill             /* 106:  */
	.long   int_ill             /* 107:  */
	.long   int_ill             /* 108:  */
	.long   int_ill             /* 109:  */
	.long   int_ill             /* 110:  */
	.long   int_ill             /* 111:  */
	.long   int_ill             /* 112:  */
	.long   int_ill             /* 113:  */
	.long   int_ill             /* 114:  */
	.long   int_ill             /* 115:  */
	.long   int_ill             /* 116:  */
	.long   int_ill             /* 117:  */
	.long   int_ill             /* 118:  */
	.long   int_ill             /* 119:  */
	.long   int_ill             /* 120:  */
	.long   int_ill             /* 121:  */
	.long   int_ill             /* 122:  */
	.long   int_ill             /* 123:  */
	.long   int_ill             /* 124:  */
	.long   int_ill             /* 125:  */
	.long   int_ill             /* 126:  */
	.long   int_ill             /* 127:  */

#else /* USE_FULL_VECTOR */

__reset:
	.long   Csym(start)           /* 0: reset */
__manual_reset:
	.long   knl_inthdr_entry1     /* 1: manual reset */
reserved:
	.long   knl_inthdr_entry2     /* 2: reserved */
	.long   knl_inthdr_entry3     /* 3: reserved */
	.long   knl_inthdr_entry4     /* 4: reserved */
trace:
	.long   knl_inthdr_entry5     /* 5: trace */
direct_transition:
	.long   knl_inthdr_entry6     /* 6: direct transition */
NMI:
	.long	knl_inthdr_entry7     /* 7: NMI */
trap:
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
#if USE_TRAP & USE_DBGSPT
	.long   Csym(knl_call_dbgspt)    /* 11: trap (#3) */
#else
	.long	knl_inthdr_entry11       /* 11: trap (#3) */
#endif
reserved2:
	.long   knl_inthdr_entry12    /* 12: reserved */
	.long   knl_inthdr_entry13    /* 13: reserved */
	.long   knl_inthdr_entry14    /* 14: reserved */
	.long   knl_inthdr_entry15    /* 15: reserved */
irq:
	.long   knl_inthdr_entry16    /* 16: IRQ0 */
	.long   knl_inthdr_entry17    /* 17: IRQ1 */
	.long   knl_inthdr_entry18    /* 18: IRQ2 */
	.long   knl_inthdr_entry19    /* 19: IRQ3 */
	.long   knl_inthdr_entry20    /* 20: IRQ4 */
	.long   knl_inthdr_entry21    /* 21: IRQ5(RTC) */
	.long   knl_inthdr_entry22    /* 22: IRQ6(USB) */
	.long   knl_inthdr_entry23    /* 23: IRQ7 */

internal_interrupt:
	.long   knl_inthdr_entry24    /* 24:  */
	.long   knl_inthdr_entry25    /* 25: WDT */
	.long   knl_inthdr_entry26    /* 26:  */
	.long   knl_inthdr_entry27    /* 27:  */
	.long   knl_inthdr_entry28    /* 28: A/D */
	.long   knl_inthdr_entry29    /* 29:  */
	.long   knl_inthdr_entry30    /* 30:  */
	.long   knl_inthdr_entry31    /* 31:  */
	.long   Csym(knl_timer_handler_startup) /* 32: TGI0A */
	.long   knl_inthdr_entry33    /* 33:  */
	.long   knl_inthdr_entry34    /* 34:  */
	.long   knl_inthdr_entry35    /* 35:  */
	.long   knl_inthdr_entry36    /* 36:  */
	.long   knl_inthdr_entry37    /* 37:  */
	.long   knl_inthdr_entry38    /* 38:  */
	.long   knl_inthdr_entry39    /* 39:  */
	.long   knl_inthdr_entry40    /* 40: TGI1A */
	.long   knl_inthdr_entry41    /* 41:  */
	.long   knl_inthdr_entry42    /* 42:  */
	.long   knl_inthdr_entry43    /* 43:  */
	.long   knl_inthdr_entry44    /* 44: TGI2A */
	.long   knl_inthdr_entry45    /* 45:  */
	.long   knl_inthdr_entry46    /* 46:  */
	.long   knl_inthdr_entry47    /* 47:  */
	.long   knl_inthdr_entry48    /* 48:  */
	.long   knl_inthdr_entry49    /* 49:  */
	.long   knl_inthdr_entry50    /* 50:  */
	.long   knl_inthdr_entry51    /* 51:  */
	.long   knl_inthdr_entry52    /* 52:  */
	.long   knl_inthdr_entry53    /* 53:  */
	.long   knl_inthdr_entry54    /* 54:  */
	.long   knl_inthdr_entry55    /* 55:  */
	.long   knl_inthdr_entry56    /* 56:  */
	.long   knl_inthdr_entry57    /* 57:  */
	.long   knl_inthdr_entry58    /* 58:  */
	.long   knl_inthdr_entry59    /* 59:  */
	.long   knl_inthdr_entry60    /* 60:  */
	.long   knl_inthdr_entry61    /* 61:  */
	.long   knl_inthdr_entry62    /* 62:  */
	.long   knl_inthdr_entry63    /* 63:  */
	.long   knl_inthdr_entry64    /* 64:  */
	.long   knl_inthdr_entry65    /* 65:  */
	.long   knl_inthdr_entry66    /* 66:  */
	.long   knl_inthdr_entry67    /* 67:  */
	.long   knl_inthdr_entry68    /* 68:  */
	.long   knl_inthdr_entry69    /* 69:  */
	.long   knl_inthdr_entry70    /* 70:  */
	.long   knl_inthdr_entry71    /* 71:  */
	.long   knl_inthdr_entry72    /* 72:  */
	.long   knl_inthdr_entry73    /* 73:  */
	.long   knl_inthdr_entry74    /* 74:  */
	.long   knl_inthdr_entry75    /* 75:  */
	.long   knl_inthdr_entry76    /* 76:  */
	.long   knl_inthdr_entry77    /* 77:  */
	.long   knl_inthdr_entry78    /* 78:  */
	.long   knl_inthdr_entry79    /* 79:  */
	.long   knl_inthdr_entry80    /* 80:  */
	.long   knl_inthdr_entry81    /* 81:  */
	.long   knl_inthdr_entry82    /* 82:  */
	.long   knl_inthdr_entry83    /* 83:  */
	.long   knl_inthdr_entry84    /* 84:  */
	.long   knl_inthdr_entry85    /* 85:  */
	.long   knl_inthdr_entry86    /* 86:  */
	.long   knl_inthdr_entry87    /* 87:  */
	.long   knl_inthdr_entry88    /* 88:  */
	.long   knl_inthdr_entry89    /* 89:  */
	.long   knl_inthdr_entry90    /* 90:  */
	.long   knl_inthdr_entry91    /* 91:  */
	.long   knl_inthdr_entry92    /* 92:  */
	.long   knl_inthdr_entry93    /* 93:  */
	.long   knl_inthdr_entry94    /* 94:  */
	.long   knl_inthdr_entry95    /* 95:  */
	.long   knl_inthdr_entry96    /* 96:  */
	.long   knl_inthdr_entry97    /* 97:  */
	.long   knl_inthdr_entry98    /* 98:  */
	.long   knl_inthdr_entry99    /* 99:  */
	.long   knl_inthdr_entry100   /* 100:  */
	.long   knl_inthdr_entry101   /* 101:  */
	.long   knl_inthdr_entry102   /* 102:  */
	.long   knl_inthdr_entry103   /* 103:  */
	.long   knl_inthdr_entry104   /* 104:  */
	.long   knl_inthdr_entry105   /* 105:  */
	.long   knl_inthdr_entry106   /* 106:  */
	.long   knl_inthdr_entry107   /* 107:  */
	.long   knl_inthdr_entry108   /* 108:  */
	.long   knl_inthdr_entry109   /* 109:  */
	.long   knl_inthdr_entry110   /* 110:  */
	.long   knl_inthdr_entry111   /* 111:  */
	.long   knl_inthdr_entry112   /* 112:  */
	.long   knl_inthdr_entry113   /* 113:  */
	.long   knl_inthdr_entry114   /* 114:  */
	.long   knl_inthdr_entry115   /* 115:  */
	.long   knl_inthdr_entry116   /* 116:  */
	.long   knl_inthdr_entry117   /* 117:  */
	.long   knl_inthdr_entry118   /* 118:  */
	.long   knl_inthdr_entry119   /* 119:  */
	.long   knl_inthdr_entry120   /* 120:  */
	.long   knl_inthdr_entry121   /* 121:  */
	.long   knl_inthdr_entry122   /* 122:  */
	.long   knl_inthdr_entry123   /* 123:  */
	.long   knl_inthdr_entry124   /* 124:  */
	.long   knl_inthdr_entry125   /* 125:  */
	.long   knl_inthdr_entry126   /* 126:  */
	.long   knl_inthdr_entry127   /* 127:  */

#include "inthdr_startup.S"

#endif /* !USE_FULL_VECTOR */
