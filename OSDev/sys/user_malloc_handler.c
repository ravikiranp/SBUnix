#include <function.h>
#include<defs.h>
#include<structs.h>
extern uint64_t* vmalloc(struct vm_area_struct*, uint64_t, uint16_t, uint16_t);
extern struct task_struct * current_task;

void user_malloc_handler()
{
	uint64_t * size = (uint64_t *)0xfffffe0000000000;
	//uint64_t * vma = (uint64_t *)0xfffffe0000000000;
	size[0] = (uint64_t)vmalloc(&(current_task->mm.mmap),(uint64_t)size[0],(uint64_t)7,current_task->task_id);
}
