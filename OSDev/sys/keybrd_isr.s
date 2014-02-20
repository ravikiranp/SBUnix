## KeyBoard ISR ##

.extern keybrd_isr_handler
.global keybrd_isr

keybrd_isr:
        cli
        push %rax
        push %rbx
        push %rcx
        push %rdx
        push %rbp
        push %rsp
        push %rsi
        push %rdi
        push %r9
        call keybrd_isr_handler
        # Acknowledge the interrupt
        movb $0x20,%al
        outb %al,$0x20

        pop %r9
        pop %rdi
        pop %rsi
        pop %rsp
        pop %rbp
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
        sti
        iretq
