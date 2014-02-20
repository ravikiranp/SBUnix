#include <defs.h>
#include <function.h>
#include "helper.h"
#include<structs.h>
extern struct task_struct * current_task;

//extern volatile int kbd_global;
//extern char kbd_buf[];
//extern int kbd_buf_index;

char * kbd_buf = (char *)0xfffffe0000000000;

int read_int(uint64_t* var_addr) {
	
	uint64_t temp;

	// Flushing the keyboard buffer
	current_task->kbd_buf_index = 0;
	//kbd_global = 0;
	//print_out("Reading integer from user\n");
	
	while(current_task->kbd_global != 1) {
		print_out("");
	}
	
	kbd_buf[current_task->kbd_buf_index] = '\0';
	//print_out("%s",kbd_buf);
	
	//ret_val = check_int(kbd_buf);
	//if(ret_val == -1)
		//return -1;
	
	temp = atoi(kbd_buf,10);
	if(temp == -1)
		return temp;
	*var_addr = temp;

	//print_out("input val is %d \n",temp);
	//strcpy((char*)var_addr,kbd_buf);

	return 0;		
}

int read_str(uint64_t* var_addr) {
	// Flushing the keyboard buffer
	current_task->kbd_buf_index = 0;
	//kbd_global = 0;
	//print_out("Reading a string from input..");
	
	
	//print_out(" The address of the variable is %x  ",var_addr);
	while(current_task->kbd_global != 1) {
		print_out("");
	}
//	print_out("Yippee..read from buf now..");
	
	//print_out(" Out of while ");
	
	kbd_buf[current_task->kbd_buf_index] = '\0';
	print_out("------%s------",kbd_buf);

	strcpy((char* )var_addr,kbd_buf);

	//print_out(" \n %d chars read",current_task->kbd_buf_index);
	return 0;
}

int read_hex(uint64_t* var_addr){
	        
        uint64_t temp;
        // Flushing the keyboard buffer
        current_task->kbd_buf_index = 0;
        //kbd_global = 0;
        //print_out("Reading integer from user\n");

        while(current_task->kbd_global != 1) {
		print_out("");
	}

        kbd_buf[current_task->kbd_buf_index] = '\0';
        //print_out("%s",kbd_buf);

        //ret_val = check_int(kbd_buf);
        //if(ret_val == -1)
                //return -1;

        temp = atoi(kbd_buf,16);
	if(temp == -1) 
		return temp;	

        *var_addr = temp;

        //print_out("input val is %d \n",temp);
	return 0;
}

int kscanf(char * format, ...) {

//	char buf[1024];
	va_list ap;
	va_start(ap,format);
        //variable = format;
	
	void * dest_addr = va_arg(ap,void *);

	current_task->kbd_global = 0;

	char* str = format;
	int ret_val = 0;
	
	 while(*str != '\0') {
                if((*str != '%') || (*(str+1) == '\0')) {
                        //write_char(*str++);
                }
                else
                {
                        switch(*(str+1)) {
                                case 'd':
                                          ret_val = read_int((uint64_t *)dest_addr);
					  if(ret_val == -1) {
						print_out("scanf: error in input!!");
						return ret_val;
					  }
                                          break;

                                case 's':
                                          ret_val = read_str((uint64_t *)dest_addr);
					  if(ret_val == -1) {
						print_out("scanf: error in input!!");
						return ret_val;
					  }
                                          break;

                                case 'x':
                                          ret_val = read_hex((uint64_t *)dest_addr);
					  if(ret_val == -1) {
						print_out("scanf: error in input!!");
						return ret_val;
					  }
                                          break;
			}
			str+=2;
		}
	}


return ret_val;
}
