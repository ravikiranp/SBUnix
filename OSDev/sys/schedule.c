#include <function.h>
#include <structs.h>
#include <sys/gdt.h>

extern struct tss_t tss;
extern struct task_struct * current_task;
extern struct task_struct * next_task;
extern int ctr;

void schedule() {
		/* CONTEXT SAVE */

		/*
		__asm__ volatile(
                	"movq %%rax, %0;"
                	"movq %%rbx, %1;"
                	"movq %%rbp, %2;"
                	"movq %%rcx, %3;"
                	"movq %%rdx, %4;"
                	"movq %%rdi, %5;"
                	"movq %%rsi, %6;"
                	"movq %%r9, %5;"
                	"movq %%r10, %9;"
                	"movq %%r11, %10;"
                	"movq %%r12, %11;"
                	"movq %%r13, %12;"
                	"movq %%r14, %13;"
                	"movq %%r15, %14;"
                	:"=r"(current_task->rax),"=r"(current_task->rbx),"=r"(current_task->rbp),"=r"(current_task->rcx),"=r"(current_task->rdx),"=r"(current_task->rdi),"=r"(current_task->rsi)//,"=r"(current_task->r8),"=r"(current_task->r9),"=r"(current_task->r10),"=r"(current_task->r11),"=r"(current_task->r12),"=r"(current_task->r13),"=r"(current_task->r14),"=r"(current_task->r15)
                	:
                	:"memory"
                	);
			*/
		if(current_task->task_id != 20) {
		__asm__ volatile (
				"pushq %%rax\n\t"
				"pushq %%rbx\n\t"
				"pushq %%rbp\n\t"
				"pushq %%rcx\n\t"
				"pushq %%rdx\n\t"
				"pushq %%rdi\n\t"
				"pushq %%rsi\n\t"
				"pushq %%r8\n\t"
				"pushq %%r9\n\t"
				"pushq %%r10\n\t"
				"pushq %%r11\n\t"
				"pushq %%r12\n\t"
				"pushq %%r13\n\t"
				"pushq %%r14\n\t"
				"pushq %%r15\n\t"
				:
				:
				:"memory"
			);	
			__asm__ volatile (
                	//"movq %%rsp,%%r12;"
			"movq %%rsp, %0;"
                	//"add $32,%%r12;"
                	//"movq %%r12,%0;"
                	:"=r"(current_task->task_kstack_top)
                	:
                	//:"%r12"
			:"memory"
                	);
		}
		//SCHEDULER
		//if(current_task->task_id != 20 && current_task->waitid == 20 && current_task->state == WAITING)
                	//current_task->state = READY;	
		int i=0,chosen=0;
		uint64_t first_switch = 0;
		uint64_t state = 0;//, waitid;
		//uint64_t scanf;
		//if(current_task->state == RUNNING)
		//	current_task->state = READY;

		__asm__ volatile(
                        "movq %1,%%rax\n\t"
                        "movq %%rax,%0\n\t"
                        :"=r" (state)
                        :"r" (current_task->state)
                        :"memory"
                );

		/*	
		__asm__ volatile(
                        "movq %1,%%rax\n\t"
                        "movq %%rax,%0\n\t"
                        :"=r" (scanf)
                        :"r" (current_task->kbd_global)
                        :"memory"
                );
		*/
		/*
		__asm__ volatile(
                        "movq %1,%%rax\n\t"
                        "movq %%rax,%0\n\t"
                        :"=r" (waitid)
                        :"r" (current_task->waitid)
                        :"memory"
                );
		*/
		if(state == RUNNING)
			current_task->state = READY;
	
		if(current_task->kbd_global == 0) {
			i = current_task->task_id;
			chosen = 1;
		}	

		while(chosen != 1 && current_task->kbd_global != 0) {// && scanf != 0) {
        		//print_out("    The current task's task id is %d   ",current_task.task_id);
        		for(i=1;i<=10;i++) {
                		if (i<=current_task->task_id)
                        		continue;
                		else {
                        		if(task[i].state == READY) {
                                		chosen = 1;
                                		break;
                        		}
                        		else
                                		continue;
                		}
        		}
       	 		if(chosen == 0) {
                		for(i=1;i<=10;i++) {
                        		if(i>=current_task->task_id)
                                		break;
                        		else {
                                		if(task[i].state == READY) {
                                        		chosen = 1;
                                        		break;
                                		}
                                		else
                                        		continue;
                        		}
                		}
                		if(chosen == 0 && current_task->state == READY) {
                        		i = current_task->task_id;
					chosen = 1;
				}
				if(chosen == 0) {
					//print_out(" STATE = %d  ",task[2].state);
					i = task[0].task_id;
					chosen = 1;
				}	
        		}
		}
		current_task = &task[i];
		current_task->state = RUNNING;
		//next_task = &task[i];
		tss.rsp0 = (uint64_t)&(current_task->task_kstack[255]); 
		//tss.rsp0 = (uint64_t)&(current_task->task_kstack[63]); 

		/*	
		__asm__ volatile (
                "movq %0, %%rax;"
                "movq %1, %%rbx;"
                "movq %2, %%rcx;"
                "movq %3, %%rdx;"
                "movq %4, %%rsi;"
                "movq %5, %%rdi;"
                "movq %6, %%rbp;"
                "movq %7, %%r8;"
                "movq %8, %%r9;"
                "movq %9, %%r10;"
                "movq %10, %%r11;"
                "movq %11, %%r12;"
                "movq %12, %%r13;"
                "movq %13, %%r14;"
                "movq %14, %%r15;"
                :
                :"r"(next_task->rax),"r"(next_task->rbx),"r"(next_task->rcx),"r"(next_task->rdx),"r"(next_task->rsi),"r"(next_task->rdi),"r"(next_task->rbp)//,"r"(next_task->r8),"r"(next_task->r9),"r"(next_task->r10),"r"(next_task->r11),"r"(next_task->r12),"r"(next_task->r13),"r"(next_task->r14),"r"(next_task->r15)
                :"memory"
                );
		*/	
	
		__asm__ volatile(
                	"movq %1,%%rax\n\t"
                	"movq %%rax,%0\n\t"
                	:"=r" (first_switch)
                	:"r" (current_task->first_switch)
                	:"memory"
                );

		__asm__ volatile(
			"movq %0,%%rsp;"
			:
			:"r"(current_task->task_kstack_top)
			:"memory"
			);

		if(first_switch == 0) {
			__asm__ volatile (
				"popq %%r15\n\t"
				"popq %%r14\n\t"
				"popq %%r13\n\t"
				"popq %%r12\n\t"
				"popq %%r11\n\t"
				"popq %%r10\n\t"
				"popq %%r9\n\t"
				"popq %%r8\n\t"
				"popq %%rsi\n\t"
				"popq %%rdi\n\t"
				"popq %%rdx\n\t"
				"popq %%rcx\n\t"
				"popq %%rbp\n\t"
				"popq %%rbx\n\t"
				"popq %%rax\n\t"
				: : :"memory"
			);
		
			__asm__ volatile  (
                		"movq %0, %%cr3;"
                		//"movq %1,%%rsp;"
				"add $8, %%rsp;"
                 		"movl $0x23,%%eax;"
                		"movl %%eax,%%ds  ;"
                 		"movl %%eax,%%es  ;"
                 		"movl %%eax,%%fs  ;"
                 		"movl %%eax,%%gs  ;"
                 		//"movb $0x20,%%al;"
                 		//"outb %%al,$0x20;"
                 		"sti;"
                 		"iretq;"
                		:
                		:"r"((uint64_t *)current_task->cr3)
                		:"%eax"
                	);
		}	
		else {
			current_task->first_switch = 0;
			__asm__ volatile  (
                		"movq %0, %%cr3;"
                		//"movq %1,%%rsp;"
                		//"add $8, %%rsp;"
                	 	"movl $0x23,%%eax;"
                		"movl %%eax,%%ds  ;"
                	 	"movl %%eax,%%es  ;"
                 		"movl %%eax,%%fs  ;"
                 		"movl %%eax,%%gs  ;"
                 		//"movb $0x20,%%al;"
                 		//"outb %%al,$0x20;"
                 		"sti;"
                 		"iretq;"
                		:
                		:"r"((uint64_t *)current_task->cr3)//, "r"(current_task->task_kstack_top)
                		:"%eax"
                	);
		}
}
