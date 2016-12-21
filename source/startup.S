/*
 * Startup routine for Beaglebone Black - ARM Cortex-A8 
 *
 * Author: Mani Sadhasivam
 *
 */

#include "startup.h"

	.globl _start
	.globl reset
	.globl low_level_init
        .globl undefined_instruction
        .globl software_interrupt
        .globl prefetch_abort
        .globl data_abort
        .globl not_used
        .globl irq
        .globl fiq

/* Place the Exception vectors in 'vectors' section. Placement will be done by linker */

	.section ".vectors", "ax"

_start:
		b 	reset
        	b     	undefined_instruction
        	b       software_interrupt
        	b       prefetch_abort
        	b       data_abort
        	b       not_used
        	b       irq
        	b       fiq
			
reset: 
		mrs	r0, cpsr		@ Read CPSR register
		orr	r0, r0, #0xd3		@ Set Supervisor mode and disable IRQ/FI for now 
		msr	cpsr, r0		@ Write to CPSR
		
		/* Configure lower address range for Exception vectors and write to VBAR */

		mrc	p15, 0, r0, c1, c0, 0	@ Read CP15-Control register
		bic	r0, r0, #0x00002000	@ Clear V bit to mask Exception vectors to lower address (VBAR)
		mcr	p15, 0, r0, c1, c0, 0	@ Write to CP15-Control register
		ldr	r0, =_start
		mcr	p15, 0, r0, c12, c0, 0	@ Write the Exception vector base address to VBAR

		bl	init_cp15

		/* Setup Stack pointer */
		
		ldr	sp, =SRAM_END
		bic	sp, sp, #7		@ Align stack pointer on 8 byte boundary ((421) = (000))

		/* Call setup code */

		bl	_setup

/* Configure CP15 in the following mode:
 *
 * 1. Invalidate Cache, TLB
 * 2. Disable MMU/D cache and enable I cache
 *
 */

init_cp15:
		/* Invalidate Cache, TLB*/
		
		mov 	r0, #0		
		mcr 	p15, 0, r0, c8, c7, 0	@ Invalidate I/D TLBs
		mcr 	p15, 0, r0, c7, c5, 0	@ Invalidate all I caches
		mcr	p15, 0, r0, c7, c10, 4	@ Data Sync Barrier operation

		/* Disable MMU/D cache and enable I cache */

		mrc	p15, 0, r0, c1, c0, 0	@ Read control register config
		bic	r0, r0, #0x00000005	@ Disable MMU and Data cache
		orr	r0, r0, #0x00001000	@ Enable I cache
        	orr     r0, r0, #0x00000800     @ Enable program flow prediction
		mcr	p15, 0, r0, c1, c0, 0	@ Write back to Control register

		mov pc, lr			@ Back to the caller
