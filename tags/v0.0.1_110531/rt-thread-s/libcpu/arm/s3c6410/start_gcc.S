/*
 * File      : start.S
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-13     Bernard      first version
 * 2006-10-05     Alsor.Z      for s3c2440 initialize
 * 2008-01-29     Yi.Qiu       for QEMU emulator
 * 2011-05-11     swkyer       for tiny6410
 */
#include "s3c6410.h"


    .section .init, "ax"
    .code 32

    .globl _start
_start:
	b		reset
	ldr		pc, _vector_undef
	ldr		pc, _vector_swi
	ldr		pc, _vector_pabt
	ldr		pc, _vector_dabt
	ldr		pc, _vector_resv
	ldr		pc, _vector_irq
	ldr		pc, _vector_fiq

_vector_undef:	.word vector_undef
_vector_swi:	.word vector_swi
_vector_pabt:	.word vector_pabt
_vector_dabt:	.word vector_dabt
_vector_resv:	.word vector_resv
_vector_irq:	.word vector_irq
_vector_fiq:	.word vector_fiq

.balignl 	16,0xdeadbeef


/* IRQ stack memory (calculated at run-time) */
    .globl IRQ_STACK_START
IRQ_STACK_START:
	.word _irq_stack_start + CONFIG_STACKSIZE*2
	
    .globl FIQ_STACK_START
FIQ_STACK_START:
	.word _fiq_stack_start + CONFIG_STACKSIZE*2
	
    .globl UNDEFINED_STACK_START
UNDEFINED_STACK_START:
	.word _undefined_stack_start + CONFIG_STACKSIZE
	
    .globl ABORT_STACK_START
ABORT_STACK_START:
	.word _abort_stack_start + CONFIG_STACKSIZE
	
    .globl _STACK_START
_STACK_START:
	.word _svc_stack_start + CONFIG_STACKSIZE*8

/* ----------------------------------entry------------------------------*/
reset:
	/* set the cpu to SVC32 mode 	*/
	mrs    r0, cpsr
	bic    r0, r0, #MODEMASK
	orr    r0, r0, #SVCMODE|NOINT
	msr    cpsr, r0
	
	/* disable vector interrupt */
    mrc    p15, 0, r0, c1, c0, 0
    bic    r0, r0, #(1<<24)
    mcr    p15, 0, r0, c1, c0, 0

	/* sp is now in TCM1 */

	/* setup stack */
	bl     stack_setup

    /* copy exception handle table to ZERO */
    mov    r0, #0
    ldr    r1, =_start
    mov    r2, #0x40
irqvec_cpy_next:
    ldr    r3, [r1], #4
    str    r3, [r0], #4
    subs   r2, r2, #4
    bne    irqvec_cpy_next

	/* clear .bss */
	mov    r0, #0                   /* get a zero */
	ldr    r1, =__bss_start         /* bss start */
	ldr    r2, =__bss_end           /* bss end */
bss_loop:
	cmp    r1, r2                   /* check if data to clear */
	strlo  r0, [r1], #4             /* clear 4 bytes */
	blo    bss_loop                 /* loop until done */

	/* call C++ constructors of global objects */
	ldr    r0, =__ctors_start__
	ldr    r1, =__ctors_end__
ctor_loop:
	cmp    r0, r1
	beq    ctor_end
	ldr    r2, [r0], #4
	stmfd  sp!, {r0-r1}
	mov    lr, pc
	bx     r2
	ldmfd  sp!, {r0-r1}
	b      ctor_loop
ctor_end:

	/* start RT-Thread Kernel */
	ldr		pc, _rtthread_startup

_rtthread_startup:
	.word rtthread_startup


/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */

    /* exception handlers */
	.align  5
