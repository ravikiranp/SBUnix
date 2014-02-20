#include<defs.h>
#include<function.h>
uint64_t kmalloc_free_ptr[20];
uint64_t stack_free_ptr[20];
extern uint64_t * get_free_kmalloc_page();
extern uint64_t * get_free_page_for_stack();

uint64_t * kern_malloc(uint16_t pid, int size) {
	uint64_t * free;
	uint64_t * update;
	if (kmalloc_free_ptr[pid] == 0) {
		free = get_free_kmalloc_page();	
		update = (uint64_t *)((uint64_t)free + (uint64_t)size);
		kmalloc_free_ptr[pid] = (uint64_t)update;
		return free;
	}
	else {
		free = (uint64_t *)kmalloc_free_ptr[pid];
		update = (uint64_t *)(kmalloc_free_ptr[pid] + (uint64_t)size);
		kmalloc_free_ptr[pid] = (uint64_t)update;
		return free;
	}	
}	
uint64_t * stack_malloc(uint16_t pid, int size) {
        uint64_t * free;
        uint64_t * update;
        if (stack_free_ptr[pid] == 0) {
                free = get_free_page_for_stack();
                update = (uint64_t *)((uint64_t)free + (uint64_t)size);
                stack_free_ptr[pid] = (uint64_t)update;
                return free;
        }
        else {
                free = (uint64_t *)stack_free_ptr[pid];
                update = (uint64_t *)(stack_free_ptr[pid] + (uint64_t)size);
                stack_free_ptr[pid] = (uint64_t)update;
                return free;
        }
}
