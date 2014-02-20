#include "vm_area_struct.h"
#include <function.h>

extern uint64_t * kern_malloc(uint16_t, int);

uint64_t* create_vma(struct vm_area_struct* vma,uint64_t* vaddr, uint64_t size,uint16_t flags, uint16_t taskid) {
	//Check in the vm_area_struct if this memory is allocated..if not,
	// allocate and return
	
	int already_allocated = 0;
	struct vm_area_struct *new_vma;
	struct vm_area_struct *temp;

	uint64_t num_pages,remainder;

	if(vma == NULL)
		return NULL;
	
	temp = vma;

	if(temp->vma_start == temp->vma_end) {
		vma->vma_start = vaddr;
        	vma->vma_flags = flags;
        	vma->vma_size = size;

         	num_pages = size/4096;
                remainder = size - (num_pages * 4096);

                /* if remainder is zero, all pages are allocated
                   if remainder is > 0, then one more page has to be allocated
                   if remainder is < 0, one page is fragmented
                */
                if(remainder > 0) {
                        num_pages += 1;
                }

                //print_out(" total pages to be allocated is %d ",num_pages);

                /* Allocate num_pages that follow the vma flags and prot */
                // TODO -- allocate pages with prot flags

                vma->vma_end = (uint64_t *)((uint64_t)(vma->vma_start) + num_pages*4096);
                vma->vma_next = NULL;
		
		/****************************************************/
	/*
	temp = vma;
        while(temp != NULL) {
              //print_out("\n-------------------------\n");
              print_out(" vma_start is %x\n ",temp->vma_start);
              print_out(" vma_size is %d\n ",temp->vma_size);
              print_out(" vma_end is %x\n ",temp->vma_end);
              temp = temp->vma_next;
              print_out("\n-------------------------\n");
        }
	*/






		/****************************************************/
		return vma->vma_start;
	}
	//clear_screen();
	// Check if the address space is already allocated in one of the VMA's
	//	print_out(" vaddr is %x\n ",vaddr);	
	//	print_out(" vaddr+size is %x\n ",vaddr+size);	
	while(temp != NULL) {
		//check if vaddr and (vaddr+size) are either to left of allocated vma or to right
	//	print_out(" vma_start is %x\n ",temp->vma_start);	
	//	print_out(" vma_size is %d\n ",temp->vma_size);	
	//	print_out(" vma_end is %x\n ",temp->vma_end);	
		if(((vaddr < temp->vma_start) && (vaddr+size < temp->vma_start)) ||  (((vaddr >= temp->vma_end) )))	{
			already_allocated = 0;
		}
		else
			already_allocated = 1;

		if(already_allocated == 1)
			break;
		temp = temp->vma_next;
	}

	if(already_allocated == 1) {
		//print_out("Already allocated is 1\n");
		return NULL;
	}
	
	temp = vma;
	while(temp->vma_next != NULL)
		temp = temp->vma_next;

	new_vma = (struct vm_area_struct*)kern_malloc(taskid,sizeof(struct vm_area_struct));	
	temp->vma_next = new_vma;
	new_vma->vma_start = vaddr;
	new_vma->vma_flags = flags;
	new_vma->vma_size = size;

	 num_pages = size/4096;
                remainder = size - (num_pages * 4096);

                /* if remainder is zero, all pages are allocated
                   if remainder is > 0, then one more page has to be allocated
                   if remainder is < 0, one page is fragmented
                */
                if(remainder > 0) {
                        num_pages += 1;
                }

                //print_out(" total pages to be allocated is %d ",num_pages);

                /* Allocate num_pages that follow the vma flags and prot */
                // TODO -- allocate pages with prot flags

                new_vma->vma_end = (uint64_t *)((uint64_t)(new_vma->vma_start) + num_pages*4096);
                new_vma->vma_next = NULL;

	/*
	temp = vma;
	while(temp != NULL) {
	      print_out("\n-------------------------\n");
	      print_out(" vma_start is %x\n ",temp->vma_start);
              print_out(" vma_size is %d\n ",temp->vma_size);
              print_out(" vma_end is %x\n ",temp->vma_end);	
	      temp = temp->vma_next; 
	      print_out("\n-------------------------\n");
	}
	*/	
	return new_vma->vma_start;

}


uint64_t* vmalloc(struct vm_area_struct* vma, uint64_t size,  uint16_t flags, uint16_t taskid) {
	/* Move through VMALLOC_START to VMALLOC_END 
	*/	
	/* For the very first allocation */
	
	//uint64_t* mem_ptr = VMALLOC_START;
	uint64_t num_pages = 0;
	uint64_t remainder;
	struct vm_area_struct* temp;
	struct vm_area_struct* new_vma;

	if(((uint64_t)(vma->vma_end) + size) > VMALLOC_END) {
		print_out(" VMA request exceeds Virtual Mem limit!!! ");
		return NULL;
	}
	
	if(size <= 0) {
		print_out(" Cannot vmalloc %d bytes!!",size);
		return NULL;
	}
	
	/* If the vm_area_struct is the first one */
	if(vma->vma_start == vma->vma_end) {
		vma->vma_start = (uint64_t *)VMALLOC_START;
		vma->vma_size = size;
		vma->vma_flags = flags;

		num_pages = size/4096;
		remainder = size - (num_pages * 4096);
		
		/* if remainder is zero, all pages are allocated 
		   if remainder is > 0, then one more page has to be allocated
		   if remainder is < 0, one page is fragmented
		*/
		if(remainder > 0) {
			num_pages += 1;	
		}
		
		print_out(" total pages to be allocated is %d ",num_pages);	

		/* Allocate num_pages that follow the vma flags and prot */
		// TODO -- allocate pages with prot flags

		vma->vma_end = (uint64_t *)((uint64_t)(vma->vma_start) + num_pages*4096);
		vma->vma_next = NULL;	
		
		print_out("\n-- INIT ALLOC  vmalloc done--");
		print_out(" vma_start is %x ",vma->vma_start);	
		print_out(" vma_size is %d ",vma->vma_size);	
		print_out(" vma_end is %x ",vma->vma_end);	
		return vma->vma_start;
	}

	/*  Go to the end of the vma list and allocate a new */
	temp = vma;
	while(temp->vma_next != NULL) {
		temp = temp->vma_next;
	}
	
	new_vma = (struct vm_area_struct*)kern_malloc(taskid,sizeof(struct vm_area_struct));	
	
	new_vma->vma_start = temp->vma_end;
	new_vma->vma_size  = size;
	new_vma->vma_flags = flags;
	
		num_pages = size/4096;
		remainder = size - (num_pages * 4096);
		
		/* if remainder is zero, all pages are allocated 
		   if remainder is > 0, then one more page has to be allocated
		   if remainder is < 0, one page is fragmented
		*/
		if(remainder > 0) {
			num_pages += 1;	
		}
		
		print_out(" total pages to be allocated is %d ",num_pages);	

		/* Allocate num_pages that follow the vma flags and prot */
		// TODO -- allocate pages with prot flags

		new_vma->vma_end = (uint64_t *)((uint64_t)(new_vma->vma_start) + num_pages*4096);
		new_vma->vma_next = NULL;	
	
	new_vma->vma_next = NULL;
	
		print_out("\n-- vmalloc done--");

		print_out(" vma_start is %x ",new_vma->vma_start);	
		print_out(" vma_size is %d ",new_vma->vma_size);	
		print_out(" vma_end is %x ",new_vma->vma_end);	

	temp->vma_next = new_vma;
		
	return new_vma->vma_start;
}