vector_undef:
	sub    sp, sp, #S_FRAME_SIZE
	stmia  sp, {r0 - r12}          /* Calling r0-r12 */
	add    r8, sp, #S_PC
	stmdb  r8, {sp, lr}^           /* Calling SP, LR */
	str    lr, [r8, #0]            /* Save calling PC */
	mrs    r6, spsr
	str    r6, [r8, #4]            /* Save CPSR	*/
	str    r0, [r8, #8]            /* Save OLD_R0 */
	mov    r0, sp

	bl     rt_hw_trap_udef

	.align	5
vector_swi:
	bl     rt_hw_trap_swi

	.align	5
vector_pabt:
	bl     rt_hw_trap_pabt

	.align	5
vector_dabt:
	sub    sp, sp, #S_FRAME_SIZE
	stmia  sp, {r0 - r12}          /* Calling r0-r12 */
	add    r8, sp, #S_PC
	stmdb  r8, {sp, lr}^           /* Calling SP, LR */
	str    lr, [r8, #0]            /* Save calling PC */
	mrs    r6, spsr
	str    r6, [r8, #4]            /* Save CPSR */
	str    r0, [r8, #8]            /* Save OLD_R0 */
	mov    r0, sp

	bl     rt_hw_trap_dabt

	.align	5
vector_resv:
	bl     rt_hw_trap_resv


    .globl  rt_interrupt_enter
    .globl  rt_interrupt_leave
    .globl  rt_thread_switch_interrput_flag
    .globl  rt_interrupt_from_thread
    .globl  rt_interrupt_to_thread
vector_irq:
	stmfd  sp!, {r0-r12, lr}
	bl     rt_interrupt_enter
	bl     rt_hw_trap_irq
	bl     rt_interrupt_leave

	/* if rt_thread_switch_interrput_flag set, jump to _interrupt_thread_switch and don't return */
	ldr    r0, =rt_thread_switch_interrput_flag
	ldr    r1, [r0]
	cmp    r1, #1
	beq    _interrupt_thread_switch

	ldmfd  sp!, {r0-r12, lr}
	subs   pc, lr, #4

	.align	5
vector_fiq:
	stmfd  sp!, {r0-r7, lr}
	bl     rt_hw_trap_fiq
	ldmfd  sp!, {r0-r7, lr}
	subs   pc, lr, #4

_interrupt_thread_switch:
	mov    r1, #0                  /* clear rt_thread_switch_interrput_flag */
	str    r1, [r0]

	ldmfd  sp!, {r0-r12, lr}       /* reload saved registers */
	stmfd  sp!, {r0-r3}            /* save r0-r3 */
	mov    r1, sp
	add    sp, sp, #16             /* restore sp */
	sub    r2, lr, #4              /* save old task's pc to r2 */
	
	mrs    r3, spsr                /* disable interrupt */
	orr    r0, r3, #NOINT
	msr    spsr_c, r0

	ldr    r0, =.+8                /* switch to interrupted task's stack */
	movs   pc, r0

	stmfd  sp!, {r2}               /* push old task's pc */
	stmfd  sp!, {r4-r12,lr}        /* push old task's lr,r12-r4 */
	mov    r4, r1                  /* Special optimised code below */
	mov    r5, r3
	ldmfd  r4!, {r0-r3}
	stmfd  sp!, {r0-r3}            /* push old task's r3-r0 */
	stmfd  sp!, {r5}               /* push old task's psr */
	mrs    r4, spsr
	stmfd  sp!, {r4}               /* push old task's spsr */

	ldr    r4, =rt_interrupt_from_thread
	ldr    r5, [r4]
	str    sp, [r5]                /* store sp in preempted tasks's TCB */

	ldr	   r6, =rt_interrupt_to_thread
	ldr	   r6, [r6]
	ldr	   sp, [r6]                /* get new task's stack pointer */

	ldmfd  sp!, {r4}               /* pop new task's spsr */
	msr    SPSR_cxsf, r4
	ldmfd  sp!, {r4}               /* pop new task's psr */
	msr    CPSR_cxsf, r4

	ldmfd  sp!, {r0-r12, lr, pc}   /* pop new task's r0-r12,lr & pc	*/


stack_setup:
	mrs    r0, cpsr
	bic    r0, r0, #MODEMASK
	orr    r1, r0, #UNDEFMODE|NOINT
	msr    cpsr_cxsf, r1			/* undef mode */
	ldr    sp, UNDEFINED_STACK_START

	orr    r1, r0, #ABORTMODE|NOINT
	msr    cpsr_cxsf, r1			/* abort mode */
	ldr    sp, ABORT_STACK_START

	orr    r1, r0, #IRQMODE|NOINT
	msr    cpsr_cxsf, r1			/* IRQ mode */
	ldr    sp, IRQ_STACK_START

	orr    r1, r0, #FIQMODE|NOINT
	msr    cpsr_cxsf, r1			/* FIQ mode */
	ldr    sp, FIQ_STACK_START

	bic    r0, r0, #MODEMASK
	orr    r1, r0, #SVCMODE|NOINT
	msr    cpsr_cxsf, r1			/* SVC mode */

	ldr    sp, _STACK_START

	/* USER mode is not initialized. */
	mov    pc, lr					/* The LR register may be not valid for the mode changes.*/


/*/*}*/

