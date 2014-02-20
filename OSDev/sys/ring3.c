#include<defs.h>
#include<function.h>
void user_mode_func() {
        print_out(" In user mode!! Yeahhh  ");
        while(1);
}

void enter_usermode() {
//      int temp = 0x23;
        __asm__ (
                 "cli;"
                 "movl $0x23,%%eax;"
                 "movl %%eax,%%ds  ;"
                 "movl %%eax,%%es  ;"
                 "movl %%eax,%%fs  ;"
                 "movl %%eax,%%gs  ;"


                 "push $0x23;"
                 "push %%rsp;"
                 "pushfq;"
                 "push $0x1b;"
                 "lea (user_mode_func),%%rax;"
                 "push %%rax;"
                 "iretq"
                  :
                  ://"r"(user_mode_func)
                );
}
