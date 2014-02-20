#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include <function.h>
#include "helper.h"
#include<structs.h>
extern struct task_struct * current_task;

char * kbd_buf = (char *)0xfffffe0000000000;

int read_int(uint64_t* var_addr) {
	
	uint64_t temp;

        __syscall0(4);
	
	temp = atoi(kbd_buf,10);
	if(temp == -1)
		return temp;
	*var_addr = temp;

	return 0;		
}

int read_str(uint64_t* var_addr) {

        __syscall0(4);

	strcpy((char*)var_addr,kbd_buf);

	return 0;
}

int read_hex(uint64_t* var_addr){
	        
        uint64_t temp;
        
	__syscall0(4);

        temp = atoi(kbd_buf,16);
	if(temp == -1) 
		return temp;	

        *var_addr = temp;

	return 0;
}

int scanf(char * format, ...) {

	va_list ap;
	va_start(ap,format);
	
	void * dest_addr = va_arg(ap,void *);

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
						printf("scanf: error in input!!");
						return ret_val;
					  }
                                          break;

                                case 's':
                                          ret_val = read_str((uint64_t *)dest_addr);
					  if(ret_val == -1) {
						printf("scanf: error in input!!");
						return ret_val;
					  }
                                          break;

                                case 'x':
                                          ret_val = read_hex((uint64_t *)dest_addr);
					  if(ret_val == -1) {
						printf("scanf: error in input!!");
						return ret_val;
					  }
                                          break;
			}
			str+=2;
		}
	}


return ret_val;
}
