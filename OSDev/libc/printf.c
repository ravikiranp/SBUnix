#include<syscall.h>
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)

char * buf_write_int(int value, char * str, int base) {
	char *rc, *ptr, *low;
        if ((base != 10) && (base != 16)) {
                *str = '\0';
        }
        rc = ptr = str;
        if ((value < 0) && (base == 10))
                *ptr++ = '-';
        low = ptr;
        do {
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
                value /= base;
        } while(value);
        *ptr-- = '\0';
        while (low < ptr) {
                char tmp = *low;
                *low++ = *ptr;
                *ptr-- = tmp;
        }
	return rc;
}

char * buf_write_intp(long int value, char * str, int base) {
	char *rc, *ptr, *low;
        if ((base != 10) && (base != 16)) {
                *str = '\0';
        }
        rc = ptr = str;
        low = ptr;
        do {
                *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
                value /= base;
        } while(value);
        *ptr-- = '\0';
        while (low < ptr) {
                char tmp = *low;
                *low++ = *ptr;
                *ptr-- = tmp;
        }
	return rc;
}

int printf(const char *format, ...) {

	char * buf_ptr = (char *)0xfffffe0000000000;
	int buf=0;
	va_list ap;
	const char* str;
	char str_temp[50];

	va_start(ap,format);
	str = format;

	// Parse the string str to get the format specifiers
	while(*str != '\0') {
		if((*str != '%') || (*(str+1) == '\0')) {
			buf_ptr[buf] = *str++;
			buf++;
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
					  while(*temp_str !='\0') {
                                                buf_ptr[buf] = *temp_str;
                                                buf++;
                                                temp_str++;
                                          }
					  break;

				case '%': if (*(str+2) != '\0')
					  	str += 1;

				default: buf_ptr[buf++] = *str;
					 buf_ptr[buf++] = *(str+1);
					
			}
			str += 2;
		}
	}
	va_end(ap);
	buf_ptr[buf] = '\0';

	int ret_val = 0;
	ret_val = __syscall1(3,(uint64_t)buf_ptr);
	return ret_val;
}
