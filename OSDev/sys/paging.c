#include <defs.h>
#include<function.h>
#include<structs.h>
uint64_t * paging_struct_pointer = (uint64_t *)0x000000;
uint64_t * virtual_physfree;

extern uint64_t * get_free_page_for_tasks();
extern void free_page(uint64_t);
extern struct task_struct * current_task;
extern struct task_struct task[10];

void map_on_cow(int protection, int size) {

	/*	
	uint64_t phys_base = (uint64_t)0xffffffff80000000;
        uint64_t * addr = get_free_page_for_tasks();
        task[i].cr3 = (uint64_t *)addr;
        uint64_t temp = (uint64_t)0xffffffff80000000;
        temp = temp | (uint64_t)addr;
        uint64_t * vir_addr = (uint64_t *)temp;
        task[i].virtual_cr3 = (uint64_t *)vir_addr;
	*/

	//print_out("        MAPPING PHYS_BASE TO PHYS_FREE FOR USER        ");
	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
        uint16_t pg_table_offset;
	uint64_t address;
	uint64_t paddr = (uint64_t)0x000000;
	uint64_t vaddr = (uint64_t)0x0000000000000000;
	uint64_t phy_addr;

	uint64_t temp = (uint64_t)0xffffffff80000000, temp_page;
	uint64_t * pml4e_ptr;
	if (current_task->task_parent_id == 0) {
		pml4e_ptr = (uint64_t *)current_task->virtual_cr3;
	}
	else {
        	uint16_t ppid;
		ppid = current_task->task_parent_id;
		pml4e_ptr = task[ppid].virtual_cr3;
	}
			
	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;
	uint64_t * new_pml4e_ptr = get_free_page_for_tasks();;
	uint64_t * new_pdptr_table;
	uint64_t * new_pd_table;
	uint64_t * new_pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_table_offset = ((0x000000001ff000 & vaddr) >> 12);

	uint16_t offset = 0;
	for(pml4e_offset = 0; pml4e_offset < 512; pml4e_offset++) {
		if(pml4e_ptr[pml4e_offset] != 0) {
			new_pdptr_table = get_free_page_for_tasks(); 
			new_pml4e_ptr[pml4e_offset] = (uint64_t)new_pdptr_table | 7; 
			address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
                	address = address << 12;
                	address = address | (uint64_t)0xffffffff80000000;
                	pdptr_table = ((uint64_t *) address);
			for(pdptr_table_offset = 0;pdptr_table_offset < 512;pdptr_table_offset++) {
				if(pdptr_table[pdptr_table_offset] != 0) {
					new_pd_table = get_free_page_for_tasks();
					new_pdptr_table[pdptr_table_offset] = (uint64_t)new_pd_table | 7;
					address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                        		address = address << 12;
                        		address = address | (uint64_t)0xffffffff80000000;
                        		pd_table = ((uint64_t *) address);
					for(pd_table_offset = 0; pd_table_offset < 512; pd_table_offset++) {
						if(pd_table[pd_table_offset] != 0) {
							new_pg_table = get_free_page_for_tasks();
							new_pd_table[pd_table_offset] = (uint64_t)new_pg_table | 7;
							address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                                        		address = address << 12;
                                        		address = address | (uint64_t)0xffffffff80000000;
                                        		pg_table = ((uint64_t *) address);
							for(pg_table_offset = 0; pg_table_offset < 512; pg_table_offset++) {
								if(pg_table[pg_table_offset] != 0 && pml4e_offset == 511 && pdptr_table_offset == 510 && (pd_table_offset > 0 && pd_table_offset < 6)) {
									paddr = (uint64_t)get_free_page();
									new_pg_table[pg_table_offset] = (uint64_t)paddr| protection;
									address = ((uint64_t) pg_table[pg_table_offset]) >> 12;
                                                        		address = address << 12;
                                                        		address = address | (uint64_t)0xffffffff80000000;
									//vaddr = paddr | (uint64_t)0x00;
								}
							}	
						}
						else {
							new_pd_table[pd_table_offset] = 0;
						}
					}
				}
				else {
					new_pdptr_table[pdptr_table_offset] = 0;
				}	 
			}
		}
		else {
			new_pml4e_ptr[pml4e_offset] = 0;
		}
	}		
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	//pdptr_table = paging_struct_pointer;
        	pdptr_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pdptr_table;
		pdptr_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}		
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pdptr_table << 40;
		phy_addr = phy_addr >> 40;
		pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pdptr not mapped, allocating at %x and mapped to %d of pml4e",pdptr_table,pml4e_offset);
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
		address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pdptr_table = ((uint64_t *) address); 
		//print_out("   pdptr address already present, %x at offset of %d of pml4e",pdptr_table,pml4e_offset);
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		//pd_table = paging_struct_pointer;
		pd_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pd_table;
		pd_table = (uint64_t *)temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pd_table << 40;
		phy_addr = phy_addr >> 40;
        	pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pd_table not mapped, allocating at %x and mapped to %d of pdptr",pd_table,pdptr_table_offset);
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pd_table = ((uint64_t *) address);
		//print_out("   pd_table address already present, %x at offset of %d of pdptr",pd_table,pdptr_table_offset);
	}

	if (pd_table[pd_table_offset] == 0) {
		//pg_table = paging_struct_pointer;
		pg_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pg_table;
		pg_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pg_table << 40;
		phy_addr = phy_addr >> 40;
        	pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pg_table not mapped, allocating at %x and mapped to %d of pd_table",pg_table,pd_table_offset);
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pg_table = ((uint64_t *)address);
		//print_out("   pg_table address already present, %x at offset of %d of pd_table",pg_table,pd_table_offset);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
		if (pg_table[pg_table_offset] == 0 && pg_table_offset < 512) {
                	pg_table[pg_table_offset] = (address) | protection;
			//print_out("      Mapped the page %x at page table offset %d    ",address,pg_table_offset); 
                	address += 4096;
			pg_table_offset++;
		}
		else if (pg_table_offset < 512 && pg_table[pg_table_offset] != 0) {
			//print_out("     Page has already been mapped to this virtual address    The content of pg_table at offset %d is %x", pg_table_offset,pg_table[pg_table_offset]);
		}
		else {
			pg_table_offset = 0;
			//pg_table = paging_struct_pointer;
			pg_table = get_free_page_for_tasks();
			temp_page = temp | (uint64_t)pg_table;
			pg_table = (uint64_t *)temp_page;
			//paging_struct_pointer = paging_struct_pointer + 512;
			for (offset = 0; offset < 512; offset++) 
				pg_table[offset] = 0;
			//print_out("   The offset of pg_table exceeded 511, hence created new pg_table at %x     ",pg_table);
			pg_table[pg_table_offset] = (address) | protection;
			address += 4096;
			pg_table_offset++;
			pd_table_offset++;
			if (pd_table_offset > 511) {
				pd_table_offset = 0;
				//pd_table = paging_struct_pointer;
				pd_table = get_free_page_for_tasks();
				temp_page = temp | (uint64_t) pd_table;
				pd_table = (uint64_t *)temp_page;
				//paging_struct_pointer = paging_struct_pointer + 512;
				for (offset = 0; offset < 152; offset++)
					pd_table[offset] = 0;
				//print_out("   The offset of pd_table exceeded 511, hence created new pd_table at %x     ",pd_table);
				phy_addr = (uint64_t)pg_table << 40;
				phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				pdptr_table_offset++;
				if (pdptr_table_offset > 511) {
					pdptr_table_offset = 0;
					//pdptr_table = paging_struct_pointer;
					pdptr_table = get_free_page_for_tasks();
					temp_page = temp | (uint64_t) pdptr_table;
					pdptr_table = (uint64_t *) temp_page;
					//paging_struct_pointer = paging_struct_pointer + 512;
					for (offset = 0; offset < 512; offset++)
						pdptr_table[offset] = 0;
					//print_out("   The offset of pdptr_table exceeded 511, hence created new pdptr_table at %x     ",pdptr_table);
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					pml4e_offset++;
					phy_addr = (uint64_t)pdptr_table << 40;
					phy_addr = phy_addr >> 40;
					pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pdptr_table mapped at %d of pml4e",pml4e_offset);
				}	
				else {
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pd_table mapped at %d of pdptr_table",pdptr_table_offset);
				}	
			}
			else {
				phy_addr = (uint64_t)pg_table << 40;
                                phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				//print_out("new pg_table mapped at %d of pd_table",pd_table_offset);
			}
		}
		//print_out("%d", no_of_pages);
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x            The contents of pd_table at offset %d is %x           ", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset], pd_table_offset, pd_table[pd_table_offset]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  in user space     ",size,paddr,vaddr);
}

