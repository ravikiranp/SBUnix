#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;
extern void func5();
extern void change_perm(uint64_t *, int);

void kern_fork_handler()
{
	uint16_t id = current_task->task_id;
	uint16_t child_id;
	if(total_tasks <= 10) {
		current_task->ref_count++;
		//print_out(" In kern_fork handler!! ");	
		uint64_t * kstack_values = (uint64_t *)0xfffffe0000000000;
		__asm__ volatile(
			"movq 128(%%rsp),%0;"
			"movq 136(%%rsp),%1;"
			"movq 144(%%rsp),%2;"
			"movq 152(%%rsp),%3;"
			"movq 160(%%rsp),%4;"
			:"=r"(kstack_values[0]),"=r"(kstack_values[1]),"=r"(kstack_values[2]),"=r"(kstack_values[3]),"=r"(kstack_values[4])
			:
			:"memory"
			);
		//print_out("   The kern stack values is going to be %x  %x   %x   %x   %x ",kstack_values[0],kstack_values[1],kstack_values[2],kstack_values[3],kstack_values[4]);
		change_perm((uint64_t *)current_task->virtual_cr3,5);
		child_id = (uint64_t)create_task((uint64_t)func5,id);

		//task[taskid].child = (uint16_t)child_id;
		//current_task->child = (uint16_t)child_id;
		//uint16_t child_count = current_task->child_count;
		//current_task->child[child_count] = child_id; 
		//current_task->child_count+=1;
		task[child_id].state = 4;
		uint64_t * ret_val = (uint64_t *)0xffffff7fbfdfefe8;
		ret_val[0] = (uint64_t)child_id;
		//print_out("   The child_id at 509 entry of pml4e is %d    ",ret_val[0]);
		uint64_t * child_cr3 = (uint64_t *)task[child_id].virtual_cr3;
		child_cr3[509] = (uint64_t)0;
	}

	else {
		print_out(" \n\n ALERT!!!! The system exceeded the number of allowed tasks   \n\n");
		while(1);
	}	
}
