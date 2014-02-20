#ifndef _VMA_STRUCT_H 
#define _VMA_STRUCT_H

#include <defs.h>

struct file;
struct mm_struct;

/* VMA Permission flags */
#define VMA_EXEC  0x0001
#define VMA_WRITE 0x0002
#define VMA_READ  0x0004


struct vma_struct{
	struct mm_struct *vm_mm;	/* Associated mm_struct */
	uint64_t* vma_start; 		/* Start Address */
	uint64_t* vma_end; 		/* First Address after vma */
	struct vma_struct* vma_next; 	/* Next vma_struct */
	unsigned char vma_prot;
	uint16_t vma_flags;		/* Flags */	
	struct file* vm_file; 		/* File we map to */		
};

#endif /* _VMA_STRUCT_H */
