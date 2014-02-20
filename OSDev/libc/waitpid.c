#include<syscall.h>
void waitpid(uint16_t task_id) {
	uint16_t * waitid = (uint16_t *)0xfffffe0000000000;
        waitid[0] = task_id;

        __syscall1(2,(uint64_t)task_id);
	
	uint64_t * waitid_wait = (uint64_t *)0xfffffe0000000000;
        waitid_wait[0] = 10000000;
        while(waitid_wait[0] > 0)
                waitid_wait[0]--;
}
