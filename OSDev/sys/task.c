#include <function.h>
#include <structs.h>
#include <sys/gdt.h>
extern struct task_struct task[10];
extern struct task_struct* cur_task;
extern uint64_t * stack_malloc(uint16_t, int);
extern uint64_t * get_free_page_for_tasks();
extern void map_user_page_virtual(uint64_t *, uint64_t, uint64_t, uint16_t, int);
extern void map_user_pages_for_cow(uint16_t, uint16_t);

extern struct task_struct * current_task;

void initialize_tasks() {
	task[0].kbd_global = 1;
        int i=0,j=0;
        for(i=1;i<=10;i++) {
                task[i].task_id = 20;
                task[i].waitid = 20;
                task[i].first_switch = 1;
                task[i].state = 0;
                task[i].sleep_count = 0;
                task[i].cr3 = (uint64_t)0;
                task[i].virtual_cr3 = (uint64_t)0;
		//task[i].child = 20;
		task[i].ref_count = 0;
		task[i].kbd_global = 1;
		task[i].kbd_buf_index = 0;
		task[i].num_open_files = 3;
		task[i].mm.mmap.vma_start = NULL;
		task[i].mm.mmap.vma_end= NULL;
		task[i].mm.mmap.vma_next= NULL;
		
		j=0;
		for(j=0;j<10;j++) {
			task[i].child[j] = 20;
			task[i].exec_child[j] = 20;
			task[i].open_files[j].sys_fd = -1;
			task[i].open_files[j].file_offset = 0;
			task[i].open_files[j].fd_state = 0;
		}

        }
        //print_out("     DONE INITIALIZING THE TASKS     ");
}

