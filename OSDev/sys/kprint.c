#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
#include<function.h>
typedef __builtin_va_list va_list;


extern void write_int(int,char*,int);

char* console_output_ptr = (char*) 0xffffffff800b8000;
static volatile char* initial_ptr = (volatile char*) 0xffffffff800b8000;

void show_cursor() {
	if (console_output_ptr < (char *)0xffffffff800b8f98) {
        	*console_output_ptr = ' ';
        	*(console_output_ptr + 1) = 0xFF;
	}
	else {
		console_output_ptr=(char*) 0xffffffff800b8000;
		//*console_output_ptr = ' ';
                //*(console_output_ptr + 1) = 0xFF;
	}
}

void clear_screen() {
	console_output_ptr = (char *)0xffffffff800b8000;
	int counter=0;
	while (counter++<500000)
	{
		*console_output_ptr = ' ';
		*(console_output_ptr + 1) = 0x00;
        	console_output_ptr += 2;
	}
	console_output_ptr = (char *)0xffffffff800b8000;
	//show_cursor();
}

void go_to(int row, int col) {
        int position=((row*80)+col)*2;
	long int final_ptr;
        __asm__ volatile ("add %%ebx, %%eax;"
                : "=a" (final_ptr)
                : "a" (position), "b" (initial_ptr)
        );
	final_ptr = (final_ptr | 0xffffffff80000000);
        console_output_ptr=(char *)final_ptr;
}

void write_char(char chr) {
	if (console_output_ptr > (char *)0xffffffff800b8f98) {
		console_output_ptr = (char *)0xffffffff800b8000;
		clear_screen();
	}

	if(chr == '\n') {
           int rem = 0;
           rem = (uint64_t)(console_output_ptr - (char *)0xffffffff800b8000)  % 0xa0;
           if(rem == 0)
                 console_output_ptr += 0xa0;
           else {
                   console_output_ptr -= rem;
                   console_output_ptr += 0xa0;
                }
        }

	else {

        *console_output_ptr = chr;
	*(console_output_ptr + 1) = 0x00;
	console_output_ptr += 2;
	}
}

void write_in_next_line(char chr) {
	//if (console_output_ptr > (char *)0xffffffff800b8f98) {
        //        console_output_ptr = (char *)0xffffffff800b8000;
        //}
	console_output_ptr = (char *)0xffffffff800b8140;
	*console_output_ptr = chr;
        *(console_output_ptr + 1) = 0x00;
        console_output_ptr += 2;
	//show_cursor();
}

void write_str(char* str) {
        while ( *str != '\0')
        {
		//if (*str == '\n')
		//{
			//write_char('Z');
			//int row = (console_output_ptr - (char *)0xb8000)/160;
			//go_to(row+1,0);	
			//char temp[5]; 
			//write_int(row,temp,10);
		//}
		//else {
		if (console_output_ptr > (char *)0xffffffff800b8f98) {
                	console_output_ptr = (char *)0xffffffff800b8000;
			clear_screen();
        	}
                *console_output_ptr = *str;
                *(console_output_ptr + 1) = 0x00;
                str++;
                console_output_ptr += 2;
        }//}
	//show_cursor();
}

void kern_write_str(char* str) {
        while ( *str != '\0')
        {
		write_char(*str++);
        }
}

void write_int(int value, char * str, int base) {
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
	write_str(rc);
}

void write_intp(long int value, char * str, int base) {
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
	write_str(rc);
}
