#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;

void kern_exit_handler()
{
	int j=0;
	print_out(" In kern_exit handler!! task %d is exiting  ",current_task->task_id);	
	if(current_task->ref_count != 0) {
		//print_out("\n the current task is a parent which still has active childre, hence will wait for the children to complete  ");
		current_task->state = EXIT_WAIT;
		__asm__ volatile (
			"sti;"
			: : :"memory"
		);
		while(1);
	}
	free_stack_page(current_task->task_id);
        free_kmalloc_page(current_task->task_id);
        current_task->state = COMPLETE;
        current_task->task_id = 20;
        current_task->waitid = 20;
	for(j=0;j<10;j++)
        	current_task->child[j]= 20;
        current_task->sleep_count = 0;
        current_task->first_switch = 1;
	current_task->ref_count = 0;
	
	if(current_task->task_parent_id != 0) {
		uint64_t ppid = current_task->task_parent_id;
		task[ppid].ref_count--;
	}
	
	total_tasks-=1;
	print_out("  The task id is reset to %d  ",current_task->task_id);
	__asm__ volatile(
		"sti;"
		: : :"memory"
		);
        //current_task->cr3 = (uint64_t)0;
	while(1);
}

void check_child_processes() {
	int i=0,j=0;
	for(i=1;i<=10;i++) {
		if(task[i].state == EXIT_WAIT && task[i].ref_count == 0) {
			free_stack_page(task[i].task_id);
        		free_kmalloc_page(task[i].task_id);
        		task[i].state = COMPLETE;
        		task[i].task_id = 20;
        		task[i].waitid = 20;
        		for(j=0;j<10;j++)
                		task[i].child[j]= 20;
        		task[i].sleep_count = 0;
        		task[i].first_switch = 1;
        		task[i].ref_count = 0;

        		if(task[i].task_parent_id != 0) {
                		uint64_t ppid = task[i].task_parent_id;
                		task[ppid].ref_count--;
        		}

        		total_tasks-=1;
        		print_out("  The parent task with id %d is reset to %d  ",i,task[i].task_id);
		}
	}
}
