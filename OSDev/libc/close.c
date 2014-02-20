#include<syscall.h>

int close(int fd) {
	int * file_desc = (int *)0xfffffe0000000000;
	file_desc[0] = fd;
	__syscall1(7,(uint64_t)file_desc[0]);	
	return ((int)file_desc[0]);
}
