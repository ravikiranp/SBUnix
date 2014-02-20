#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;
extern uint64_t waitid;

void kern_waitpid_handler()
{
	print_out(" In kern_waitpid handler!! ");	
	uint16_t * waitid = (uint16_t *)0xfffffe0000000000;
	current_task->waitid = waitid[0];
	print_out(" task.waitid of %d is now set to %d  ",current_task->task_id,current_task->waitid);
        current_task->state = WAITING;
}

void check_waitpid() {
        uint16_t wait;
	int i;
        for(i=1;i<=10;i++) {
                if(task[i].waitid != 20) {
                        wait = task[i].waitid;
                        if(task[wait].state == 0 && task[i].state == WAITING) {
                                task[i].state = READY;
                                task[i].waitid = 20;
                        }
                }
        }
}
