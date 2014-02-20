#ifndef _TASK_STRUCT_H
#define _TASK_STRUCT_H

#include <defs.h>
#include "mm_struct.h"

struct task_struct {
	uint64_t stack[64];
	//struct mm_struct *mm;	
};

#endif /* _TASK_STRUCT_H */
