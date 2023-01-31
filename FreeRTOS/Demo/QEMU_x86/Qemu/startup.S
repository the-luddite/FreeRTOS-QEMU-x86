/*
 * Copyright 2015 Wink Saville
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define	GDT_ENTRIES			8
#define	GDT_ENTRY_SIZE		8
#define	GDT_BYTES			(GDT_ENTRIES * GDT_ENTRY_SIZE)

/* Globals and externs */
	.global _start

	.extern __text_start
	.extern __text_end
	.extern __data_vma
	.extern __data_lma
	.extern __data_start
	.extern __data_end
	.extern __bss_start
	.extern __bss_end
	.extern __stack_for_main

	.global __text_start
	.global __text_end
	.global __data_vma
	.global __data_lma
	.global __data_start
	.global __data_end
	.global __bss_start
	.global __bss_end

	.extern setsegs
	.extern CRT_Init
	.extern kernel_load_check
	.extern main

  // [From](http://wiki.osdev.org/Bare_Bones)
  // Constants for multiboot header
  .set MAGIC, 0x1BADB002          // A standard magic value
  .set FLAGS, 0x3                 // Bit0 align on page boundry,
                                  // Bit1 provide memory map
  .set CHECKSUM, -(MAGIC + FLAGS) // Checksum

  // The multiboot header
  .section .multiboot
  .align 4
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

  /* Set-up GDT */
  .section .data

  .align	16
  .globl	gdt
  gdt:	.space	GDT_BYTES
  gdtr:	.word	(GDT_BYTES-1)		/* sizeof _gdt -1 (in bytes) */
  .long	gdt							/* global pointer to the gdt */

  // Start up code
  .section .text
_start:
;   movl  $stack_top, %esp  // Initialize stack
	/* Initialise the stack pointer */
	movl $__stack_for_main, %esp

    /* Reset EFLAGS. */
    pushl $0
    popf

 	/* Set up the global descriptor table */
	call	setsegs
	lgdt	gdtr
	ljmp	$0x8, $gdt1					/* CS descriptor 1 */
	gdt1:
	movl	$0x10, %eax					/* DS descriptor 2 */
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movl	$0x18, %eax					/* SS descriptor 3 */
	movw	%ax, %ss

    /* Clear interrupt flag */
	cli

    /* Initialise platform */
	call CRT_Init

    call  main             // Call main

  // If kmain returns we hang
  cli                     // Turn off interrupts
hang:
  hlt                     // Halt
  jmp hang                // Loop if we unhalt

  // Stack
  .section .bootstrap_stack
  .skip 16384

/*-------------------------------------------------------------------------
	                  GLOBAL ASSEMBLY LANGUAGE ROUTINES
--------------------------------------------------------------------------*/

 	/* halt */
 	.globl	halt
	halt:
	jmp halt
	ret

	/* inb */
	.globl	inb
	inb:	movl	4(%esp), %edx
	xorl	%eax, %eax	# clr eax
	inb	%dx, %al
	ret

	/* inw */
	.globl	inw
	inw:	movl	4(%esp), %edx
	xorl	%eax, %eax	# clr eax
	inw		%dx, %ax
	ret

	/* inl */
	.globl  inl
	inl:	movl	4(%esp), %edx
	xorl	%eax, %eax
	inl		%dx, %eax
	ret

	/* outw */
	.globl	outw
	outw:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outw	%ax, %dx
	ret

	/* outl */
	.globl	outl
	outl:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outl	%eax, %dx
	ret

stack_top:

