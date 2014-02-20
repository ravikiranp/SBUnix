#include<function.h>
#include<defs.h>
#include<structs.h>

extern uint64_t * get_free_page_for_tasks();
extern struct task_struct task[10];
extern void free_page(uint64_t);

void change_perm(uint64_t * base, int permission) {
	uint64_t * pml4e_ptr = base;
	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;
	uint64_t address;
	int pml4e_offset = 0;
	int pdptr_offset = 0;
	int pd_offset = 0;
	int pg_offset = 0;
	for(pml4e_offset = 0; pml4e_offset < 512; pml4e_offset++) {
		if(pml4e_ptr[pml4e_offset] != 0) {
			address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
                        address = address << 12;
                        address = address | (uint64_t)0xffffffff80000000;
                        pdptr_table = ((uint64_t *) address);			
			for(pdptr_offset = 0;pdptr_offset < 512; pdptr_offset++) {
				if(pdptr_table[pdptr_offset] != 0) {
					address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
                        		address = address << 12;
                        		address = address | (uint64_t)0xffffffff80000000;
                        		pd_table = ((uint64_t *) address);
					for(pd_offset = 0;pd_offset < 512; pd_offset++) {
						if(pd_table[pd_offset] != 0) {
							address = ((uint64_t) pd_table[pd_offset]) >> 12;
                                        		address = address << 12;
                                        		address = address | (uint64_t)0xffffffff80000000;
                                        		pg_table = ((uint64_t *) address);
							for(pg_offset=0; pg_offset<512; pg_offset++) {
								if(pg_table[pg_offset] != 0) {
									address = ((uint64_t) pg_table[pg_offset]) >> 3;
                                                        		address = address << 3;
									if(address >= 0xb00000) {	
										//print_out("IN-----the page address is %x",pg_table[pg_offset]);
                                                        			address = address | (uint64_t)permission;
                                                        			pg_table[pg_offset] = ((uint64_t) address);
										//print_out(" The pg_table contents is now %x  ",pg_table[pg_offset]);	
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
void map_user_pages_for_cow(uint16_t tid, uint16_t pid) {
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

	//uint64_t free;
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
									//if(address >= (uint64_t)0xb00000)
										//print_out("   ADDRESS IS %x   ",address);
									if(address >= (uint64_t)0xb00000 && new_pg_table[pg_offset] == 0) {
										//print_out("   IN-----Copying the address %x from parent to child   "); 
										new_pg_table[pg_offset] = address | (uint64_t)5;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	task[tid].virtual_cr3 = (uint64_t *)new_pml4e_ptr;
}

void change_parent_page_perm(uint64_t * pml4e_ptr,uint64_t vaddr,int permission) {
	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

	uint64_t pml4e_offset;
	uint64_t pdptr_offset;
	uint64_t pd_offset;
	uint64_t pg_offset;
	uint64_t address;

	pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_offset = ((0x000000001ff000 & vaddr) >> 12);

	address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
        address = address << 12;
        address = address | (uint64_t)0xffffffff80000000;
        pdptr_table = (uint64_t *)address;		

	address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
        address = address << 12;
        address = address | (uint64_t)0xffffffff80000000;
        pd_table = (uint64_t *)address; 

	address = ((uint64_t) pd_table[pd_offset]) >> 12;
        address = address << 12;
        address = address | (uint64_t)0xffffffff80000000;
        pg_table = (uint64_t *)address; 

	address = ((uint64_t) pg_table[pg_offset]) >> 12;
        address = address << 12;
	//print_out(" The contents of the parent pg_table was %x  ",pg_table[pg_offset]);
	pg_table[pg_offset] = (uint64_t)address | (uint64_t)permission;
	//print_out("  And it is now %x  ",pg_table[pg_offset]);
}

void create_new_page_for_child(uint64_t * pml4e_ptr,uint64_t vaddr,int permission) {
	uint64_t * pdptr_table;
        uint64_t * pd_table;
        uint64_t * pg_table;

        uint64_t pml4e_offset;
        uint64_t pdptr_offset;
        uint64_t pd_offset;
        uint64_t pg_offset;
	uint64_t address;

        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_offset = ((0x000000001ff000 & vaddr) >> 12);

	//print_out(" the offsets are %d %d %d %d  ",pml4e_offset,pdptr_offset,pd_offset,pg_offset);
	
	address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
        address = address << 12;
	//print_out(" the address of the pdptr_table is %x  ",address);
        address = address | (uint64_t)0xffffffff80000000;
        pdptr_table = (uint64_t *)address;
	
        address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
        address = address << 12;
	//print_out(" the address of the pd_table is %x  ",address);
        address = address | (uint64_t)0xffffffff80000000;
        pd_table = (uint64_t *)address;

        address = ((uint64_t) pd_table[pd_offset]) >> 12;
        address = address << 12;
	//print_out(" the address of the pd_table is %x  ",address);
        address = address | (uint64_t)0xffffffff80000000;
        pg_table = (uint64_t *)address;	

	uint64_t * free = get_free_page();
	//print_out("  the new page that os created is %x  ",free);
	uint64_t * temp_page = (uint64_t *)0xffffffff809ff000;
	uint64_t * parent_page = (uint64_t *)((uint64_t)vaddr & (uint64_t)0xfffffffffffff000); 
	print_out("  the page that will be read from is %x  ",parent_page);
	
	uint16_t offset;
	for(offset=0;offset<512;offset++) {
		temp_page[0] = parent_page[0];
	}
	
	//print_out(" the old contents of pg_table was %x  ",pg_table[pg_offset]);	
	pg_table[pg_offset] = (uint64_t)free | 7;
	//print_out(" the new contents of pg_table is %x  ",pg_table[pg_offset]);	
	uint64_t * child_page = (uint64_t *)((uint64_t)vaddr & (uint64_t)0xfffffffffffff000);
	//print_out(" the address of the new child page is %x  ",child_page);

	for(offset=0;offset<512;offset++) {
		child_page[0] = temp_page[0];	
	}
}

void clear_all_user_page_entries(uint64_t * pml4e_ptr) {
	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;
	uint64_t address;
	int pml4e_offset = 0;
	int pdptr_offset = 0;
	int pd_offset = 0;
	int pg_offset = 0;
	int pdptr_empty = 0;
	int pd_empty = 0;
	int pg_empty = 0;
	uint64_t pdptr_address;
	uint64_t pd_address;
	uint64_t pg_address;
	for(pml4e_offset = 0; pml4e_offset < 512; pml4e_offset++) {
		if(pml4e_ptr[pml4e_offset] != 0) {
			address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
                        address = address << 12;
                        address = address | (uint64_t)0xffffffff80000000;
                        pdptr_table = ((uint64_t *) address);			
			for(pdptr_offset = 0;pdptr_offset < 512; pdptr_offset++) {
				if(pdptr_table[pdptr_offset] != 0) {
					address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
                        		address = address << 12;
                        		address = address | (uint64_t)0xffffffff80000000;
                        		pd_table = ((uint64_t *) address);
					for(pd_offset = 0;pd_offset < 512; pd_offset++) {
						if(pd_table[pd_offset] != 0) {
							address = ((uint64_t) pd_table[pd_offset]) >> 12;
                                        		address = address << 12;
                                        		address = address | (uint64_t)0xffffffff80000000;
                                        		pg_table = ((uint64_t *) address);
							for(pg_offset=0; pg_offset<512; pg_offset++) {
								if(pg_table[pg_offset] != 0) {
									address = ((uint64_t) pg_table[pg_offset]) >> 3;
                                                        		address = address << 3;
									if(address >= 0xb00000) {	
                                                        			pg_table[pg_offset] = 0; 
									}
								}
							}
							pg_empty = 1;
							for(pg_offset=0;pg_offset<512;pg_offset++)
								if(pg_table[pg_offset] != 0) {
									pg_empty = 0;
									break;
								}	
							if(pg_empty == 1) {
								pg_address = ((uint64_t) pd_table[pd_offset]) >> 12;
                                                        	pg_address = pg_address << 12;
								free_page((uint64_t)pg_address);
							}
						}
					}
					pd_empty = 1;
					for(pd_offset=0;pd_offset<512;pd_offset++) 
						if(pd_table[pd_offset] != 0) {
							pd_empty = 0;
							break;
						}
					if(pd_empty == 1) {
						pd_address = ((uint64_t) pdptr_table[pdptr_offset]) >> 12;
                                                pd_address = pd_address << 12;
                                                free_page((uint64_t)pd_address);
					}
				}		
			}
			pdptr_empty = 1;
			for(pdptr_offset=0;pdptr_offset<512;pdptr_offset++)
				if(pdptr_table[pdptr_offset] != 0) {
					pdptr_empty = 0;
					break;
				}
			if(pdptr_empty == 1) {
				pdptr_address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
                        	pdptr_address = pdptr_address << 12;	
				free_page((uint64_t)pdptr_address);	
			}
		}
	}
}
