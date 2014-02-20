#include <function.h>
#include<defs.h>
#include<structs.h>

extern void free_stack_page(uint16_t);
extern void free_kmalloc_page(uint16_t);
extern struct task_struct * current_task;

void kern_print_handler()
{
	char * buf_ptr = (char *)0xfffffe0000000000;
	//print_out(" In kern_print handler!! ");	
	kern_write_str(buf_ptr);
}
