## KeyBoard ISR ##

.extern page_fault_isr_handler 
.extern error_code

.global page_fault_isr 

page_fault_isr:
        cli
        push %rsp
	movq %rsp,error_code
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rbp
        #push %rsp
        push %rsi
        push %rdi
        push %r9
        call page_fault_isr_handler

        # Acknowledge the interrupt
        #movb $0x20,%al
        #outb %al,$0x20

	#hlt
        pop %r9
        pop %rdi
        pop %rsi
        #pop %rsp
        pop %rbp
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
	//mov 8(%rsp),%rsp
	pop %rsp
	pop %r10
	#mov %r10, error_code
        sti
        iretq
