#include <defs.h>
#ifndef _HELPER_FUNCTIONS
#define _HELPER_FUNCTIONS


#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
typedef __builtin_va_list va_list;

// for print_out
extern char * console_output_ptr;
extern void write_char(char);
extern void write_str(char*);
extern void write_strn(char*);
extern void write_int(int,char*,int);
extern void write_intp(long int,char*,int);
extern void clear_screen();
extern void go_to(int,int);
extern void print_out(char *,...);

// for paging
extern int total;
extern int total_tasks;
extern void create_page_tables();
extern void initialize_free_pages();
extern uint64_t * get_free_page();
extern uint64_t * get_free_page_for_paging();
extern uint64_t * get_free_page_for_tasks();
extern uint64_t * virtual_physfree;
extern void map_page_virtual(uint64_t *, uint64_t, uint64_t, uint16_t, int);
extern uint64_t kmalloc_free_ptr[];
extern uint64_t stack_free_ptr[];
extern uint64_t * stack_malloc(uint16_t, int);
extern uint64_t * kern_malloc(uint16_t, int);

// for page_fault
extern uint64_t ** error_code;

// for kernel context switching 
extern void func1();
extern void func2();
extern void init_switch(uint64_t *);
extern uint64_t func1_stack[];
extern uint64_t func2_stack[];
extern void * func1_stack_top;
extern void * func2_stack_top;
extern void * old_func;
extern void * new_func;
extern uint16_t create_task(uint64_t,uint16_t);

// syscalls
extern void kern_sleep(uint16_t);
extern uint64_t kern_fork();
extern uint16_t kern_getpid();
extern void kern_waitpid(uint16_t);
extern void kern_write_str(char*);
extern void kern_printf(char*);
//extern int kscanf(char *,uint64_t *);

#endif
