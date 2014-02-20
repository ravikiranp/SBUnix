#include <function.h>
extern char * buf_write_int(int, char *, int);
extern char * buf_write_intp(long int, char *, int);
extern void write_str(char*);

void kern_print(char* temp,...) {
	char * buf_ptr = (char *)0xfffffe0000000000;
	int buf=0;
	va_list ap;
	char* str;
	char str_temp[50];

	va_start(ap,temp);
	str = temp;

	// Parse the string str to get the format specifiers
	while(*str != '\0') {
		if((*str != '%') || (*(str+1) == '\0')) {
			buf_ptr[buf] = *str++;
			buf++;
			//write_char(*str++);
		}
		else
		{
			char temp_ch;
			int temp_int;
			int temp_hex;
			void* ptr;
			char* temp_str;
			char* buf_str;

			switch(*(str+1)) {
				case 'd': 
					  temp_int = va_arg(ap,int);
					  buf_str = buf_write_int(temp_int,str_temp,10);
					  while(*buf_str !='\0') {
						buf_ptr[buf] = *buf_str;
						buf++;
						buf_str++;
					  }
					  break;
				
				case 'c': 
					  temp_ch = va_arg(ap,int);
					  //write_char(temp_ch);
					  buf_ptr[buf] = temp_ch;
					  buf++;
					  break;

				case 'x': 
					  temp_hex = va_arg(ap, int);
					  buf_str = buf_write_int(temp_hex,str_temp,16);
					  while(*buf_str !='\0') {
                                                buf_ptr[buf] = *buf_str;
                                                buf++;
                                                buf_str++;
                                          }
					  break;

				case 'p': 
					  ptr = va_arg(ap,void*);
					  long int longint;
					  char temp_intp[50];
					  __asm__ volatile ("mov %0,%%r9"
                				: "=r" (longint)
                				: "r" (ptr)
                				);
					  buf_str = buf_write_intp(longint,temp_intp,16);
					  while(*buf_str !='\0') {
                                                buf_ptr[buf] = *buf_str;
                                                buf++;
                                                buf_str++;
                                          }
					  break;

				case 's': 
					  temp_str = va_arg(ap,char*);
					  //write_str(temp_str);
					  while(*temp_str !='\0') {
                                                buf_ptr[buf] = *temp_str;
                                                buf++;
                                                temp_str++;
                                          }
					  break;

				case '%': if (*(str+2) != '\0')
					  	str += 1;

				default: //write_char(*str); 
					  //write_char(*(str+1));
					 buf_ptr[buf++] = *str;
					 buf_ptr[buf++] = *(str+1);
					
			}

			str += 2;
		}
	}
	va_end(ap);
	buf_ptr[buf] = '\0';
	__asm__ volatile(
                "mov $0x86,%%eax;"
                " int $0x86;"
                :
                :
                :"memory"
                );
}
