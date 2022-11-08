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
  .global _start
_start:
  movl  $stack_top, %esp  // Initialize stack
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

	/* outb */
	.globl	outb
	outb:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outb	%al, %dx
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

