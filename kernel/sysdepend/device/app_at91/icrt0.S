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
 *  @(#) icrt0.S
 *
 *    Start up module
 */

#include "hwconfig.h"
#include "utk_config.h"

#include <machine.h>
#include <tk/asm.h>

#if USE_IMALLOC
	/* Low level memory manager information */
	.comm	Csym(knl_lowmem_top), 4	// Head of area (Low address)
	.comm	Csym(knl_lowmem_limit), 4	// End of area (High address)
#endif

/*
 * Vector Table
 */
        .section .vector,"ax"
        .code 32
        .align 0
        .global __reset
__reset:
        b       start              /* reset */
        .global undef_vector
undef_vector:
        b       undef_vector       /* undefined operation */
        .global swi_vector
swi_vector:
        b       swi_handler        /* software interrupt */
        .global prefetch_vector
prefetch_vector:
        b       prefetch_vector     /* prefetch abort */
        .global data_abort_vector
data_abort_vector:
        b       data_abort_vector  /* data abort */
        .global reserved_vector
reserved_vector:
        b       reserved_vector    /* reserved */
        .global irq_vector
irq_vector:
        ldr pc, [pc, #-0xf20]      /* IRQ: AIC_IVR */
        .global fiq_vector
fiq_vector:
        ldr pc, [pc, #-0xf20]      /* FIQ: AIC_FVR */

/*
 * Start up routine
 */
        .section .ftext.1,"ax"
        .code 32
        .align     0
        .global    start
start:
	msr     cpsr, #(PSR_SVC|PSR_DI)

	.section .romtext.1,"ax"
        .code 32
        .align     0
flashrom_init:
	/* r0 := EBI_BASE(0xffe00000) */
	mov     r0, #(0xff << 24)
	orr     r0, r0, #(0xe0 << 16)
	/* r1 := EBI_CSR0(0x100020b1) */
	mov	r1, #(0x10 << 24)
	orr	r1, r1, #(0x20 << 8)
	orr	r1, r1, #(0xb1)
	str     r1, [r0, #0]

crystal_init:
	add     r9, pc, #(crystal_immediates - . - 8)
	ldmia	r9, {r0-r2}
	/*
		r0 = ENABLE_16MHz_CRYSTAL,
		r1 = APMC_BASE | APMC_CGMR,
		r2 = APMC_BASE | APMC_SR
	*/

	/* enable 16MHz crystal */
	str     r0, [r1]
wait_stabilized:
	ldr     r3, [r2]
	tst     r3, #APMC_MOSCS
	beq     wait_stabilized         /* if APMC_SR & APMC_SR_MOSCS == 0 */
	/* use 16MHz */
	orr     r0, r0, #(0x01 << 14)   /* Main Oscillator */
	str     r0, [r1]

	b       setup_ram_vectors

crystal_immediates:
	.long	ENABLE_16MHz_CRYSTAL
	.long	APMC_BASE | APMC_CGMR
	.long	APMC_BASE | APMC_SR

ram_vector_table:
        ldr pc, [pc, #0x18]        /* reset */
        ldr pc, [pc, #0x18]        /* undefined operation */
        ldr pc, [pc, #0x18]        /* software interrupt */
        ldr pc, [pc, #0x18]        /* prefetch abort */
        ldr pc, [pc, #0x18]        /* data abort */
        nop                        /* reserved */
        ldr pc, [pc, #-0xf20]      /* IRQ: AIC_IVR */
        ldr pc, [pc, #-0xf20]      /* FIQ: AIC_FVR */

ram_vector_address_table:
        .long   start
        .long   undef_vector
        .long   swi_vector
        .long   prefetch_vector
        .long   data_abort_vector

setup_ram_vectors:
	mov     r8, #INTERNAL_RAM_START_BEFORE_REMAP /* dst*/
	sub     r9, pc, #(8 + . - ram_vector_table)  /* src */
	ldmia   r9!, {r0-r7} /* read vector */
	stmia   r8!, {r0-r7} /* write vector */
	ldmia   r9!, {r0-r4} /* read jump table */
	stmia   r8!, {r0-r4} /* write jump table */


/*
 * chip select and remap
 */
	ldr     r12, =after_remap_start
	/* r1 = EBI_BASE(0xffe00000) */
	mov	r1, #(0xff << 24)
	orr	r1, r1, #(0xe0 << 16)
	/* Flash ROM 0x10000000- */
	mov     r2, #0x10000000
	orr	r2, r2, #(0x20 << 8)
	orr	r2, r2, #(0xb1)
	str     r2, [r1, #0x00]
	/* SRAM 0x20000000- */
	mov     r2, #0x20000000
	orr	r2, r2, #(0x30 << 8)
	orr	r2, r2, #(0xa9)
	str     r2, [r1, #0x04]
	/* Ethernet 0x40000000- */
	mov     r2, #0x40000000
	orr	r2, r2, #(0x20 << 8)
	orr	r2, r2, #(0x35)
	str     r2, [r1, #0x1c]
	/* remap */
	mov     r2, #1
	str     r2, [r1, #EBI_RCR]

	mov  pc, r12

/* -------- From here, address space after remap --------------------- */

after_remap_start:


        .section .ftext.2,"ax"
        .code 32
        .align     0

#if USE_PROTECT_MODE
	ldr     r1, =SF_BASE
	ldr     r2, =PMRKEY
	orr     r2, r2, #(1 << 5)
	str     r2, [r1, #SF_PMR]
#endif

init_stacks:
        ldr  r1, =EXCEPTION_STACK_TOP
#if ABT_STACK_SIZE != 0
	msr  cpsr, #(PSR_ABT|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #ABT_STACK_SIZE
#endif
#if UND_STACK_SIZE != 0
	msr  cpsr, #(PSR_UND|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #UND_STACK_SIZE
#endif
#if USR_STACK_SIZE != 0
	msr  cpsr, #(PSR_USR|PSR_DI)
	mov  sp, =APPLICATION_STACK_TOP
#endif
#if FIQ_STACK_SIZE != 0
	msr  cpsr, #(PSR_FIQ|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #FIQ_STACK_SIZE
#endif
	msr  cpsr, #(PSR_IRQ|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #IRQ_STACK_SIZE

	msr  cpsr, #(PSR_SVC|PSR_DI)
	mov  sp, r1

#if USE_TMONITOR
	ldr     r0, =tm_init
	mov     lr, pc
	bx      r0
#endif

	.section .romtext.2,"ax"
	.code 32
	.align     0

	/* .data */
	ldr     r8, =__data_org   /* src address */
	ldr     r9, =__data_start /* dst address */
	ldr     r10, =__data_end
	subs    r10, r10, r9       /* r10 := data_size */
	beq     data_done          /* if __data_start == __data_end */

data_loop:
	ldmia   r8!, {r0}
	stmia   r9!, {r0}
	subs    r10, r10, #4
	bne     data_loop          /* if data_size != 0 */

data_done:

	.section .ftext.3,"ax"
	.code 32
	.align     0

#if USE_NOINIT
	ldr     r9, =__noinit_end   /* dst address */
#else
	/* .bss */
	ldr     r9, =__bss_start   /* dst address */
#endif
	ldr     r10, =__bss_end
	subs    r10, r10, r9       /* r10 := data_size */
	beq     bss_done           /* if __bss_start == __bss_end */
	mov     r0, #0

bss_loop:
	stmia   r9!, {r0}
	subs    r10, r10, #4
	bne     bss_loop           /* if data_size != 0 */

bss_done:

#if USE_IMALLOC
	ldr     r5, =SYSTEMAREA_TOP
	cmp	r9, r5			// _end or RAM_TOP
	movhi	r5, r9			// Either of High addresses
	ldr	ip, =knl_lowmem_top
	str	r5, [ip]		// knl_lowmem_top = _end or RAM_TOP
	ldr     r5, =SYSTEMAREA_END
	ldr	ip, =knl_lowmem_limit
	str	r5, [ip]		// knl_lowmem_limit = RAM_END
#endif

kernel_start:
	ldr	ip, =Csym(main)
	mov	r0, #0
	mov	r14, pc
	bx	ip
l_end:
	b     l_end

#if USE_TMONITOR
tm_init:
	/* TC0, TC1, TC2 clock enable */
	ldr    r1, =APMC_BASE
	mov    r2, #(7 << 6)
	str    r2, [r1, #APMC_PCER]

	/* initialize serial I/O */
	ldr     r0, =sio_init
	bx      r0
	/* return directly to the place tm_init called from sio_init */
#endif

swi_handler:
	str    lr, [sp, #-4]!
	str    ip, [sp, #-4]!
	mrs    ip, spsr
	str    ip, [sp, #-4]!

	ldr    ip, [lr, #-4]   /* load SWI No. */
	bic    ip, ip, #(0xff << 24)

	ldr    lr, =Csym(knl_intvec) /* exception vector table */
	add    ip, lr, ip, LSL #2 /* lr := lr + ip*4 = vecaddr */
	ldr    lr, [ip]
	bx     lr

	.global knl_irq_handler
knl_irq_handler:
	sub    lr, lr, #4
	stmfd  sp!, {lr}  /* sp-> lr_xxx */

#if USE_PROTECT_MODE
        ldr    lr, =AIC_BASE
        str    lr, [lr, #AIC_IVR]
#else
        ldr    lr, =AIC_BASE
        ldr    lr, [lr, #AIC_IVR]
#endif

	stmfd  sp!, {ip} /* sp-> ip, lr_xxx */
	mrs    ip, spsr
	stmfd  sp!, {ip} /* sp-> spsr_xxx, ip, lr_xxx */
	stmfd  sp!, {r3} /* sp-> r3, spsr_xxx, ip, lr_xxx */

	ldr    lr, =(AIC_BASE | AIC_ISR)
	ldr    lr, [lr] /* lr := IRQ No. */
	ldr    ip, =Csym(knl_intvec) /* exception vector table */
	add    ip, ip, lr, LSL #2 /* ip := &vector[IRQ No.] */
	ldr    r3, [ip] /* r3 := vector[IRQ No.] */
	mov    lr, pc
	bx     r3


