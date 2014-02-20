#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;
extern void func3();

void syscall_handler()
{
	print_out(" In syscall handler!! ");	
	while(1);
}
