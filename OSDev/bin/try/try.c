#include <stdio.h>

int main(int argc, char* argv[]) {
        printf("\n----------Hello I am trying!----------\n");
	char * test = malloc(100);
	test = "Hi, i am the test string ";
        printf("\n----------I am now forking a child ----------\n");
        uint16_t val = fork();
        if(val == 0) {
		char * filename = malloc(50);
		filename = "bin/exec_try";
		printf(" Entering now ");
		printf("\n $$$$$$$$$$$$$$$$$$$$$$$$the test string in child is %s\n",test);
		execvpe((const char *)filename);
		printf("\n&&&&&&&&&&&&&&&&&&&&&I am the child and the filename i got is %s and i wont return here&&&&&&&&&&&&&&&&&&&\n",filename);
	}
	else {
		printf("\n----------I am the parent----------\n");
		printf("\nthe test srting in parent is %s\n",test);
	}
	return 0;
}
