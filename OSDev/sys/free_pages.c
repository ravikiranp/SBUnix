#include <defs.h>
#include<function.h>

char * free_bitmap = (char *)0xffffffff80400000;

void initialize_free_pages() {
	int no_of_pages = (total * 1048576) / 4096; 		
	//print_out(" The total number of pages required is %d",no_of_pages);
	uint16_t i;
	for(i = 0; i < no_of_pages; i++) {
		free_bitmap[i] = 1;
	}
	for(i=0; i<20; i++) {
		kmalloc_free_ptr[i] = 0;
		stack_free_ptr[i] = 0;
	}
	//print_out("    Address of free_bitmap is %p",free_bitmap); 
	/*
	uint32_t address = 0; 
	uint16_t page_no;
	for(address = 0;address < 0xa00000;) {
		page_no = address / 0x10000;
		free_bitmap[page_no] = 0;
		address += 0x1000;
	}
	*/
}
uint64_t * get_free_page() {
	uint64_t * page_addr = (uint64_t *) 0x00000;
	uint64_t temp = 0;
	int no_of_pages = (total * 1048576) / 4096, free_page = 0; 		
	//print_out(" the number of pages is %d and total is %d",no_of_pages,total);
	for(free_page = 2816; free_page < no_of_pages; free_page++) {
		if(free_bitmap[free_page] == 1) {
			temp = free_page * 0x1000;
			page_addr = (uint64_t *)temp;
			//print_out(" The first page that is free is %x and the free_page offset is at %d", page_addr,free_page);
			free_bitmap[free_page] = 0;
			break;
		}
	}
	return page_addr;
}	
uint64_t * get_free_page_for_paging() {
	uint64_t * page_addr = (uint64_t *) 0x00000;
	uint64_t temp = 0;
	//int no_of_pages = (total * 1048576) / 4096, free_page = 0; 		
	int free_page = 0;
	//print_out(" the number of pages is %d and total is %d",no_of_pages,total);
	for(free_page = 0; free_page < 1280; free_page++) {
		if(free_bitmap[free_page] == 1) {
			temp = free_page * 0x1000;
			page_addr = (uint64_t *)temp;
			//print_out(" The first page that is free is %x and the free_page offset is at %d", page_addr,free_page);
			free_bitmap[free_page] = 0;
			break;
		}
	}
	return page_addr;
}	

uint64_t * get_free_page_for_tasks() {
        uint64_t * page_addr = (uint64_t *) 0x00000;
        uint64_t temp = 0;
        //int no_of_pages = (total * 1048576) / 4096, free_page = 0;            
        int free_page = 0;
        //print_out(" the number of pages is %d and total is %d",no_of_pages,total);
        for(free_page = 1280; free_page < 2304; free_page++) {
                if(free_bitmap[free_page] == 1) {
                        temp = free_page * 0x1000;
                        page_addr = (uint64_t *)temp;
                        //print_out(" The first page that is free is %x and the free_page offset is at %d", page_addr,free_page);
                        free_bitmap[free_page] = 0;
                        break;
                }
        }
        return page_addr;
}

void free_page(uint64_t page) {
	int page_no = page / 4096;
	print_out("     the page that will be freed is %d     ", page_no);
	free_bitmap[page_no] = 1;
} 

uint64_t * get_free_page_for_stack() {
        uint64_t * page_addr = (uint64_t *) 0x00000;
        uint64_t temp = 0, vbase = (uint64_t)0xffffffff80000000;
        //int no_of_pages = (total * 1048576) / 4096, free_page = 0;
        int free_page = 0;
        //print_out(" the number of pages is %d and total is %d",no_of_pages,total);
        for(free_page = 2304; free_page < 2560; free_page++) {
                if(free_bitmap[free_page] == 1) {
                        temp = free_page * 0x1000;
                        temp = vbase | temp;
                        page_addr = (uint64_t *)temp;
                        //print_out(" The first page that is free is %x and the free_page offset is at %d", page_addr,free_page);
                        free_bitmap[free_page] = 0;
                        break;
                }
        }
        return page_addr;
}

uint64_t * get_free_kmalloc_page() {
        uint64_t * page_addr = (uint64_t *) 0x00000;
        uint64_t temp = 0, vbase = (uint64_t)0xffffffff80000000;
        //int no_of_pages = (total * 1048576) / 4096, free_page = 0;
	int free_page = 0;
        //print_out(" the number of pages is %d and total is %d",no_of_pages,total);
        for(free_page = 2560; free_page < 2816; free_page++) {
                if(free_bitmap[free_page] == 1) {
                        temp = free_page * 0x1000;
			temp = vbase | temp;
                        page_addr = (uint64_t *)temp;
                        //print_out(" The first page that is free is %x and the free_page offset is at %d", page_addr,free_page);
                        free_bitmap[free_page] = 0;
                        break;
                }
        }
        return page_addr;
}
void free_kmalloc_page(uint16_t pid) {
	uint64_t kmalloc_base = kmalloc_free_ptr[pid] & (uint64_t)0x0000000000fff000;
	kmalloc_base = kmalloc_base >> 12;
	//print_out("    The Kmalloc base is %d   ",kmalloc_base);
	kmalloc_free_ptr[pid] = 0;
	free_bitmap[kmalloc_base] = 1;	
	//int page_no = page / 4096;
	//print_out("     the page that will be freed is %d     ", page_no);
	//free_bitmap[page_no] = 1;
} 
void free_stack_page(uint16_t pid) {
        uint64_t stack_base = stack_free_ptr[pid] & (uint64_t)0x0000000000fff000;
        stack_base = stack_base >> 12;
        //print_out("    The Kmalloc base is %d   ",kmalloc_base);
        stack_free_ptr[pid] = 0;
        free_bitmap[stack_base] = 1;
        //int page_no = page / 4096;
        //print_out("     the page that will be freed is %d     ", page_no);
        //free_bitmap[page_no] = 1;
}
