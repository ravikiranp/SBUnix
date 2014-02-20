#include<syscall.h>
uint16_t fork() {
	uint64_t * ret_addr = (uint64_t *)0xfffffe0000000000;
        __asm__ volatile(
                "movq 8(%%rsp),%0;"
                :"=r"(*ret_addr)
                :
                :"memory"
                );
	
        __syscall0(3);

	uint64_t * ret_val = (uint64_t *)0xffffff7fbfdfefe8;
        return ret_val[0];
}
