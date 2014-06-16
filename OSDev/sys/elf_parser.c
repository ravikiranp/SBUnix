#include <defs.h>
#include <function.h>
#include<structs.h>
#include "elf_header.h"
extern uint64_t * kern_malloc(uint16_t pid, int size);
extern struct task_struct task[10];
extern uint16_t parse_taskid;

struct parse_info* elf_parser(uint64_t parse_ptr) {

	int j=0;
	for(j=1;j<=10;j++) {	
		if(task[j].task_id == 20 && task[j].state == 0) {
			parse_taskid = j;
			break;
		}
	}

	struct parse_info *ptr = (struct parse_info *)kern_malloc(j,sizeof(struct parse_info));
	ptr->load_list = NULL;
	
	Elf64_Ehdr* elfhdr;
	elfhdr = (Elf64_Ehdr*)parse_ptr;

	if(parse_ptr == NULL) {
		print_out(" ELF file pointer is null!!");
		return NULL;
	}


	ptr->file_start_ptr = (uint64_t)elfhdr;

	if(elfhdr->e_type == ET_EXEC) {
		//print_out("ELF file is a executable\n");
		print_out("");
		}
	else {
		print_out("\nELF file is not a executable!!");
		return NULL;
	}

	ptr->entry_point = elfhdr->e_entry;
	
	Elf64_Phdr* elf_phdr = (Elf64_Phdr *)(parse_ptr + (uint64_t)(elfhdr->e_phoff));
	
//------------------------------------------------------------------------------
//	print_out("\nProgram header info\n");
	
	int i = 0;
	
	for(i = 0; i < elfhdr->e_phnum; i++ ) {
		
		parse_ptr = (uint64_t) elf_phdr;

		// Type of segment...PT_LOAD.. 
		//print_out("\np_type is %d",elf_phdr->p_type);	
		switch(elf_phdr->p_type) {
	
			case PT_GNU_STACK: 	//print_out("\nSegment type is PT_GNU_STACK");
					   	break;

			case PT_LOAD:		//print_out("\nSegment type is PT_LOAD");
						break;
		}

		if(elf_phdr->p_type == PT_LOAD) {
			// 
			struct load_info *load_ptr = (struct load_info *)kern_malloc(j,sizeof(struct load_info));
			load_ptr->file_offset = elf_phdr->p_offset;
			load_ptr->v_addr = elf_phdr->p_vaddr;
			load_ptr->file_seg_size = elf_phdr->p_filesz;
			load_ptr->mem_seg_size = elf_phdr->p_memsz;
			load_ptr->flags = elf_phdr->p_flags;
			load_ptr->next = NULL;
			
			if(ptr->load_list == NULL)
				ptr->load_list = load_ptr;
			else
			{
				struct load_info* temp = ptr->load_list;
				while(temp->next != NULL) {
					temp = temp->next;	
				}
				temp->next = load_ptr;
				
			}
				
		}

#if 0
		// File offset for the first byte of segment
		print_out("\np_offset is %d bytes",elf_phdr->p_offset);	

		// Virtual address the first byte of segment in memory
		print_out("\np_vaddr is %p",elf_phdr->p_vaddr);	

		// Physical address of the segment
		print_out("\np_paddr is %p",elf_phdr->p_paddr);	


		// Number of bytes in the file image of segment
		print_out("\nSegment size in the file(p_filesz) is %d bytes",elf_phdr->p_filesz);	

		print_out("\nSegment size in memory(p_memsz) is %d bytes",elf_phdr->p_memsz);	

		print_out("\nPermission flags(rwx p_flags) is %x",elf_phdr->p_flags);	
		
#endif	
		elf_phdr = (Elf64_Phdr *)(parse_ptr +  (uint64_t)elfhdr->e_phentsize);
		
	}
	return ptr;
}
