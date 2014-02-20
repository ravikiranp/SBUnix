#
#   idt.s
#   
#

.text

.global _x86_asm_load_idt

_x86_asm_load_idt:

	cli
	
	# Decreasing the clock frequency to 1Hz
	movb $0x36, %al
	outb %al, $0x43
	movw $0xffff, %ax
	outb %al, $0x40
	shrw $8,%ax
	outb %al, $0x40	
	
	lidt (%rdi)
	sti
	retq
