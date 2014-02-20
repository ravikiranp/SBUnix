#include <stdio.h>

int main(int argc, char* argv[]) {
	int * d = (int *)malloc(1000);
	char * name = (char *)malloc(10);
	printf("Hello World!\n");

	printf("\nENTER YOUR NAME : ");
	scanf("%s",name);
	printf("\nThe entered name is %s",name);

	printf("\nENTER A DIGIT :");
	scanf("%d",d);
	printf("\nThe entered digit is %d",*d);
	
	uint16_t pid = getpid();
	printf("\nMy pid is %d\n",pid);
	//waitpid(2);
	return 0;
}
