#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <defs.h>
#include<function.h>

#define SYSCALL_PROTO(n) static __inline uint64_t __syscall##n

SYSCALL_PROTO(0)(uint64_t n) {
	//uint16_t pid;
	//uint64_t fork_ret;
	switch(n) {
		case 1: //EXIT 
			__asm__ volatile(
                		"mov $0x85,%%eax;"
                		" int $0x85;"
                		: : :"memory"
                		);	
			break;	
		case 2: //GET_PID
			__asm__ volatile(
                		"mov $0x82,%%eax;"
                		" int $0x82;"
                		:
                		:
                		:"memory"
                	);
			break;
	
		case 3: //FORK
			__asm__ volatile(
                		"mov $0x81,%%eax;"
                		"int $0x81;"
                		:
                		:
                		:"memory"
                	);
			break;	

		case 4: //SCANF 
                        __asm__ volatile(
                                "mov $0x87,%%eax;"
                                " int $0x87;"
                                : : :"memory"
                                );
                        break;

		case 5: //DUMMY
			__asm__ volatile(
				"mov $0x80,%%eax;"
				"int $0x80;"
				: : :"memory"
			);
			break;

		default: break;
	}
	return 0;
}

SYSCALL_PROTO(1)(uint64_t n, uint64_t a1) {
	switch(n) {
		case 1: //kern_sleep((uint16_t)a1);
			__asm__ volatile(
                		"mov $0x83,%%eax;"
                		" int $0x83;"
                		:
                		:
                		:"memory"
                	);
			break;

		case 2: //kern_waitpid((uint16_t)a1);
			__asm__ volatile(
                		"mov $0x84,%%eax;"
                		" int $0x84;"
                		:
                		:
                		:"memory"
                		);
			break;

		case 3: //kern_printf((char *)a1);
			__asm__ volatile(
                		"mov $0x86,%%eax;"
                		" int $0x86;"
                		:
                		:
                		:"memory"
                		);
			break;

		case 4: //user_malloc
			__asm__ volatile (
				"mov $0x88,%%eax;"
				"int $0x88;"
				: : :"memory"
			);
			break;

		case 5: //open
                        __asm__ volatile (
                                "mov $0x89,%%eax;"
                                "int $0x89;"
                                : : :"memory"
                        );
                        break;

		case 6: //exec
                        __asm__ volatile (
                                "mov $0x52,%%eax;"
                                "int $0x52;"
                                : : :"memory","%eax"
                        );
                        break;

		case 7: //close
                        __asm__ volatile (
                                "mov $0x50,%%eax;"
                                "int $0x50;"
                                : : :"memory","%eax"
                        );
                        break;

		default : break;
	}
	return 0;
}

SYSCALL_PROTO(2)(uint64_t n, uint64_t a1, uint64_t a2) {
	switch(n) {
		case 1: //kscanf((char *)a1,(uint64_t *)a2); 
			break;
		default: break;
	}
	return 0;
}

SYSCALL_PROTO(3)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	switch(n) {
		case 1: //read
			__asm__ volatile(
				"mov $0x51, %%eax;"
				"int $0x51;"
				: : :"memory"
			);
			break;
	
		default : break;
	}
	return 0;
}

SYSCALL_PROTO(4)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
	return 0;
}

#endif
