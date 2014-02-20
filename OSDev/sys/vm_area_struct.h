#ifndef __VM_AREA_STRUCT
#define __VM_AREA_STRUCT

#include <defs.h>

#define VMALLOC_START 	0x0000000000001000
#define VMALLOC_END	0x00007FFFFFFFFFFF

/* VMA Permission flags */
#define VMA_EXEC  0x0001
#define VMA_WRITE 0x0002
#define VMA_READ  0x0004


struct vm_area_struct{
        uint64_t* vma_start;            /* Start Address */
	uint64_t vma_size;		/* Size of vma */

        uint64_t* vma_end;              /* First Address after vma */
        struct vm_area_struct* vma_next;    /* Next vma_struct */
        unsigned char vma_prot;
        uint16_t vma_flags;             /* Flags */
};


#endif /* __VM_AREA_STRUCT */
