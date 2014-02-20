#include<syscall.h>
void * malloc(uint64_t bytes) {
	//int ret_val = 0;
	uint64_t * vma_start = (uint64_t *)0xfffffe0000000000;
	vma_start[0] = (uint64_t)bytes;

        __syscall1(4,bytes);

	return (uint64_t *)vma_start[0];
}