void map_user_page_virtual(uint64_t * pml4e_ptr, uint64_t vaddr, uint64_t paddr, uint16_t size, int protection) {
	//print_out("        MAPPING PHYS_BASE TO PHYS_FREE FOR USER        ");
	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
        uint16_t pg_table_offset;
	uint64_t address;
	uint64_t phy_addr;

	uint64_t temp = (uint64_t)0xffffffff80000000, temp_page;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_table_offset = ((0x000000001ff000 & vaddr) >> 12);

	phy_addr = (uint64_t)pml4e_ptr << 40;
   	phy_addr = phy_addr >> 40;
       	pml4e_ptr[510] = ((uint64_t)phy_addr) | 7;

	uint16_t offset = 0;
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	//pdptr_table = paging_struct_pointer;
        	pdptr_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pdptr_table;
		pdptr_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}		
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pdptr_table << 40;
		phy_addr = phy_addr >> 40;
		pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pdptr not mapped, allocating at %x and mapped to %d of pml4e",pdptr_table,pml4e_offset);
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
		address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pdptr_table = ((uint64_t *) address); 
		//print_out("   pdptr address already present, %x at offset of %d of pml4e",pdptr_table,pml4e_offset);
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		//pd_table = paging_struct_pointer;
		pd_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pd_table;
		pd_table = (uint64_t *)temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pd_table << 40;
		phy_addr = phy_addr >> 40;
        	pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pd_table not mapped, allocating at %x and mapped to %d of pdptr",pd_table,pdptr_table_offset);
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pd_table = ((uint64_t *) address);
		//print_out("   pd_table address already present, %x at offset of %d of pdptr",pd_table,pdptr_table_offset);
	}

	if (pd_table[pd_table_offset] == 0) {
		//pg_table = paging_struct_pointer;
		pg_table = get_free_page_for_tasks();
		temp_page = temp | (uint64_t)pg_table;
		pg_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pg_table << 40;
		phy_addr = phy_addr >> 40;
        	pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pg_table not mapped, allocating at %x and mapped to %d of pd_table",pg_table,pd_table_offset);
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pg_table = ((uint64_t *)address);
		//print_out("   pg_table address already present, %x at offset of %d of pd_table",pg_table,pd_table_offset);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
		if (pg_table[pg_table_offset] == 0 && pg_table_offset < 512) {
                	pg_table[pg_table_offset] = (address) | protection;
			//print_out("      Mapped the page %x at page table offset %d    ",address,pg_table_offset); 
                	address += 4096;
			pg_table_offset++;
		}
		else if (pg_table_offset < 512 && pg_table[pg_table_offset] != 0) {
			//print_out("     Page has already been mapped to this virtual address    The content of pg_table at offset %d is %x", pg_table_offset,pg_table[pg_table_offset]);
		}
		else {
			pg_table_offset = 0;
			//pg_table = paging_struct_pointer;
			pg_table = get_free_page_for_tasks();
			temp_page = temp | (uint64_t)pg_table;
			pg_table = (uint64_t *)temp_page;
			//paging_struct_pointer = paging_struct_pointer + 512;
			for (offset = 0; offset < 512; offset++) 
				pg_table[offset] = 0;
			//print_out("   The offset of pg_table exceeded 511, hence created new pg_table at %x     ",pg_table);
			pg_table[pg_table_offset] = (address) | protection;
			address += 4096;
			pg_table_offset++;
			pd_table_offset++;
			if (pd_table_offset > 511) {
				pd_table_offset = 0;
				//pd_table = paging_struct_pointer;
				pd_table = get_free_page_for_tasks();
				temp_page = temp | (uint64_t) pd_table;
				pd_table = (uint64_t *)temp_page;
				//paging_struct_pointer = paging_struct_pointer + 512;
				for (offset = 0; offset < 152; offset++)
					pd_table[offset] = 0;
				//print_out("   The offset of pd_table exceeded 511, hence created new pd_table at %x     ",pd_table);
				phy_addr = (uint64_t)pg_table << 40;
				phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				pdptr_table_offset++;
				if (pdptr_table_offset > 511) {
					pdptr_table_offset = 0;
					//pdptr_table = paging_struct_pointer;
					pdptr_table = get_free_page_for_tasks();
					temp_page = temp | (uint64_t) pdptr_table;
					pdptr_table = (uint64_t *) temp_page;
					//paging_struct_pointer = paging_struct_pointer + 512;
					for (offset = 0; offset < 512; offset++)
						pdptr_table[offset] = 0;
					//print_out("   The offset of pdptr_table exceeded 511, hence created new pdptr_table at %x     ",pdptr_table);
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					pml4e_offset++;
					phy_addr = (uint64_t)pdptr_table << 40;
					phy_addr = phy_addr >> 40;
					pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pdptr_table mapped at %d of pml4e",pml4e_offset);
				}	
				else {
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pd_table mapped at %d of pdptr_table",pdptr_table_offset);
				}	
			}
			else {
				phy_addr = (uint64_t)pg_table << 40;
                                phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				//print_out("new pg_table mapped at %d of pd_table",pd_table_offset);
			}
		}
		//print_out("%d", no_of_pages);
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x            The contents of pd_table at offset %d is %x           ", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset], pd_table_offset, pd_table[pd_table_offset]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  in user space     ",size,paddr,vaddr);
	//print_out(" In map_user_virtual_page...the 510th entry is %x and the address of pml4e is %x   ",pml4e_ptr[510],pml4e_ptr);
}

