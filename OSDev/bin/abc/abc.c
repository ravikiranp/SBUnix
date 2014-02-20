#include <stdio.h>

int main(int argc, char* argv[]) {
	printf("Hello OS!\n");
	printf(" I am going to sleep for 5 seconds ");
	sleep(5);
	printf("I am now forking a child ");
	
	uint16_t val = fork();
	if(val == 0) {
		uint16_t pid = getpid();
		printf(" \nI am the child with pid %d ",pid);
		printf("\nI AM OPENING A FILE!! ");
		int fd = open("temp/abc");
		printf(" The return value of opening temp/abc is %d  ",fd);
		sleep(2);
		fd = open("temp/def");
		printf(" The return value of opening temp/def is %d  ",fd);
		char * buf = malloc(100);

		int succ = read(fd,buf,10);
		buf[succ] = '\0';
		printf(" \n--the num of bytes read is %d and the contents are %s  ",succ,buf);	
		
		succ = read(fd,buf,9);
		buf[succ] = '\0';
		printf(" \nthe num of bytes read is %d and the contents are %s  ",succ,buf);	

		succ = read(fd,buf,100);
		buf[succ] = '\0';
		printf(" \nthe num of bytes read is %d and the contents are %s  ",succ,buf);	
		
		succ = close(fd);
		printf(" \nThe close was a success with value %d ",succ);
		succ = close(10);
		printf(" \nNow is dont knwo what this is..%d :(",succ);
	}
	else {
		uint16_t pid = getpid();
		printf(" \nI am the parent with pid %d ",pid);
	}
	return 0;
}
