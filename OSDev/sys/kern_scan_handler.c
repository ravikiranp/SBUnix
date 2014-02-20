#include <function.h>
#include<defs.h>
#include<structs.h>
#include<stdio.h>
extern struct task_struct * current_task;

void dummy()
{
}
void kern_scan_handler()
{
	//print_out(" In kern_scan handler ");	
	current_task->kbd_global = 0;
	current_task->kbd_buf_index = 0;
	__asm__ volatile(
		"sti;"
		: : :"memory"
	);
	//char buf[5]="";
	while(current_task->kbd_global != 1) {
		//print_out("%s",buf);
		dummy();
        }
}
