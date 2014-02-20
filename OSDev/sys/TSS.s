#
#   Timer ISR
#
#

.extern TSS_isr_handler 

.global TSS_isr

TSS_isr:
	cli

	//call timer_find_new_func

	#hlt

	push %rbx
	push %rbp
	push %rax
	push %rcx
	push %rdx
	push %rsp
	push %rsi
	push %rdi
	push %r9
	#push %r12
	#push %r13
	#push %r14
	#push %r15

	//mov %rsp,old_func
	
	# Acknowledge the interrupt
	movb $0x20,%al
	outb %al,$0x20 
	
	call TSS_isr_handler	
	//call timer_switch 

	#pop %r15
	#pop %r14
	#pop %r13
	#pop %r12
	pop %r9
	pop %rdi
	pop %rsi
	pop %rsp
	pop %rdx
	pop %rcx
	pop %rax
	pop %rbp
	pop %rbx
	sti
	iretq
