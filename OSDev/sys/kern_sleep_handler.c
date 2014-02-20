#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;

void kern_sleep_handler()
{
	print_out(" In kern_sleep handler!! ");	
	uint16_t * sleep_sec = (uint16_t *)0xfffffe0000000000;
	current_task->sleep_count = sleep_sec[0];
	current_task->state = SLEEP;
	//print_out("...the sleep count of %d is set to %d and its state is now %d...",current_task->task_id,current_task->sleep_count,current_task->state);
	//__asm__ volatile(
	//	"sti;"
	//	: : :"memory"
	//);
	//while(1);
}

void decrement_sleep_count() {
        int i=0;
        for(i=1;i<=10;i++) {
                if(task[i].sleep_count > 0) {
                        task[i].sleep_count--;
                        if(task[i].sleep_count == 0 && task[i].state == SLEEP) {
                                task[i].state = READY;
				//print_out(" The state of the waiting task,%d, is now set to READY that is %d  ",task[i].task_id,task[i].state);
			}
                }
		else if (task[i].state == SLEEP && task[i].sleep_count == 0)
			task[i].state = READY;
        }
}
