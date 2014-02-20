#
# Remap the int #'s  to begin from 32 instead of 0
#
#
.text

.global _x86_asm_remap_intr
_x86_asm_remap_intr:
	
	movb $0x11,%al
	outb %al,$0x20
	outb %al,$0xa0

	movb $0x20,%al
	outb %al,$0x21
	movb $0x28,%al
	outb %al,$0xa1

	movb $0x4,%al
	outb %al,$0x21
	movb $0x2,%al
	outb %al,$0xa1

	movb $0x1,%al
	outb %al,$0x21
	outb %al,$0xa1
	
	# Enable interrupts before you leave
	sti 
	retq
	
