#include<function.h>
#include<structs.h>
#include<defs.h>
#include"elf_header.h"
extern void map_user_page_virtual(uint64_t *, uint64_t, uint64_t, uint16_t, int);
extern uint64_t* create_vma(struct vm_area_struct* ,uint64_t* , uint64_t ,uint16_t,uint16_t);
extern int parse_taskid;
extern struct parse_info * file; 
extern struct load_info * load_file_list;
extern uint64_t * virtual_physfree;
extern uint64_t file_cr3;

//void load_file(uint64_t * src, uint64_t offset, uint64_t size, uint64_t * dest, uint64_t * entry_point) {
void load_file() {

	 file_cr3 = (uint64_t)get_free_page_for_tasks();	

	 uint64_t phys_base = (uint64_t)0xffffffff80000000;
         uint64_t * addr = (uint64_t *)file_cr3;
         task[parse_taskid].cr3 = (uint64_t *)addr;
         uint64_t temp = (uint64_t)0xffffffff80000000;
         temp = temp | (uint64_t)addr;
         uint64_t * vir_addr = (uint64_t *)temp;
         task[parse_taskid].virtual_cr3 = (uint64_t *)vir_addr;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x000000,2816,7);

	/*
	 phys_base = phys_base + (uint64_t)0x100000;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x100000,2048,3);
         phys_base = phys_base + (uint64_t)0x800000;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x900000,512,7);
	*/
         //phys_base = phys_base + (uint64_t)0x100000;
         //map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0xa00000,256,7);
         addr = get_free_page_for_tasks();
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)0xfffffe0000000000,(uint64_t)addr,1,7);

	 __asm__ volatile("movq %0,%%cr3;"
			:
			:"b"((uint64_t *)task[parse_taskid].cr3)
			);

	//file = parse_elf("bin/hello");
        //load_file_list = file->load_list;

	 uint64_t free;
	 int page_size;
	 uint16_t no_of_pages = 0;
	 //entry_point = (uint64_t *)0x4000b0;

	 //i = parse_taskid;
	 task[parse_taskid].task_id = parse_taskid;
         task[parse_taskid].waitid = 20;
         task[parse_taskid].ref_count = 0;
         task[parse_taskid].task_parent_id = 0;
         task[parse_taskid].task_rip = (uint64_t)file->entry_point;

         uint64_t * stack_free = stack_malloc(parse_taskid,2048);
         task[parse_taskid].task_kstack = (uint64_t *)stack_free;
         stack_free = stack_malloc(parse_taskid,2048);
         task[parse_taskid].task_ustack = (uint64_t *)stack_free;

         task[parse_taskid].task_kstack_top = (uint64_t)&(task[parse_taskid].task_kstack[255]);
         task[parse_taskid].task_ustack_top = (uint64_t)&(task[parse_taskid].task_ustack[255]);

         task[parse_taskid].kmalloc_page_ptr = (uint64_t *)0xffffffff80d00000;
         task[parse_taskid].kmalloc_free_ptr = (uint64_t *)0xffffffff80d00000;
         task[parse_taskid].state = 4;
         task[parse_taskid].first_switch = 1;

         (task[parse_taskid].task_kstack[255])    = 0x23;
         (task[parse_taskid].task_kstack[254])    = task[parse_taskid].task_ustack_top;
         (task[parse_taskid].task_kstack[253])    = 0x200;
         (task[parse_taskid].task_kstack[252])    = 0x1b;
         (task[parse_taskid].task_kstack[251])    = (uint64_t)file->entry_point;

         task[parse_taskid].task_kstack_top = (uint64_t)&(task[parse_taskid].task_kstack[251]);

	 char * copy_from;
	 char * copy_to; 
	 uint64_t loc_offset = 0;

	 while(load_file_list != NULL) {
	 	free = (uint64_t)get_free_page();
	 	page_size = (int) load_file_list->file_seg_size;
	 	no_of_pages = 0;
		while(page_size > 0) {
			no_of_pages += 1;
			page_size -= 4096;
		}

         	map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)load_file_list->v_addr,free,no_of_pages,7);	

		//print_out("\n the file offset is %d ",load_file_list->file_offset);
	 	copy_from = (char *)((uint64_t)file->file_start_ptr + (uint64_t)load_file_list->file_offset);
	 	copy_to = (char *) load_file_list->v_addr;
	 	loc_offset = 0;

	 	//print_out(" \nthe contents of %d no of pages starting from %x will be copied to %x, the file exec point is %x and the task id of this task will be %d",no_of_pages,copy_from,copy_to,file->entry_point,parse_taskid);

	 	for(loc_offset = 0;loc_offset<load_file_list->file_seg_size;loc_offset++) {
			*copy_to = *copy_from;
			copy_from+=1;
		 	copy_to+=1;
		}
		create_vma(&(task[parse_taskid].mm.mmap),(uint64_t *)load_file_list->v_addr,load_file_list->mem_seg_size,(uint16_t)load_file_list->flags,parse_taskid);
		load_file_list = load_file_list->next;
	}

	uint64_t phy = (uint64_t)virtual_physfree;	
	phy = phy & (uint64_t)0x000000000fffffff;
	

	__asm__ volatile("mov %0, %%cr3"
		: :"b"((uint64_t *)phy)
		);
	
	total_tasks -= 1;
}	