void mapaddr_2MB(uint64_t * pml4e_ptr, uint64_t vaddr, uint64_t paddr, uint16_t size) {

	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
	uint64_t address;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);

	uint16_t offset = 0;
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	pdptr_table = paging_struct_pointer;
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}		
		paging_struct_pointer = paging_struct_pointer + 512;
		pml4e_ptr[pml4e_offset] = ((uint64_t)pdptr_table) | 7;
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 3;
		address = address << 3;
		pdptr_table = ((uint64_t *) address); 
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		pd_table = paging_struct_pointer;
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		paging_struct_pointer = paging_struct_pointer + 512;
        	pdptr_table[pdptr_table_offset] = ((uint64_t) pd_table) | 7;
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 3;
                address = address << 3;
		pd_table = ((uint64_t *) address);
	}

	if (pd_table[pd_table_offset] == 0) {
		pg_table = paging_struct_pointer;
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		paging_struct_pointer = paging_struct_pointer + 512;
        	pd_table[pd_table_offset] = ((uint64_t)pg_table) | 7;
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 3;
                address = address << 3;
		pg_table = ((uint64_t *)address);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
        	for(offset = 0; offset < 512; offset++) {
                	pg_table[offset] = (address) | 7;
                	address += 4096;
		}
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x             The contents of pd_table at offset %d is %x            The contents of pd_table at offset %d is %x", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset],pd_table_offset,pd_table[pd_table_offset], pd_table_offset+1, pd_table[pd_table_offset+1]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  Yeah!!!!! ****",size,paddr,vaddr);
}

