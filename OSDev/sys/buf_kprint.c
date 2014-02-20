#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
#include<function.h>
typedef __builtin_va_list va_list;


//extern void write_int(int,char*,int);

//char* console_output_ptr = (char*) 0xffffffff800b8000;
//static volatile char* initial_ptr = (volatile char*) 0xffffffff800b8000;

char * bufk_write_int(int value, char * str, int base) {
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
	//write_str(rc);
}

char * bufk_write_intp(long int value, char * str, int base) {
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
	//write_str(rc);
	return rc;
}
