#include<syscall.h>
#include<stdio.h>
void execvpe(const char * filename) {
	char * file_name = (char *)0xfffffe0000000000;
	while(*filename != '\0') {
		*file_name = *filename;
		filename++;
		file_name++;
	}
	*file_name = '\0';
        __syscall1(6,(uint64_t)file_name[0]);
}