void map_page(uint64_t * pml4e_ptr, uint64_t vaddr, uint64_t paddr, uint16_t size, int protection) {

	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
        uint16_t pg_table_offset;
	uint64_t address;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_table_offset = ((0x000000001ff000 & vaddr) >> 12);

	uint16_t offset = 0;
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	//pdptr_table = paging_struct_pointer;
        	pdptr_table = get_free_page_for_paging();
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}		
		//paging_struct_pointer = paging_struct_pointer + 512;
		pml4e_ptr[pml4e_offset] = ((uint64_t)pdptr_table) | 7;
		//print_out("   pdptr not mapped, allocating at %x and mapped to %d of pml4e",pdptr_table,pml4e_offset);
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 3;
		address = address << 3;
		pdptr_table = ((uint64_t *) address); 
		//print_out("   pdptr address already present, %x at offset of %d of pml4e",pdptr_table,pml4e_offset);
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		//pd_table = paging_struct_pointer;
		pd_table = get_free_page_for_paging();
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		//paging_struct_pointer = paging_struct_pointer + 512;
        	pdptr_table[pdptr_table_offset] = ((uint64_t) pd_table) | 7;
		//print_out("   pd_table not mapped, allocating at %x and mapped to %d of pdptr",pd_table,pdptr_table_offset);
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 3;
                address = address << 3;
		pd_table = ((uint64_t *) address);
		//print_out("   pd_table address already present, %x at offset of %d of pdptr",pd_table,pdptr_table_offset);
	}

	if (pd_table[pd_table_offset] == 0) {
		//pg_table = paging_struct_pointer;
		pg_table = get_free_page_for_paging();
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		//paging_struct_pointer = paging_struct_pointer + 512;
        	pd_table[pd_table_offset] = ((uint64_t)pg_table) | 7;
		//print_out("   pg_table not mapped, allocating at %x and mapped to %d of pd_table",pg_table,pd_table_offset);
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 3;
                address = address << 3;
		pg_table = ((uint64_t *)address);
		//print_out("   pg_table address already present, %x at offset of %d of pd_table",pg_table,pd_table_offset);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
		if (pg_table[pg_table_offset] == 0 && pg_table_offset < 512) {
                	pg_table[pg_table_offset] = (address) | protection;
			//print_out("      Mapped the page %x at page table offset %d    ",address,pg_table_offset); 
                	address += 4096;
			pg_table_offset++;
		}
		else if (pg_table_offset < 512 && pg_table[pg_table_offset] != 0) {
			//print_out("     Page has already been mapped to this virtual address    The content of pg_table at offset %d is %x", pg_table_offset,pg_table[pg_table_offset]);
		}
		else {
			pg_table_offset = 0;
			//pg_table = paging_struct_pointer;
			pg_table = get_free_page_for_paging();
			//paging_struct_pointer = paging_struct_pointer + 512;
			for (offset = 0; offset < 512; offset++) 
				pg_table[offset] = 0;
			//print_out("   The offset of pg_table exceeded 511, hence created new pg_table at %x     ",pg_table);
			pg_table[pg_table_offset] = (address) | protection;
			address += 4096;
			pg_table_offset++;
			pd_table_offset++;
			if (pd_table_offset > 511) {
				pd_table_offset = 0;
				//pd_table = paging_struct_pointer;
				pd_table = get_free_page_for_paging();
				//paging_struct_pointer = paging_struct_pointer + 512;
				for (offset = 0; offset < 152; offset++)
					pd_table[offset] = 0;
				//print_out("   The offset of pd_table exceeded 511, hence created new pd_table at %x     ",pd_table);
				pd_table[pd_table_offset] = ((uint64_t)pg_table) | 7;
				pdptr_table_offset++;
				if (pdptr_table_offset > 511) {
					pdptr_table_offset = 0;
					//pdptr_table = paging_struct_pointer;
					pdptr_table = get_free_page_for_paging();
					//paging_struct_pointer = paging_struct_pointer + 512;
					for (offset = 0; offset < 512; offset++)
						pdptr_table[offset] = 0;
					//print_out("   The offset of pdptr_table exceeded 511, hence created new pdptr_table at %x     ",pdptr_table);
					pdptr_table[pdptr_table_offset] = ((uint64_t)pd_table) | 7;
					pml4e_offset++;
					pml4e_ptr[pml4e_offset] = ((uint64_t)pdptr_table) | 7;
					//print_out("new pdptr_table mapped at %d of pml4e",pml4e_offset);
				}	
				else {
					pdptr_table[pdptr_table_offset] = ((uint64_t)pd_table) | 7;
					//print_out("new pd_table mapped at %d of pdptr_table",pdptr_table_offset);
				}	
			}
			else {
				pd_table[pd_table_offset] = ((uint64_t)pg_table) | 7;
				//print_out("new pg_table mapped at %d of pd_table",pd_table_offset);
			}
		}
		//print_out("%d", no_of_pages);
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x            The contents of pd_table at offset %d is %x           The contents of pd_table at offset %d is %x", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset], pd_table_offset, pd_table[pd_table_offset],pg_table_offset-1,pg_table[pg_table_offset-1]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  Yeah!!!!! ****",size,paddr,vaddr);
}

