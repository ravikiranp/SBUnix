#include<syscall.h>
void sleep(uint16_t sec) {
	uint16_t * sleep_sec = (uint16_t *)0xfffffe0000000000;
        sleep_sec[0] = (uint16_t)sec;
	
        __syscall1(1,(uint64_t)sec);

	uint64_t * sleep_wait = (uint64_t *)0xfffffe0000000000;
        sleep_wait[0] = 10000000;
        while(sleep_wait[0] > 0)
                sleep_wait[0]--;
}
