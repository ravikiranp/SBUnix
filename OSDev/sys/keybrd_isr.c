#include <defs.h>
#include <function.h>
#include<structs.h>

extern struct task_struct * current_task;
extern void myprint (char*i, ...);

extern void go_to(int, int);
extern void write_char(char);


unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char kbdus_num[15] = {0,0,'!','@','#','$','%','^','&','*','(',')','_','+'};  

char * keybrd_ptr=(char *)0xffffffff800b8f9c;	
static volatile int ctr,shift;

//int kbd_buf_index = 0;
//volatile int kbd_global = 0;
char  * kbd_buf_ptr = (char *)0xfffffe0000000000;

void write_keybrd_char(char chr) {

	*(keybrd_ptr)= chr;
        *(keybrd_ptr+ 1) = 0x00;
	
	if(current_task->kbd_global == 1)
		return;
	if(chr == '\n') {
//	__asm__("cli");
		kbd_buf_ptr[current_task->kbd_buf_index] = '\0';
		current_task->kbd_global = 1;
		//print_out("read_done");
//	__asm__("sti");
		return;
	}
	if(chr == '\b') {
		console_output_ptr -= 2;
		*console_output_ptr = ' ';
		*(console_output_ptr + 1)= 0x00;
		current_task->kbd_buf_index -= 1;
		//kbd_buf_ptr[current_task->kbd_buf_index++] = chr;
	}
	else {
		*console_output_ptr = chr;
		*(console_output_ptr + 1)= 0x00; 
		console_output_ptr += 2;
		kbd_buf_ptr[current_task->kbd_buf_index++] = chr;
	}
}
 
void keybrd_isr_handler() {

    unsigned volatile char scancode; //,kbd_buf;
    //kbd_buf='0';
    // Read from the keyboard's data buffer
    __asm__ ("inb $0x60,%%al"
		: "=r" (scancode)
		:
    );

    // If the top bit of the byte we read from the keyboard is
    //  set, that means that a key has just been released 
    if (ctr == 0) {
	//kbd_buf='0';
    }

    if (scancode & 0x80) {
	//write_char(kbdus[0x02]);
    }
    else
    {
	if(scancode == 0x2a) {
		shift=1;
	}

	else if (shift == 1) {
		if ((scancode > 0x0f && scancode < 0x1a) || (scancode > 29 && scancode < 39) || (scancode > 43 && scancode < 51)) {
			char uppercase = kbdus[scancode]-32;
			write_keybrd_char(uppercase);
		}	
		else if (scancode >0x01 && scancode < 0x0E) {
			write_keybrd_char(kbdus_num[scancode]);
		}
		else if (scancode == 0x1A)
			write_keybrd_char('{');
		else if (scancode == 0x1B)
			write_keybrd_char('}');
		else if (scancode == 0x27)
                        write_keybrd_char(':');
		else if (scancode == 0x28)
                        write_keybrd_char('"');
		else if (scancode == 0x29)
                        write_keybrd_char('~');
		else if (scancode == 0x2B)
                        write_keybrd_char('|');
		else if (scancode == 0x33)
                        write_keybrd_char('<');
		else if (scancode == 0x34)
                        write_keybrd_char('>');
		else if (scancode == 0x35)
                        write_keybrd_char('?');
		else {
			write_keybrd_char(kbdus[scancode]);
		}
		shift=0;
	}
	else {
		write_keybrd_char(kbdus[scancode]);
    	}	
    }
    //kbd_buf=scancode;
    ctr++;

}
