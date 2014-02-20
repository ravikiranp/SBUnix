#include<syscall.h>
uint16_t getpid() {
	//int ret_val = 0;
        __syscall0(2);
	uint16_t * getpid = (uint16_t *)0xfffffe0000000000;
        return getpid[0];
}