void mapaddr_2MB_virtual(uint64_t * pml4e_ptr, uint64_t vaddr, uint64_t paddr, uint16_t size) {

	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
	uint64_t address;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);

	print_out("        The pml4e, pdptr and pdtable offsets are %d  %d and %d         ",pml4e_offset, pdptr_table_offset,pd_table_offset);
	uint16_t offset = 0;
	uint64_t phy_addr;
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	pdptr_table = paging_struct_pointer;
		print_out("%%%%% pdptr will be assigned this address %p",paging_struct_pointer);
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}	
		paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pdptr_table << 40;
		//phy_addr = (uint64_t)paging_struct_pointer << 40;
		phy_addr = phy_addr >>40;
		print_out("%%%%% the phy_addr is %x",phy_addr);
			
		pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
		print_out("the address of pdptr_table is %p and the contents of the pml4e at offset %d is %x",pdptr_table,pml4e_offset,pml4e_ptr[pml4e_offset]);
		print_out("%%%%% the pml4e_offset is %d and contents of pml4e is %x",pml4e_offset,pml4e_ptr[pml4e_offset]);
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
		//print_out("&&&&&&&  address is %x &&&&&&&",address); 
		address = address << 12;
		//print_out("&&&&&&&  address is %x &&&&&&&",address); 
		address = address | (uint64_t)0xffffffff80000000;
		print_out("%%%%% pml4e is alreday mapped, the contents at offset %d are %x, the address of pdptr is %x",pml4e_offset,pml4e_ptr[pml4e_offset],address);
		pdptr_table = ((uint64_t *) address); 
		//print_out("     the address of pdptr is %p   ",pdptr_table);
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		print_out("%%%%% pd_table will be assigned this address %p",paging_struct_pointer);
		pd_table = paging_struct_pointer;
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pd_table << 40;
		phy_addr = phy_addr >> 40;
		print_out("%%%%% the phy_addr is %x",phy_addr);

        	pdptr_table[pdptr_table_offset] = ((uint64_t) phy_addr) | 7;
		print_out("the address of pd_table is %p and the contents of the pdptr at offset %d is %x",pd_table,pdptr_table_offset,pdptr_table[pdptr_table_offset]);
		print_out("%%%%% the pdptr_table_offset is %d and contents of pdptr is %x",pdptr_table_offset,pdptr_table[pdptr_table_offset]);
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		print_out("%%%%% pdptr is alreday mapped, the address of pdtable is %x",address);
		pd_table = ((uint64_t *) address);
	}

	if (pd_table[pd_table_offset] == 0) {
		print_out("%%%%% ********pg_table will be assigned this address %p",paging_struct_pointer);
		pg_table = paging_struct_pointer;
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pg_table << 40;
		phy_addr = phy_addr >> 40;
		print_out("%%%%% the phy_addr is %x",phy_addr);

        	pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
		print_out("the address of pg_table is %p and the contents of the pd_table at offset %d is %x",pg_table,pd_table_offset,pd_table[pd_table_offset]);
		print_out("%%%%% the pd_table_offset is %d and contents of pd_table is %x",pd_table_offset,pd_table[pd_table_offset]);
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		print_out("%%%%% pd_table is alreday mapped, the address of pg_table is %x",address);
		pg_table = ((uint64_t *)address);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
        	for(offset = 0; offset < 512; offset++) {
                	pg_table[offset] = (address) | 7;
                	address += 4096;
		}
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x             The contents of pd_table at offset %d is %x", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset],pd_table_offset,pd_table[pd_table_offset]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  Yeah!!!!! ****",size,paddr,vaddr);
	//for (offset = 0; offset<15; offset++) {
	//	print_out("****The contents of pd_table at offset %d is %x****",offset,pd_table[offset]);
	//}
}

