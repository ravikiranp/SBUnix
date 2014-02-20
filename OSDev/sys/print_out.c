#include <function.h>


void print_out(char* temp,...) {
	va_list ap;
	char* str;
	char str_temp[50];

	va_start(ap,temp);
	str = temp;

	// Parse the string str to get the format specifiers
	while(*str != '\0') {
		if((*str != '%') || (*(str+1) == '\0')) {
			write_char(*str++);
		}
		else
		{
			char temp_ch;
			int temp_int;
			int temp_hex;
			void* ptr;
			char* temp_str;

			switch(*(str+1)) {
				case 'd': 
					  temp_int = va_arg(ap,int);
					  write_int(temp_int,str_temp,10);
					  break;
				
				case 'c': 
					  temp_ch = va_arg(ap,int);
					  write_char(temp_ch);
					  break;

				case 'x': 
					  temp_hex = va_arg(ap, int);
					  write_int(temp_hex,str_temp,16);
					  break;

				case 'p': 
					  ptr = va_arg(ap,void*);
					  long int longint;
					  char temp_intp[50];
					  __asm__ volatile ("mov %0,%%r9"
                				: "=r" (longint)
                				: "r" (ptr)
                				);
					  write_intp(longint,temp_intp,16);
					  break;

				case 's': 
					  temp_str = va_arg(ap,char*);
					  write_str(temp_str);
					  break;

				case '%': if (*(str+2) != '\0')
					  	str += 1;

				default: write_char(*str); 
					  write_char(*(str+1));
					
			}

			str += 2;
		}
	}
	va_end(ap);
}
