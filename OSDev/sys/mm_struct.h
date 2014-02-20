#ifndef __MM_STRUCT_H
#define __MM_STRUCT_H

#include <defs.h>
#include "vm_area_struct.h"

struct mm_struct {
  int count;
  uint64_t pgd;  /* cr3 info */
  unsigned long context;
  unsigned long start_code, end_code, start_data, end_data;
  unsigned long start_brk, brk, start_stack, start_mmap;
  unsigned long arg_start, arg_end, env_start, env_end;
  unsigned long rss, total_vm, locked_vm; /* rss is the total pages mapped */
  unsigned long def_flags;
  struct vm_area_struct mmap;
};


#endif /* __MM_STRUCT_H */
