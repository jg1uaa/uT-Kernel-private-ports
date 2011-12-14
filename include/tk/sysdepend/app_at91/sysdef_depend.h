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
 *	@(#)sysdef_depend.h (tk/AT91)
 *
 *	Definition about AT91M55800A
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * Program status register (PSR)
 */
#define PSR_N		0x80000000	/* Condition flag Negative */
#define PSR_Z		0x40000000	/* Zero */
#define PSR_C		0x20000000	/* Carry */
#define PSR_V		0x10000000	/* Overflow */

#define PSR_I		0x00000080	/* Interrupt (IRQ) disable */
#define PSR_F		0x00000040	/* Fast Interrupt (FIQ) disable */
#define PSR_T		0x00000020	/* Thumb mode */

#define PSR_DI		(PSR_I | PSR_F)	/* All Interrupts disable */

#define PSR_M(n)	( n )		/* Processor mode 0-31 */
#define PSR_USR		PSR_M(16)	/* User mode */
#define PSR_FIQ		PSR_M(17)	/* Fast Interrupt (FIQ) mode */
#define PSR_IRQ		PSR_M(18)	/* Interrupt (IRQ) mode */
#define PSR_SVC		PSR_M(19)	/* Supervisor mode */
#define PSR_ABT		PSR_M(23)	/* Abort mode */
#define PSR_UND		PSR_M(27)	/* Undefined order mode */
#define PSR_SYS		PSR_M(31)	/* System mode */

/* ------------------------------------------------------------------------ */

/*
 * Software interrupt number for micro T-Kernel 
 */
#define SWI_SVC		36	/* micro T-Kernel system call/extension SVC */
#define SWI_RETINT	37	/* tk_ret_int() system call */
#define SWI_DISPATCH	38	/* Task dispatcher */
#define SWI_DEBUG	39	/* Debugger support function */

/* ------------------------------------------------------------------------ */

/*
 * Special Function Registers
 */
#define SF_BASE   0xfff00000
#define SF_CIDR   0x00
#define SF_EXID   0x04
#define SF_RSR    0x08
/* reserved       0x0c */
/* reserved       0x10 */
/* reserved       0x14 */
#define SF_PMR    0x18
#define PMRKEY    0x27a80000

/*
 * External Bus Interface
 */
#define EBI_BASE  0xffe00000
#define EBI_RCR   0x20
#define EBI_MCR   0x24

/*
 * Advanced Power Management Controller
 */
#define APMC_BASE 0xffff4000
#define APMC_SCER 0x00
#define APMC_SCDR 0x04
#define APMC_SCSR 0x08
/* reserved       0x0c */
#define APMC_PCER 0x10
#define APMC_PCDR 0x14
#define APMC_PCSR 0x18
/* reserved       0x1c */
#define APMC_CGMR 0x20
/* reserved       0x24 */
#define APMC_PCR  0x28
#define APMC_PMR  0x2c
#define APMC_SR   0x30
#define APMC_IER  0x34
#define APMC_IDR  0x38
#define APMC_IMR  0x3c

#define APMC_MOSC_BYP   0x01
#define APMC_MOSC_EN    0x02
#define APMC_MKCO_DIS   0x04
#define APMC_PRES_NONE  (0x00 << 4)
#define APMC_PRES_DIV2  (0x01 << 4)
#define APMC_PRES_DIV4  (0x02 << 4)
#define APMC_PRES_DIV8  (0x03 << 4)
#define APMC_PRES_DIV16 (0x04 << 4)
#define APMC_PRES_DIV32 (0x05 << 4)
#define APMC_PRES_DIV64 (0x06 << 4)

#define APMC_CSS_LF     0x00
#define APMC_CSS_MOSC   0x01
#define APMC_CSS_PLL    0x02

#define APMC_SHDALC     0x01
#define APMC_WKACKC     0x02

#define APMC_SHDALS_OUT_TRIS    0x00
#define APMC_SHDALS_OUT_LEVEL0  0x01
#define APMC_SHDALS_OUT_LEVEL1  0x02
#define APMC_WKACKS_OUT_TRIS    (0x00 << 2)
#define APMC_WKACKS_OUT_LEVEL0  (0x01 << 2)
#define APMC_WKACKS_OUT_LEVEL1  (0x02 << 2)
#define APMC_WKEN               (0x01 << 4)
#define APMC_ALSHEN             (0x01 << 5)
#define APMC_WKEDG_NONE         (0x00 << 6)
#define APMC_WKEDG_POS_EDG      (0x01 << 6)
#define APMC_WKEDG_NEG_EDG      (0x02 << 6)
#define APMC_WKEDG_BOTH_EDG     (0x03 << 6)

#define APMC_MOSCS      0x01
#define APMC_PLL_LOCK   0x02

/*
 *  Parallel IO
 */
#define PIOA_BASE 0xfffec000
#define PIOB_BASE 0xffff0000
#define PIO_PER   0x00
#define PIO_PDR   0x04
#define PIO_PSR   0x08
/* reserved       0x0c */
#define PIO_OER   0x10
#define PIO_ODR   0x14
#define PIO_OSR   0x18
/* reserved       0x1c */
#define PIO_IFER  0x20
#define PIO_IFDR  0x24
#define PIO_IFSR  0x28
/* reserved       0x2c */
#define PIO_SODR  0x30
#define PIO_CODR  0x34
#define PIO_ODSR  0x38
#define PIO_PDSR  0x3c
#define PIO_IER   0x40
#define PIO_IDR   0x44
#define PIO_IMR   0x48
#define PIO_ISR   0x4c
#define PIO_MDER  0x50
#define PIO_MDDR  0x54
#define PIO_MDSR  0x58
/* reserved       0x5c */

/*
 * Advanced Interrupt Controller
 */
#define FIQ        0
#define SWIRQ      1
#define US0IRQ     2
#define US1IRQ     3
#define US2IRQ     4
#define SPIRQ      5
#define TC0IRQ     6
#define TC1IRQ     7
#define TC2IRQ     8
#define TC3IRQ     9
#define TC4IRQ    10
#define TC5IRQ    11
#define WDIRQ     12
#define PIOAIRQ   13
#define PIOBIRQ   14
#define AD0IRQ    15
#define AD1IRQ    16
#define DA0IRQ    17
#define DA1IRQ    18
#define RTCIRQ    19
#define APMCIRQ   20
#define SCLKIRQ   23
#define IRQ5      24
#define IRQ4      25
#define IRQ3      26
#define IRQ2      27
#define IRQ1      28
#define IRQ0      29
#define COMMRX    30
#define COMMTX    31

#define AIC_BASE  0xfffff000
#define AIC_SMR0  0x0000
#define AIC_SMR1  0x0004
#define AIC_SMR2  0x0008
#define AIC_SMR3  0x000c
#define AIC_SMR4  0x0010
#define AIC_SMR5  0x0014
#define AIC_SMR6  0x0018
#define AIC_SMR7  0x001c
#define AIC_SMR8  0x0020
#define AIC_SMR9  0x0024
#define AIC_SMR10 0x0028
#define AIC_SMR11 0x002c
#define AIC_SMR12 0x0030
#define AIC_SMR13 0x0034
#define AIC_SMR14 0x0038
#define AIC_SMR15 0x003c
#define AIC_SMR16 0x0040
#define AIC_SMR17 0x0044
#define AIC_SMR18 0x0048
#define AIC_SMR19 0x004c
#define AIC_SMR20 0x0050
#define AIC_SMR21 0x0054
#define AIC_SMR22 0x0058
#define AIC_SMR23 0x005c
#define AIC_SMR24 0x0060
#define AIC_SMR25 0x0064
#define AIC_SMR26 0x0068
#define AIC_SMR27 0x006c
#define AIC_SMR28 0x0070
#define AIC_SMR29 0x0074
#define AIC_SMR30 0x0078
#define AIC_SMR31 0x007c
#define AIC_SVR0  0x0080
#define AIC_SVR1  0x0084
#define AIC_SVR2  0x0088
#define AIC_SVR3  0x008c
#define AIC_SVR4  0x0090
#define AIC_SVR5  0x0094
#define AIC_SVR6  0x0098
#define AIC_SVR7  0x009c
#define AIC_SVR8  0x00a0
#define AIC_SVR9  0x00a4
#define AIC_SVR10 0x00a8
#define AIC_SVR11 0x00ac
#define AIC_SVR12 0x00b0
#define AIC_SVR13 0x00b4
#define AIC_SVR14 0x00b8
#define AIC_SVR15 0x00bc
#define AIC_SVR16 0x00c0
#define AIC_SVR17 0x00c4
#define AIC_SVR18 0x00c8
#define AIC_SVR19 0x00cc
#define AIC_SVR20 0x00d0
#define AIC_SVR21 0x00d4
#define AIC_SVR22 0x00d8
#define AIC_SVR23 0x00dc
#define AIC_SVR24 0x00e0
#define AIC_SVR25 0x00e4
#define AIC_SVR26 0x00e8
#define AIC_SVR27 0x00ec
#define AIC_SVR28 0x0070
#define AIC_SVR29 0x00f4
#define AIC_SVR30 0x00f8
#define AIC_SVR31 0x00fc
#define AIC_IVR   0x0100
#define AIC_FVR   0x0104
#define AIC_ISR   0x0108
#define AIC_IPR   0x010c
#define AIC_IMR   0x0110
#define AIC_CISR  0x0114
/* reserved       0x0118 */
/* reserved       0x011c */
#define AIC_IECR  0x0120
#define AIC_IDCR  0x0124
#define AIC_ICCR  0x0128
#define AIC_ISCR  0x012c
#define AIC_EOICR 0x0130
#define AIC_SPU   0x0134

/*
 * Timer Counter
 */
#define TCB0_BASE 0xfffd0000
#define TCB1_BASE 0xfffd4000
#define TC_C0     0x00
#define TC_C1     0x40
#define TC_C2     0x80
#define TC_CCR    0x00
#define TC_CMR    0x04
/* reserved       0x08 */
/* reserved       0x0c */
#define TC_CVR    0x10
#define TC_RA     0x14
#define TC_RB     0x18
#define TC_RC     0x1c
#define TC_SR     0x20
#define TC_IER    0x24
#define TC_IDR    0x28
#define TC_IMR    0x2c
#define TC_BCR    0xc0
#define TC_BMR    0xc4

#define TC_CLKEN  0x01
#define TC_CLKDIS 0x02
#define TC_SWTRG  0x04

#define TC_CLKS_MCK2    0x00
#define TC_CLKS_MCK8    0x01
#define TC_CLKS_MCK32   0x02
#define TC_CLKS_MCK128  0x03
#define TC_CLKS_MCK1024 0x04
#define TC_CLKS_XC0     0x05
#define TC_CLKS_XC1     0x06
#define TC_CLKS_XC2     0x07
#define TC_CLKI         0x08
#define TC_BURST_NONE   0x00
#define TC_BURST_XC0    0x10
#define TC_BURST_XC1    0x20
#define TC_BURST_XC2    0x30
#define TC_LDBSTOP      0x40
#define TC_LDBDIS       0x80
#define TC_ETRGEDG_EDGE_NONE    0x0000
#define TC_ETRGEDG_RIDING_EDGE  0x0100
#define TC_ETRGEDG_FALLING_EDGE 0x0200
#define TC_ETRGEDG_BOTH_EDGE    0x0300
#define TC_ABETRG_TIOB          0x0000
#define TC_ABETRG_TIOA          0x0400
#define TC_CPCTRG               0x4000
#define TC_WAVE                 0x8000
#define TC_LDRA_EDGE_NONE       0x0000
#define TC_LDRA_RISING_EDGE     0x010000
#define TC_LDRA_FALLING_EDGE    0x020000
#define TC_LDRA_BOTH_EDGE       0x030000
#define TC_LDRB_EDGE_NONE       0x000000
#define TC_LDRB_RISING_EDGE     0x040000
#define TC_LDRB_FALLING_EDGE    0x080000
#define TC_LDRB_BOTH_EDGE       0x0c0000
/*
 * USART
 */
#define USART0_BASE     0xfffc0000
#define USART1_BASE     0xfffc4000
#define USART2_BASE     0xfffc8000
#define US_CR           0x00
#define US_MR           0x04
#define US_IER          0x08
#define US_IDR          0x0c
#define US_IMR          0x10
#define US_CSR          0x14
#define US_RHR          0x18
#define US_THR          0x1c
#define US_BRGR         0x20
#define US_RTOR         0x24
#define US_TTGR         0x28
/* reserved             0x2c */
#define US_RPR          0x30
#define US_RCR          0x34
#define US_TPR          0x38
#define US_TCR          0x3c


#endif /* __TK_SYSDEF_DEPEND_H__ */
