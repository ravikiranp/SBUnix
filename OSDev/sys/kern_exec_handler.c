#include <function.h>
#include<defs.h>
#include<structs.h>
#include"elf_header.h"
#include "fs.h"

extern struct parse_info * file;
extern struct load_info * load_file_list;
extern struct task_struct * current_task;

extern void change_perm(uint64_t *, int);
extern struct parse_info* parse_elf(char*);
extern uint64_t* create_vma(struct vm_area_struct* ,uint64_t* , uint64_t ,uint16_t,uint16_t);
extern void map_user_page_virtual(uint64_t *, uint64_t, uint64_t, uint16_t, int);
extern void clear_all_user_page_entries(uint64_t *);

void kern_exec_handler()
{
	//print_out(" In kern_exec handler!! ");	
	char * filename = (char *)0xfffffe0000000000;
	//print_out(" the parameter is %s.......",filename);
	int res = kern_open((char *)filename);
	if(res == -1) {
		print_out("\n**********Cannot open the file %s - %d**********\n",(char *)filename,res);
		return;
	}
	else
		kern_close(res);

	//uint16_t parent = current_task->task_parent_id;
	//if(task[parent].ref_count == 1 && task[parent].child[0] == current_task->task_id) {
	//	change_perm((uint64_t *)task[parent].virtual_cr3,7);
	//}

	uint16_t parent = current_task->task_parent_id;
	int j=0;
	for(j=0;j<10;j++) {
		if(task[parent].child[j] == current_task->task_id)
			task[parent].child[j] = 20;
	}
	for(j=0;j<10;j++) {
		if(task[parent].exec_child[j] == 20)
			task[parent].exec_child[j] = current_task->task_id;
			break;
	}
	current_task->kbd_global = 1;
        current_task->kbd_buf_index = 0;
        //current_task->num_open_files = 3;
        current_task->mm.mmap.vma_start = NULL;
        current_task->mm.mmap.vma_end= NULL;
        current_task->mm.mmap.vma_next= NULL;

	file = parse_elf((char *)filename);
        load_file_list = file->load_list;

	current_task->first_switch = 1;
	current_task->task_kstack[255] = 0x23;
	current_task->task_kstack[254] = current_task->task_ustack_top;
        current_task->task_kstack[253] = 0x200;
        current_task->task_kstack[252] = 0x1b;
        current_task->task_kstack[251] = (uint64_t)file->entry_point;

	current_task->task_rip = (uint64_t)file->entry_point;
	
	current_task->task_kstack_top = (uint64_t)&(current_task->task_kstack[251]);
	
	clear_all_user_page_entries((uint64_t *)current_task->virtual_cr3);

	char * copy_from;
        char * copy_to;
        uint64_t loc_offset = 0;
	uint64_t free;	
	int page_size;
        uint16_t no_of_pages = 0;

	while(load_file_list != NULL) {
                free = (uint64_t)get_free_page();
                page_size = (int) load_file_list->file_seg_size;
                no_of_pages = 0;
                while(page_size > 0) {
                        no_of_pages += 1;
                        page_size -= 4096;
                }

                map_user_page_virtual((uint64_t *)current_task->virtual_cr3,(uint64_t)load_file_list->v_addr,free,no_of_pages,7);

                //print_out("\n the file offset is %d ",load_file_list->file_offset);
                copy_from = (char *)((uint64_t)file->file_start_ptr + (uint64_t)load_file_list->file_offset);
                copy_to = (char *) load_file_list->v_addr;
                loc_offset = 0;

                //print_out(" \nthe contents of %d no of pages starting from %x will be copied to %x, the file exec point is %x and the task id of this task will be %d    ",no_of_pages,copy_from,copy_to,file->entry_point,current_task->task_id);

                for(loc_offset = 0;loc_offset<load_file_list->file_seg_size;loc_offset++) {
                        *copy_to = *copy_from;
                        copy_from+=1;
                        copy_to+=1;
                }
                create_vma(&(current_task->mm.mmap),(uint64_t *)load_file_list->v_addr,load_file_list->mem_seg_size,(uint16_t)load_file_list->flags,current_task->task_id);
                load_file_list = load_file_list->next;
        }

	__asm__ volatile(
		"movl $0x23,%%eax;"
                "movl %%eax,%%ds  ;"
                "movl %%eax,%%es  ;"
                "movl %%eax,%%fs  ;"
                "movl %%eax,%%gs  ;"	
		"movq %0,%%rsp;"
		"sti;"
                "iretq;"
                :
                :"r"(current_task->task_kstack_top)
                :"%eax"
                );
}
