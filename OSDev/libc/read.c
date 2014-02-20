#include<syscall.h>

int read(int fd, void * buf, uint64_t count) {
	uint64_t * param = (uint64_t *)0xfffffe0000000000;
	param[0] = (uint64_t)fd;
	param[1] = (uint64_t)buf;
	param[2] = (uint64_t)count;	
	__syscall3(1,(uint64_t)fd,(uint64_t)buf,(uint64_t)count);	
	return ((int)param[0]);
}
