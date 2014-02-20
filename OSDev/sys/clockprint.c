#include <function.h>
volatile int ctr;
extern void decrement_sleep_count();
extern void check_waitpid();
extern void check_child_processes();

void clockprint () {

	static volatile int sec,min,hr;
	int i;	
	if (ctr%18 == 0) {
		//clear_screen();
		check_waitpid();
		char * temp = console_output_ptr;
		go_to(24,68);
        	for(i=0;i<9;i++) {
                	write_char(' ');
        	}
        	go_to(24,68);
		sec++;
		sec%=60;
		if (sec == 0) { 
			min++;
			min%=60;
			if (min == 0) 
 				hr++;
		}
		print_out("|%d:%d:%d|",hr,min,sec);
		console_output_ptr=temp;	
		decrement_sleep_count();
		//check_child_processes();
	}
	ctr++;
}