void map_page_virtual(uint64_t * pml4e_ptr, uint64_t vaddr, uint64_t paddr, uint16_t size, int protection) {

	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
        uint16_t pg_table_offset;
	uint64_t address;
	uint64_t phy_addr;

	uint64_t temp = (uint64_t)0xffffffff80000000, temp_page;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_table_offset = ((0x000000001ff000 & vaddr) >> 12);

	uint16_t offset = 0;
	if (pml4e_ptr[pml4e_offset] == 0) { 
        	//pdptr_table = paging_struct_pointer;
        	pdptr_table = get_free_page_for_paging();
		temp_page = temp | (uint64_t)pdptr_table;
		pdptr_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                	pdptr_table[offset] = 0;
        	}		
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pdptr_table << 40;
		phy_addr = phy_addr >> 40;
		pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pdptr not mapped, allocating at %x and mapped to %d of pml4e",pdptr_table,pml4e_offset);
	}
	else {
		address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
		address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pdptr_table = ((uint64_t *) address); 
		//print_out("   pdptr address already present, %x at offset of %d of pml4e",pdptr_table,pml4e_offset);
	}
		
	if (pdptr_table[pdptr_table_offset] == 0) {
		//pd_table = paging_struct_pointer;
		pd_table = get_free_page_for_paging();
		temp_page = temp | (uint64_t)pd_table;
		pd_table = (uint64_t *)temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pd_table[offset] = 0;
                }	
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pd_table << 40;
		phy_addr = phy_addr >> 40;
        	pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pd_table not mapped, allocating at %x and mapped to %d of pdptr",pd_table,pdptr_table_offset);
	}
	else {
		address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pd_table = ((uint64_t *) address);
		//print_out("   pd_table address already present, %x at offset of %d of pdptr",pd_table,pdptr_table_offset);
	}

	if (pd_table[pd_table_offset] == 0) {
		//pg_table = paging_struct_pointer;
		pg_table = get_free_page_for_paging();
		temp_page = temp | (uint64_t)pg_table;
		pg_table = (uint64_t *) temp_page;
		for(offset = 0; offset < 512; offset++) {
                        pg_table[offset] = 0;
                }
		//paging_struct_pointer = paging_struct_pointer + 512;
		phy_addr = (uint64_t)pg_table << 40;
		phy_addr = phy_addr >> 40;
        	pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
		//print_out("   pg_table not mapped, allocating at %x and mapped to %d of pd_table",pg_table,pd_table_offset);
	}
	else {
		address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                address = address << 12;
		address = address | (uint64_t)0xffffffff80000000;
		pg_table = ((uint64_t *)address);
		//print_out("   pg_table address already present, %x at offset of %d of pd_table",pg_table,pd_table_offset);
	}

        address = paddr;
	uint16_t no_of_pages;
	for(no_of_pages = 0; no_of_pages < size; no_of_pages++) {
		if (pg_table[pg_table_offset] == 0 && pg_table_offset < 512) {
                	pg_table[pg_table_offset] = (address) | protection;
			//print_out("      Mapped the page %x at page table offset %d    ",address,pg_table_offset); 
                	address += 4096;
			pg_table_offset++;
		}
		else if (pg_table_offset < 512 && pg_table[pg_table_offset] != 0) {
			//print_out("     Page has already been mapped to this virtual address    The content of pg_table at offset %d is %x", pg_table_offset,pg_table[pg_table_offset]);
		}
		else {
			pg_table_offset = 0;
			//pg_table = paging_struct_pointer;
			pg_table = get_free_page_for_paging();
			temp_page = temp | (uint64_t)pg_table;
			pg_table = (uint64_t *)temp_page;
			//paging_struct_pointer = paging_struct_pointer + 512;
			for (offset = 0; offset < 512; offset++) 
				pg_table[offset] = 0;
			//print_out("   The offset of pg_table exceeded 511, hence created new pg_table at %x     ",pg_table);
			pg_table[pg_table_offset] = (address) | protection;
			address += 4096;
			pg_table_offset++;
			pd_table_offset++;
			if (pd_table_offset > 511) {
				pd_table_offset = 0;
				//pd_table = paging_struct_pointer;
				pd_table = get_free_page_for_paging();
				temp_page = temp | (uint64_t) pd_table;
				pd_table = (uint64_t *)temp_page;
				//paging_struct_pointer = paging_struct_pointer + 512;
				for (offset = 0; offset < 152; offset++)
					pd_table[offset] = 0;
				//print_out("   The offset of pd_table exceeded 511, hence created new pd_table at %x     ",pd_table);
				phy_addr = (uint64_t)pg_table << 40;
				phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				pdptr_table_offset++;
				if (pdptr_table_offset > 511) {
					pdptr_table_offset = 0;
					//pdptr_table = paging_struct_pointer;
					pdptr_table = get_free_page_for_paging();
					temp_page = temp | (uint64_t) pdptr_table;
					pdptr_table = (uint64_t *) temp_page;
					//paging_struct_pointer = paging_struct_pointer + 512;
					for (offset = 0; offset < 512; offset++)
						pdptr_table[offset] = 0;
					//print_out("   The offset of pdptr_table exceeded 511, hence created new pdptr_table at %x     ",pdptr_table);
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					pml4e_offset++;
					phy_addr = (uint64_t)pdptr_table << 40;
					phy_addr = phy_addr >> 40;
					pml4e_ptr[pml4e_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pdptr_table mapped at %d of pml4e",pml4e_offset);
				}	
				else {
					phy_addr = (uint64_t)pd_table << 40;
					phy_addr = phy_addr >> 40;
					pdptr_table[pdptr_table_offset] = ((uint64_t)phy_addr) | 7;
					//print_out("new pd_table mapped at %d of pdptr_table",pdptr_table_offset);
				}	
			}
			else {
				phy_addr = (uint64_t)pg_table << 40;
                                phy_addr = phy_addr >> 40;
				pd_table[pd_table_offset] = ((uint64_t)phy_addr) | 7;
				//print_out("new pg_table mapped at %d of pd_table",pd_table_offset);
			}
		}
		//print_out("%d", no_of_pages);
        }

        //print_out("  the address of pml4e is %p   the address of pdptr_table is %p     the address of pd_table is %p      the address of pg_table is %p   ", pml4e_ptr, pdptr_table,pd_table,pg_table);
        //print_out("  The contents of pml4e at offset %d is %x      The contents of pdptr at offset %d is %x            The contents of pd_table at offset %d is %x           ", pml4e_offset, pml4e_ptr[pml4e_offset],pdptr_table_offset,pdptr_table[pdptr_table_offset], pd_table_offset, pd_table[pd_table_offset]);
	//print_out("     The contents of pml4e at offset 511 is %x     ",pml4e_ptr[511]);
	//print_out("    Mapped %d page/s starting from phy addr %x to virtual addr %x  Yeah!!!!! ****",size,paddr,vaddr);
}

