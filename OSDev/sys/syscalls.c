#include<defs.h>
#include<function.h>
#include<structs.h>
extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;
extern uint16_t getpid;
extern uint16_t sleep_sec;
extern uint16_t waitid;

uint64_t kern_fork() {
	uint64_t * ret_addr = (uint64_t *)0xfffffe0000000000;
	__asm__ volatile(
		"movq 8(%%rsp),%0;"
		:"=r"(*ret_addr)
		:
		:"memory"
		);
	__asm__ volatile(
		"mov $0x81,%%eax;"
                "int $0x81;"
		:
		:
		:"memory"
		);
	uint64_t * ret_val = (uint64_t *)0xffffff7fbfdfefe8;
	print_out(" The ret value is %d   ",ret_val[0]);
	return ret_val[0];		
}

void kern_exec(const char * filename) {

        uint64_t * file_name = (uint64_t *)0xfffffe0000000000;
	file_name[0] = (uint64_t)filename;

        __asm__ volatile(
                "mov $0x90,%%eax;"
                "int $0x90;"
                :
                :
                :"memory"
                );
}

uint16_t kern_getpid() {
        __asm__ volatile(
                "mov $0x82,%%eax;"
                " int $0x82;"
                :
                :
                :"memory"
                );
	uint16_t * getpid = (uint16_t *)0xfffffe0000000000;
	return getpid[0];
}

void kern_sleep(uint16_t sec) {
	uint16_t * sleep_sec = (uint16_t *)0xfffffe0000000000;
	sleep_sec[0] = (uint16_t)sec;
	print_out("...The sleep count will be set to %d...",sleep_sec[0]);
        __asm__ volatile(
                "mov $0x83,%%eax;"
                " int $0x83;"
                :
                :
                :"memory"
                );
	uint64_t * sleep_wait = (uint64_t *)0xfffffe0000000000;
	sleep_wait[0] = 10000000;
	while(sleep_wait[0] > 0)
		sleep_wait[0]--;
}

void kern_waitpid(uint16_t task_id) {
	uint16_t * waitid = (uint16_t *)0xfffffe0000000000;
	waitid[0] = task_id;
        __asm__ volatile(
                "mov $0x84,%%eax;"
                " int $0x84;"
                :
                :
                :"memory"
                );
	uint64_t * waitid_wait = (uint64_t *)0xfffffe0000000000;
	waitid_wait[0] = 10000000;
	while(waitid_wait[0] > 0)
		waitid_wait[0]--;
}

void kern_exit() {
        __asm__ volatile(
                "mov $0x85,%%eax;"
                " int $0x85;"
                :
                :
                :"memory"
                );
	//while(1);
}

void kern_printf(char * a) {
        __asm__ volatile(
                "mov $0x86,%%eax;"
                " int $0x86;"
                :
                :
                :"memory"
                );
}
