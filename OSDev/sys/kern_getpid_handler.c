#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;
extern void func3();

void kern_getpid_handler()
{
	//print_out(" In kern_getpid handler!! ");	
	uint16_t * getpid = (uint16_t *)0xfffffe0000000000;
	getpid[0] = current_task->task_id;
}