void load_shell() {

	 file_cr3 = (uint64_t)get_free_page_for_tasks();	

	 uint64_t phys_base = (uint64_t)0xffffffff80000000;
         uint64_t * addr = (uint64_t *)file_cr3;
         task[0].cr3 = (uint64_t *)addr;
         uint64_t temp = (uint64_t)0xffffffff80000000;
         temp = temp | (uint64_t)addr;
         uint64_t * vir_addr = (uint64_t *)temp;
         task[0].virtual_cr3 = (uint64_t *)vir_addr;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x000000,256,7);
	 phys_base = phys_base + (uint64_t)0x100000;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x100000,2048,3);
         phys_base = phys_base + (uint64_t)0x800000;
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)phys_base,(uint64_t)0x900000,512,7);
         //addr = get_free_page_for_tasks();
         //map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)0x000000,(uint64_t)addr,1,7);
         addr = get_free_page_for_tasks();
         map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)0xfffffe0000000000,(uint64_t)addr,1,7);

	 __asm__ volatile("movq %0,%%cr3;"
			:
			:"b"((uint64_t *)task[0].cr3)
			);

	//file = parse_elf("bin/hello");
        //load_file_list = file->load_list;

	 uint64_t free;
	 int page_size;
	 uint16_t no_of_pages = 0;
	 //entry_point = (uint64_t *)0x4000b0;

	 //i = parse_taskid;
	 task[0].task_id = 0;
         task[0].waitid = 20;
         task[0].ref_count = 0;
         task[0].task_parent_id = 0;
         task[0].task_rip = (uint64_t)file->entry_point;

         uint64_t * stack_free = stack_malloc(0,2048);
         task[0].task_kstack = (uint64_t *)stack_free;
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
         (task[0].task_kstack[251])    = (uint64_t)file->entry_point;

         task[0].task_kstack_top = (uint64_t)&(task[0].task_kstack[251]);

	 char * copy_from;
	 char * copy_to; 
	 uint64_t loc_offset = 0;

	 while(load_file_list != NULL) {
	 	free = (uint64_t)get_free_page();
	 	page_size = (int) load_file_list->file_seg_size;
	 	no_of_pages = 0;
		while(page_size > 0) {
			no_of_pages += 1;
			page_size -= 4096;
		}

         	map_user_page_virtual((uint64_t *)vir_addr,(uint64_t)load_file_list->v_addr,free,no_of_pages,7);	

		//print_out("\n the file offset is %d ",load_file_list->file_offset);
	 	copy_from = (char *)((uint64_t)file->file_start_ptr + (uint64_t)load_file_list->file_offset);
	 	copy_to = (char *) load_file_list->v_addr;
	 	loc_offset = 0;

	 	//print_out(" \nthe contents of %d no of pages starting from %x will be copied to %x, the file exec point is %x and the task id of this task will be 0",no_of_pages,copy_from,copy_to,file->entry_point);

	 	for(loc_offset = 0;loc_offset<load_file_list->file_seg_size;loc_offset++) {
			*copy_to = *copy_from;
			copy_from+=1;
		 	copy_to+=1;
		}
		create_vma(&(task[0].mm.mmap),(uint64_t *)load_file_list->v_addr,load_file_list->mem_seg_size,(uint16_t)load_file_list->flags,0);
		load_file_list = load_file_list->next;
	}

	uint64_t phy = (uint64_t)virtual_physfree;	
	phy = phy & (uint64_t)0x000000000fffffff;
	

	__asm__ volatile("mov %0, %%cr3"
		: :"b"((uint64_t *)phy)
		);
	parse_taskid = 0;	
	total_tasks -= 1;
}	