void free_virtual_page(uint64_t * pml4e_ptr, uint64_t vaddr, uint16_t size) {

	uint16_t pml4e_offset;
        uint16_t pdptr_table_offset;
        uint16_t pd_table_offset;
        uint16_t pg_table_offset;
	uint64_t address;

	uint64_t * pdptr_table;
	uint64_t * pd_table;
	uint64_t * pg_table;

        /* Map the kernel to a high region in memory */
        pml4e_offset = ((0x0000ff8000000000 & vaddr) >> 39);
        pdptr_table_offset = ((0x0000007f80000000 & vaddr) >> 30);
        pd_table_offset = ((0x000000003fe00000 & vaddr) >> 21);
        pg_table_offset = ((0x000000001ff000 & vaddr) >> 12);

	uint16_t offset = 0;
	for(offset=0;offset < size; offset++) {
		if (pml4e_ptr[pml4e_offset] == 0) { 
			print_out("     No mapping exists for this virtual address     ");
		}
		else {
			address = ((uint64_t) pml4e_ptr[pml4e_offset]) >> 12;
			address = address << 12;
			address = address | (uint64_t)0xffffffff80000000;
			pdptr_table = ((uint64_t *) address); 
			//print_out("     Clearing the %d offset of pml4e_table and moving to pdptr_table at %x     ",pml4e_offset,pdptr_table);
			//pml4e_ptr[pml4e_offset] = 0;

			if (pdptr_table[pdptr_table_offset] == 0) {
				print_out("     No mapping exists for this virtual address     ");
			}
			else {
				address = ((uint64_t) pdptr_table[pdptr_table_offset]) >> 12;
                		address = address << 12;
				address = address | (uint64_t)0xffffffff80000000;
				pd_table = ((uint64_t *) address);
				//print_out("     Clearing the %d offset of pdptr_table and moving to pd_table at %x     ",pdptr_table_offset,pd_table);
				//pdptr_table[pdptr_table_offset] = 0;

				if (pd_table[pd_table_offset] == 0) {
					print_out("     No mapping exists for this virtual address     ");
				}
				else {
					address = ((uint64_t) pd_table[pd_table_offset]) >> 12;
                			address = address << 12;
					address = address | (uint64_t)0xffffffff80000000;
					pg_table = ((uint64_t *)address);
					//print_out("     Clearing the %d offset of pd_table and moving to pg_table at %x     ",pd_table_offset,pg_table);
					//pd_table[pd_table_offset] = 0;
	
					if (pg_table[pg_table_offset] == 0) {
						print_out("     No mapping exists for this virtual address     ");
					}
					else {
						address = ((uint64_t) pg_table[pg_table_offset]) >> 12;
                                        	address = address << 12;
						print_out("     Clearing the %d offset of pg_table        ",pg_table_offset);
						pg_table[pg_table_offset] = 0;
						free_page(address);
					}
				}
			}
		}
		vaddr = vaddr + (uint64_t)0x1000;	
	}		 
}

