.extern user_readdir_handler 

.global user_readdir_int


user_readdir_int:
	cli
#        push %rsp
#        movq %rsp,error_code
	push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rsi
        push %rdi
	push %r8
        push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15
	


	call user_readdir_handler
	//call schedule	
	
#	movl $0x23,%eax
#        movl %eax,%ds 
#        movl %eax,%es 
#        movl %eax,%fs 
#        movl %eax,%gs 

	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10	
	pop %r9
	pop %r8
        pop %rdi
        pop %rsi
	pop %rdx
        pop %rcx
        pop %rbx
	pop %rax

#	pop %rsp
#	pop %r11
	
#	pop %r10
 #       add $2,%r10
#	push %r10;
	
        sti

	iretq
