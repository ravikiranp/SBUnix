#include<syscall.h>

int open(const char * filename) {
	uint64_t * file_name = (uint64_t *)0xfffffe0000000000;
	file_name[0] = (uint64_t)filename;
	__syscall1(5,(uint64_t)filename);	
	return ((int)file_name[0]);
}