void create_page_tables(void* mem_free) {
        /* pml4e table at freemem */
        uint64_t *pml4e_ptr = (uint64_t *) mem_free;

	uint16_t offset = 0;
	for(offset = 0; offset < 512; offset++) {
                pml4e_ptr[offset] = 0;
        }	
	
	paging_struct_pointer = pml4e_ptr + 512;
	map_page((uint64_t *)mem_free,0xffffffff80000000,0x000000,256,7);
	map_page((uint64_t *)mem_free,0xffffffff80100000,0x100000,2048,5);
	map_page((uint64_t *)mem_free,0xffffffff80900000,0x900000,512,7);
	//map_page((uint64_t *)mem_free,0xffffffff80a00000,0xa00000,256,7);


	/* 510th entry of the pml4e table is used for self referencing */
	pml4e_ptr[510] = ((uint64_t)pml4e_ptr) | 7;

	__asm__ volatile("movq %0,%%cr3"
			:
			//: "b" ((uint64_t *)pml4e_ptr)				
			: "b" ((uint64_t *)pml4e_ptr)				
		); 
	
	uint64_t temp = (uint64_t)0xffffffff80000000;
	uint64_t temp_page = (uint64_t) 0xffffffff80000000;
	temp = temp | (uint64_t)mem_free;
	virtual_physfree = (uint64_t *)temp;
	temp = temp_page | (uint64_t)paging_struct_pointer;
	paging_struct_pointer = (uint64_t *)temp;
	//print_out("**************the 511 offset of pml4e_ptr is %x**************",virtual_physfree[511]);
	
	//map_page_ref((uint64_t *)virtual_physfree, 0xffffffff80e00000, 0xe000, 1);
/*	mapaddr_2MB_virtual((uint64_t *)virtual_physfree,0xffffffff80a00000,0xa00000,1);
	
	asm volatile("movq %0,%%cr3"
                        :
                        : "b" ((uint64_t *)mem_free)
                );
	
	print_out("      Calling map_page_virtual      ");

	map_page_virtual((uint64_t *)virtual_physfree,0xffffffff80a00000,0xa00000,1);

	asm volatile("movq %0,%%cr3"
                        :
                        : "b" ((uint64_t *)mem_free)
                );
	char * try = (char *) 0xffffffff80a00000;
	try[0] = 'x';
	print_out("     Haha....%c",try[0]);
*/
}
