#include<syscall.h>
void exit(int status) {
	//int ret_val = 0;
        __syscall0(1);
}
