#include<function.h>
#include<defs.h>
#include<structs.h>

extern uint64_t * get_free_page_for_tasks();
extern struct task_struct task[10];
extern void free_page(uint64_t);

void map_user_pages_on_exit(uint16_t tid, uint16_t pid) {
	uint64_t * pml4e_ptr = task[pid].virtual_cr3;
	uint64_t * new_pml4e_ptr = task[tid].virtual_cr3;
	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;
	uint64_t * new_pdptr_table;
	uint64_t * new_pd_table;
	uint64_t * new_pg_table;
	uint64_t pml4e_offset=0;
	uint64_t pdptr_offset=0;
	uint64_t pd_offset=0;
	uint64_t pg_offset=0;
	uint64_t offset=0;
	uint64_t address=0;
	uint64_t free=0;
	uint64_t * freepage;

	for(pml4e_offset = 0;pml4e_offset<512;pml4e_offset++) {
		if(pml4e_ptr[pml4e_offset] != 0) {
			address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
                        address = address << 12;
                        address = address | (uint64_t)0xffffffff80000000;
                        pdptr_table = ((uint64_t *) address);			
			if(new_pml4e_ptr[pml4e_offset] == 0) {
				address = (uint64_t)get_free_page_for_tasks();
				//print_out(" Getting new page %x for child, at pml4e offset %d", address,pml4e_offset);
				new_pml4e_ptr[pml4e_offset] = (uint64_t)address | 7;
				address = address | (uint64_t)0xffffffff80000000;
				new_pdptr_table = ((uint64_t *) address);
				for(offset=0;offset<512;offset++)
					new_pdptr_table[offset] = 0;
			}
			else {
				address = ((uint64_t) new_pml4e_ptr[pml4e_offset]) >> 12;
                        	address = address << 12;
                        	address = address | (uint64_t)0xffffffff80000000;
                        	new_pdptr_table = ((uint64_t *) address);
			}
			for(pdptr_offset=0;pdptr_offset<512;pdptr_offset++) {
				if(pdptr_table[pdptr_offset] != 0) {
					address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
                        		address = address << 12;
                        		address = address | (uint64_t)0xffffffff80000000;
                        		pd_table = ((uint64_t *) address);	
					if(new_pdptr_table[pdptr_offset] == 0) {
						address = (uint64_t)get_free_page_for_tasks();
						//print_out(" Getting new page %x for child at pdptr offset %d", address,pdptr_offset);
						new_pdptr_table[pdptr_offset] = (uint64_t)address | 7;
                                		address = address | (uint64_t)0xffffffff80000000;
                                		new_pd_table = ((uint64_t *) address);
						for(offset=0;offset<512;offset++)
							new_pd_table[offset] = 0;
					}
					else {
						address = ((uint64_t) new_pdptr_table[pdptr_offset]) >> 12;
                                		address = address << 12;
                                		address = address | (uint64_t)0xffffffff80000000;
                                		new_pd_table = ((uint64_t *) address);
					}
					for(pd_offset = 0;pd_offset<512;pd_offset++) {
						if(pd_table[pd_offset] != 0) {
							address = ((uint64_t) pd_table[pd_offset]) >> 12;
                                        		address = address << 12;
                                        		address = address | (uint64_t)0xffffffff80000000;
                                        		pg_table = ((uint64_t *) address);
							if(new_pd_table[pd_offset] == 0) {
								address = (uint64_t)get_free_page_for_tasks();
								//print_out(" Getting new page %x for child at pdtable offset %d", address,pd_offset);
								new_pd_table[pd_offset] = (uint64_t)address | 7;
                                                		address = address | (uint64_t)0xffffffff80000000;
                                                		new_pg_table = ((uint64_t *) address);	
								for(offset=0;offset<512;offset++)
									new_pg_table[offset] = 0;
							}
							else {
								address = ((uint64_t) new_pd_table[pd_offset]) >> 12;
                                                		address = address << 12;
                                                		address = address | (uint64_t)0xffffffff80000000;
                                                		new_pg_table = ((uint64_t *) address);
							}
							for(pg_offset=0;pg_offset<512;pg_offset++) {
								if(pg_table[pg_offset] != 0) {
									address = ((uint64_t) pg_table[pg_offset]) >> 12;
                                                        		address = address << 12;
									if(address >= (uint64_t)0xb00000 && new_pg_table[pg_offset] != 0) {
										free = ((uint64_t)new_pg_table[pg_offset]) >> 12;
										free = free << 12;
										if(address == free) {
											freepage = get_free_page();
										        for(offset = 0;offset<512;offset++)
												freepage[offset] = 0;
											//for(offset=0;offset<512;offset++)
													
											print_out("\ngotto map page at %x for the user", address);	 
											print_out(" \nthe offsets are %d %d %d %d ",pml4e_offset,pdptr_offset,pd_offset,pg_offset);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//task[tid].virtual_cr3 = (uint64_t *)new_pml4e_ptr;
}