uint16_t create_task(uint64_t func, uint16_t ppid) {
//extern struct tss_t tss;

// Prepare the initial kernel stack 
	int i=0;
	uint16_t taskid = 0;
	
	if(ppid == 100) {
		task[0].task_id = 0;
		task[0].waitid = 20;
        	task[0].ref_count = 0;
		task[0].task_parent_id = ppid;	
		task[0].task_rip = func;
		//uint64_t * stack_free = stack_malloc(0,512);
		uint64_t * stack_free = stack_malloc(0,2048);
		task[0].task_kstack = (uint64_t *)stack_free;
		//stack_free = stack_malloc(0,512);
		stack_free = stack_malloc(0,2048);
		task[0].task_ustack = (uint64_t *)stack_free;	
		task[0].task_kstack_top = (uint64_t)&(task[0].task_kstack[255]);
		task[0].task_ustack_top = (uint64_t)&(task[0].task_ustack[255]);
		task[0].kmalloc_page_ptr = (uint64_t *)0xffffffff80d00000;
        	task[0].kmalloc_free_ptr = (uint64_t *)0xffffffff80d00000;
        	task[0].state = 4;
        	task[0].first_switch = 1;
		(task[0].task_kstack[255])    = 0x23;
        	(task[0].task_kstack[254])    = task[0].task_ustack_top;
        	(task[0].task_kstack[253])    = 0x200;
        	(task[0].task_kstack[252])    = 0x1b;
        	(task[0].task_kstack[251])    = task[0].task_rip;
        	task[0].task_kstack_top = (uint64_t)&(task[0].task_kstack[251]);
		uint64_t phys_base = (uint64_t)0xffffffff80000000;
		uint64_t * addr = get_free_page_for_tasks();
        	task[0].cr3 = (uint64_t *)addr;
        	uint64_t temp = (uint64_t)0xffffffff80000000;
        	temp = temp | (uint64_t)addr;
        	uint64_t * vir_addr = (uint64_t *)temp;
		task[0].virtual_cr3 = (uint64_t *)vir_addr;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x000000,2816,7);
		return 0;
	}
	for(i=1;i<=10;i++) {
		if(task[i].state == 0) {
			task[i].task_id = i;
			taskid = task[i].task_id;
			task[i].waitid = 20;
        		task[i].ref_count = 0;
			task[i].task_parent_id = ppid;	
			task[i].task_rip = func;
	
			uint64_t * stack_free = stack_malloc(i,2048);
			//uint64_t * stack_free = stack_malloc(i,512);
			task[i].task_kstack = (uint64_t *)stack_free;
			stack_free = stack_malloc(i,2048);
			//stack_free = stack_malloc(i,512);
			task[i].task_ustack = (uint64_t *)stack_free;	
	
			task[i].task_kstack_top = (uint64_t)&(task[i].task_kstack[255]);
			//task[i].task_kstack_top = (uint64_t)&(task[i].task_kstack[63]);
			task[i].task_ustack_top = (uint64_t)&(task[i].task_ustack[255]);
			//task[i].task_ustack_top = (uint64_t)&(task[i].task_ustack[63]);

			//task[i].func_name = (uint64_t *)func;
			task[i].kmalloc_page_ptr = (uint64_t *)0xffffffff80d00000;
        		task[i].kmalloc_free_ptr = (uint64_t *)0xffffffff80d00000;
        		task[i].state = 4;
        		task[i].first_switch = 1;

			(task[i].task_kstack[255])    = 0x23;
        		(task[i].task_kstack[254])    = task[i].task_ustack_top;
        		(task[i].task_kstack[253])    = 0x200;
        		(task[i].task_kstack[252])    = 0x1b;
        		(task[i].task_kstack[251])    = task[i].task_rip;

			//(task[i].task_kstack[63])    = 0x23;
        		//(task[i].task_kstack[62])    = task[i].task_ustack_top;
        		//(task[i].task_kstack[61])    = 0x200;
        		//(task[i].task_kstack[60])    = 0x1b;
        		//(task[i].task_kstack[59])    = task[i].task_rip;

        		task[i].task_kstack_top = (uint64_t)&(task[i].task_kstack[251]);
        		//task[i].task_kstack_top = (uint64_t)&(task[i].task_kstack[59]);

			//cur_task->task_rsp = (uint64_t)&(task[task_id].task_kstack[59]);	
			__asm__ volatile(
                                "movq %%rax, %0;"
                                "movq %%rbx, %1;"
                                "movq %%rbp, %2;"
                                "movq %%rcx, %3;"
                                "movq %%rdx, %4;"
                                "movq %%rsi, %5;"
                                "movq %%rdi, %6;"
                                "movq %%r8, %7;"
                                "movq %%r9, %8;"
                                "movq %%r10, %9;"
                                "movq %%r11, %10;"
                                "movq %%r12, %11;"
                                "movq %%r13, %12;"
                                "movq %%r14, %13;"
                                "movq %%r15, %14;"
                                :
                                :"r" (task[i].rax),"r" (task[i].rbx),"r" (task[i].rbp),"r" (task[i].rcx),"r" (task[i].rdx),"r" (task[i].rsi),"r" (task[i].rdi),"r" (task[i].r8),"r" (task[i].r9),"r" (task[i].r10),"r" (task[i].r11),"r" (task[i].r12),"r" (task[i].r13),"r" (task[i].r14),"r" (task[i].r15)
                                :"memory"
                                );
			break;
		}
	}
	if(ppid == 0) {
		uint64_t phys_base = (uint64_t)0xffffffff80000000;
		uint64_t * addr = get_free_page_for_tasks();
        	task[i].cr3 = (uint64_t *)addr;
        	uint64_t temp = (uint64_t)0xffffffff80000000;
        	temp = temp | (uint64_t)addr;
        	uint64_t * vir_addr = (uint64_t *)temp;
		task[i].virtual_cr3 = (uint64_t *)vir_addr;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x000000,2816,7);
		phys_base = phys_base + (uint64_t)0x100000;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x100000,2048,5);
		phys_base = phys_base + (uint64_t)0x800000;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x900000,512,7);
		addr = get_free_page_for_tasks();
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)0xfffffe0000000000,(uint64_t)addr,1,7);
	}
	if(ppid != 0) {
		int j=0;
		for(j=0;j<10;j++) {
			if(task[ppid].child[j] == 20) {
				task[ppid].child[j] = i;
				break;
			}
		}
		task[i].mm = task[ppid].mm;
		//task[i].mm.mmap = task[ppid].mm.mmap;
		uint64_t phys_base = (uint64_t)0xffffffff80000000;
		uint64_t * addr = get_free_page_for_tasks();
        	task[i].cr3 = (uint64_t *)addr;
        	uint64_t temp = (uint64_t)0xffffffff80000000;
        	temp = temp | (uint64_t)addr;
        	uint64_t * vir_addr = (uint64_t *)temp;
        	task[i].virtual_cr3 = (uint64_t *)vir_addr;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x000000,256,7);
		phys_base = phys_base + (uint64_t)0x100000;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x100000,2048,5);
		phys_base = phys_base + (uint64_t)0x800000;
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x900000,512,7);
		addr = get_free_page_for_tasks();
		map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)0xfffffe0000000000,(uint64_t)addr,1,7);
	
		map_user_pages_for_cow((uint16_t)i,(uint16_t)ppid);

		int i = 0;
		for(i=0;i<256;i++) {
			task[taskid].task_ustack[i] = current_task->task_ustack[i];
		}	
		uint64_t * kstack_values= (uint64_t *)0xfffffe0000000000;
		//print_out("   The kern stack values is going to be %x  %x   %x   %x   %x ",kstack_values[0],kstack_values[1],kstack_values[2],kstack_values[3],kstack_values[4]);
		uint64_t offset = (uint64_t)kstack_values[3] << 52;
		offset = offset >> 52;
		kstack_values[3] = (uint64_t)task[taskid].task_ustack | (offset); 
		//(task[taskid].task_kstack[63]) = (uint64_t)kstack_values[4];	
		//(task[taskid].task_kstack[62]) = (uint64_t)kstack_values[3];	
		//(task[taskid].task_kstack[61]) = (uint64_t)kstack_values[2];	
		//(task[taskid].task_kstack[60]) = (uint64_t)kstack_values[1];	
		//(task[taskid].task_kstack[59]) = (uint64_t)kstack_values[0];	
		(task[taskid].task_kstack[255]) = (uint64_t)kstack_values[4];	
		(task[taskid].task_kstack[254]) = (uint64_t)kstack_values[3];	
		(task[taskid].task_kstack[253]) = (uint64_t)kstack_values[2];	
		(task[taskid].task_kstack[252]) = (uint64_t)kstack_values[1];	
		(task[taskid].task_kstack[251]) = (uint64_t)kstack_values[0];	
		//print_out(" The kernel stacktop is at %x  and the base is at %x the user stack begins at %x",task[taskid].task_kstack[62],task[taskid].task_kstack,task[taskid].task_ustack);
	}
	total_tasks-=1;
	return taskid;	
}


// Switch from kernel stack to the task_id
void switch_to(uint64_t task_id) {
	
	tss.rsp0 = (uint64_t)&(task[task_id].task_kstack[255]);
	current_task = &task[task_id];
	current_task->first_switch = 0;	
	__asm__ volatile(
                "movq %0,%%cr3\n\t"
                :
                :"r"((uint64_t *)task[task_id].cr3)
                :"memory"
                );
	 uint16_t sel = 0x28;
        __asm__ volatile (
                "ltr %0;"
		"movl $0x23,%%eax;"
                "movl %%eax,%%ds  ;"
                "movl %%eax,%%es  ;"
                "movl %%eax,%%fs  ;"
                "movl %%eax,%%gs  ;"
		"movq %1,%%rsp;"
		"sti;"
		"iretq;"
                :
                :"r"(sel),"r"(task[task_id].task_kstack_top)
		:"%eax"
                );
}
