#
#   Timer ISR
#
#

.extern clockprint
.extern schedule
.extern check_waitpid

.global timer_isr

timer_isr:
	cli
	
	push %rax
	#push %rbx
	#push %rcx
	#push %rdx
	#push %rsi
	#push %rdi
	#push %rbp

	//call clockprint
	# Acknowledge the interrupt
	movb $0x20,%al
	outb %al,$0x20 

	pop %rax
	call clockprint
	//call check_waitpid 
	call schedule
	#pop %rbp
	#pop %rdi
	#pop %rsi
	#pop %rdx
	#pop %rcx
	#pop %rbx
	#pop %rax
	sti
	iretq
