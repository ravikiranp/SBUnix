#include<function.h>
void GPF_isr_handler () {
	print_out("    General Protection fault Caught    ");
	while(1);
/*
	uint64_t * virtual_addr;
	asm volatile("movq %%cr2, %0"
                        : "=b" (virtual_addr)
                        : 
                );
	uint64_t free = (uint64_t)get_free_page();
	print_out("mapping %p to %x     phys_free is at %p",virtual_addr,free,virtual_physfree); 
	map_page_virtual((uint64_t *)virtual_physfree,(uint64_t)virtual_addr,free,1);	
	
	char * try = (char *)virtual_addr;
	try[0] = 'b';
	print_out("   Haha....%c", try[0]);
	//while(1);
*/
}
