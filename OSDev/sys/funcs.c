#include<defs.h>
#include<function.h>
#include<structs.h>
#include<stdio.h>
#include<syscall.h>

//extern uint64_t kern_fork();
//extern uint16_t kern_getpid();
//extern void kern_sleep(uint16_t);
//extern void kern_waitpid(uint16_t);
//extern void kern_exit();
extern void kern_print(char*,...);
extern int printf(const char *, ...);
extern int kscanf(char *,...);

void kernel_thread() {
	print_out("Kernel_thread");	
	while(1); 
}
void func1()
{
        int i=0,j=0;
	//pid = get_pid();
	//print_out(" My PID is %d  ",pid);
        while(i < 10) {
		//j=0;
		if(i==5)
			kern_waitpid(1);
                print_out("     Hello ");
		//if(j==0) {
		//	sleep(10);
		//	j=1;
		//}
		//print_out(" I am task %d and my cr3 is %x  ",current_task->task_id,current_task->cr3);
                //schedule();
                i++;
		j=0;
		while(j<100000000)
			j++;
        }
	//kern_exit();
	__syscall0(1);
        //while(1);
}

void func2() {
        int i=0,j=0;
	//pid = get_pid();
	//print_out(" My PID is %d  ",pid);
        while(i < 10) {
		//if(i==2)
		//	kern_sleep(5);
			//waitpid(3);
                //print_out("World...i is now %d :)   ",i);
                printf("World...i is now %d :)   ",i);
		//print_out(" I am task %d and my cr3 is %x  ",current_task->task_id,current_task->cr3);
                //schedule();
                i++;
		j=0;
		while(j < 100000000)
			j++;
        }
	//char * name = (char *)0x0000000030000000;
	
	char * d = (char *)0x0000000030000000;
	//int * h = (int *)0x0000000030000000;

	//print_out("  Enter a digit  ");
	//kscanf("%d",h);
	//print_out(" The entered digit is %d ",*h);

	print_out(" \nEnter your name ");
        kscanf("%s",d);
        print_out(" The name entered is %s  ",d);

	//kern_exit();
	__syscall0(1);
	while(1);
}
void func3() {
        int i=0,j=0;
	//uint16_t pid;
	char * char_ptr = (char *)0x0000000030000000;
	//pid = kern_getpid();
	//print_out(" My PID is %d  ",pid);
	//uint16_t mypid = kern_getpid();
	//print_out(" My pid is %d   ",mypid);
	//kern_sleep(5);
        while(i < 10) {
                j=0;
		char_ptr[0] = 'B';
                printf(" Hi %c...How",char_ptr[0]);
                //kern_print(" How");
                //print_out(" How");
		//print_out(" I am task %d and my cr3 is %x  ",current_task->task_id,current_task->cr3);
                //schedule();
                i++;
                while(j < 100000000)
                        j++;
        }
	//uint64_t val = kern_fork();
	uint16_t val = __syscall0(3);
	printf("   Whhhoooooohhhoooo!!!!! ");
	//uint64_t val = 0;
	if(val == 0) {
		printf("    reached here now i will get my pid  ");
		//char * char_ptr1 = (char *)0x0000000030000000;
		//print_out(" I read the parent page...its value is %c   ",char_ptr1[0]);
		//char_ptr1[0] = 'Z';
		//print_out("I just updated a user page!!!! here it goes %c",char_ptr1[0]);
		//j=0;
		__syscall1(1,5);
		//kern_sleep(5);
		//while(j<=10000000)
		//	j++;
		//uint16_t pid = kern_getpid();
		uint16_t pid = __syscall0(2); 
		printf("    I am the child with pid %d     ",pid);
		char * char_ptr1 = (char *)0x0000000030000000;
		printf(" I read the parent page...its value is %c   ",char_ptr1[0]);
		char_ptr1[0] = 'Z';
		printf("I just updated a user page!!!! here it goes %c",char_ptr1[0]);
			
	}
	else {
		printf("I am now calling get_pid");
		//uint16_t pid = kern_getpid();
		uint16_t pid = __syscall0(2);
		printf("Got my PID");
		char * char_ptr1 = (char *)0x0000000030000000;
		char_ptr1[0] = 'Z';
		printf("I just updated a user page successfully....%c",char_ptr1[0]);
		printf("    I am the Parent with PID %d and i just forked a child with PID %d and i will wait for my child to join me   ",pid,val);
		printf(" I am the parent!!!");
		//kern_waitpid(val);
		//__syscall1(2,val);
		//j=0;
		//while(j<=10000000)
                //        j++;
		//printf("EXITING ALONG WITH CHILD!!! :) ");
		printf("EXITING LEAVING CHILD BEHIND!!! :) ");
	}
	__syscall0(1);
	//kern_exit();
	//while(1);
}
void func4() {
        int i=0,j=0;
	//char * char_ptr = (char *)0xffffffff81000000;
	//pid = get_pid();
	//print_out(" My PID is %d  ",pid);
	//uint16_t mypid = kern_getpid();
	//print_out(" My pid is %d   ",mypid);
        while(i < 10) {
                j=0;
		//char_ptr[0] = 'A';
                //print_out(" are you %c",char_ptr[0]);
                print_out(" are you ");
		//print_out(" I am task %d and my cr3 is %x  ",current_task->task_id,current_task->cr3);
                //schedule();
                i++;
                while(j < 100000000)
                        j++;
        }
	//kern_exit();
	while(1);
}
void func5() {
        int i=0,j=0;
	//uint16_t pid;
	//pid = kern_getpid();
	//print_out(" My PID is %d  ",pid);
	//kern_waitpid(1);
        while(i < 10) {
                j=0;
                print_out(" doing? ");
		//print_out(" I am task %d and my cr3 is %x  ",current_task->task_id,current_task->cr3);
                //schedule();
                i++;
                while(j < 100000000)
                        j++;
        }
	//kern_exit();
	__syscall0(1);
	//while(1);
}
