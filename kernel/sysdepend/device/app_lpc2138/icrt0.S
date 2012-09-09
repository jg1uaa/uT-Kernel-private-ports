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
        ldr     pc, =start         /* reset (long jump) */
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
        b       knl_irq_handler    /* IRQ */
        .global fiq_vector
fiq_vector:
        b       fiq_vector         /* FIQ: no support XXX */

        .pool

/*
 * Start up routine
 */
        .section .text
        .code 32
        .align     0
        .global    start
start:
	msr     cpsr, #(PSR_SVC|PSR_DI)

// board setup code ----------------------------------------------------------

	mov	r0, #0
	mov	r1, #1

memmap_init:	// remap vector area (boot loader -> user flash)
	ldr	r2, =MEMMAP
	strb	r1, [r2]

pll_init:	// setup PLL
	ldr	r4, =PLLCON
	mov	r5, #0x55
	mov	r6, #0xaa

	ldrh	r2, [r4, #0x08]		// PLLSTAT
	tst	r2, #(1 << 9)
					// PLL is connected: disconnect now
	strneb	r1, [r4, #0x00]		// PLLCON=1 (PLL enable/disconnect)
	strneb	r6, [r4, #0x0c]		//  PLLFEED=0xaa
	strneb	r5, [r4, #0x0c]		//  PLLFEED=0x55

	strb	r0, [r4, #0x00]		// PLLCON=0 (PLL disable/disconnect)
	strb	r6, [r4, #0x0c]		//  PLLFEED=0xaa
	strb	r5, [r4, #0x0c]		//  PLLFEED=0x55

	mov	r2, #(1 << 5)		// CCLK = Fcco / (2 x P)
					//  60MHz = 240MHz / (2 x 2)
	orr	r2, r2, #(4 - 1)	// CCLK = Fosc x M
					//  60MHz = 15MHz x 4
	strb	r2, [r4, #0x04]		// PLLCFG
	strb	r6, [r4, #0x0c]		//  PLLFEED=0xaa
	strb	r5, [r4, #0x0c]		//  PLLFEED=0x55

	strb	r1, [r4, #0x00]		// PLLCON=1 (PLL enable/disconnect)
	strb	r6, [r4, #0x0c]		//  PLLFEED=0xaa
	strb	r5, [r4, #0x0c]		//  PLLFEED=0x55

pll_wait:
	ldrh	r2, [r4, #0x08]		// PLLSTAT
	tst	r2, #(1 << 10)
	beq	pll_wait		// wait for lock

	mov	r3, #3
	strb	r3, [r4, #0x00]		// PLLCON=3 (PLL enable/connect)
	strb	r6, [r4, #0x0c]		//  PLLFEED=0xaa
	strb	r5, [r4, #0x0c]		//  PLLFEED=0x55

pclk_init:	// setup PCLK
	ldr	r2, =APBDIV		// PCLK=CCLK/4 (LPC21xx/LPC22xx)
	strb	r0, [r2, #0x00]		// APBDIV

// ---------------------------------------------------------------------------

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

	// UART setup is moved to devinit.c:knl_init_device() //

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

	stmfd  sp!, {ip} /* sp-> ip, lr_xxx */
	mrs    ip, spsr
	stmfd  sp!, {ip} /* sp-> spsr_xxx, ip, lr_xxx */
	stmfd  sp!, {r3} /* sp-> r3, spsr_xxx, ip, lr_xxx */

	ldr    ip, =VICIRQStatus
	ldr    ip, [ip]
	mov    lr, #0 /* lr: IRQ No. */

bsf_step16:
	ldr    r3,=0xffff
	tst    ip, r3
	addeq  lr, lr, #16
	moveq  ip, ip, lsr #16
bsf_step8:
	tst    ip, #0xff
	addeq  lr, lr, #8
	moveq  ip, ip, lsr #8
bsf_step4:
	tst    ip, #0x0f
	addeq  lr, lr, #4
	moveq  ip, ip, lsr #4
bsf_step2:
	tst    ip, #0x03
	addeq  lr, lr, #2
	moveq  ip, ip, lsr #2
bsf_step1:
	tst    ip, #0x01
	addeq  lr, lr, #1

	ldr    ip, =Csym(knl_intvec) /* exception vector table */
	add    ip, ip, lr, LSL #2 /* ip := &vector[IRQ No.] */
	ldr    r3, [ip] /* r3 := vector[IRQ No.] */
	mov    lr, pc
	bx     r3

	.pool
