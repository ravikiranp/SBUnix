#include <defs.h>
#include "mm_struct.h"
#include"file.h"

#define RUNNING 1
#define WAITING 2
#define SUSPEND 3
#define READY 4
#define SLEEP 5
#define EXIT_WAIT 6 
#define COMPLETE 0 

struct task_struct {
        uint64_t task_rip;              // RIP for the task
        uint64_t task_rsp;              // Actual position of rsp
        uint64_t task_ss;               // Actual Stack Segment
        
        uint64_t task_kstack_top;       // Stack top of kernel stack
        uint64_t task_ustack_top;       // Stack top of user stack
        uint64_t task_cr3;              // cr3 for the stack

        //uint64_t task_kstack[64];               // Kernel stack
        //uint64_t task_ustack[64];               // User stack
        uint64_t * task_kstack;               // Kernel stack
        uint64_t * task_ustack;               // User stack

        uint16_t task_id;               //task id
        uint64_t task_parent_id;        //task id for the parent

        struct mm_struct mm;

	struct file open_files[10];
	uint16_t num_open_files;

	volatile uint64_t state;
        //uint16_t task_id;
        uint16_t parent_id;
	//uint16_t child[10];
	volatile uint16_t child[10];
	volatile uint16_t exec_child[10];
	//uint16_t child_count;
        uint16_t ref_count;
        uint16_t waitid;
        volatile uint64_t first_switch;
        uint64_t * user_stack;
        uint64_t * kern_stack;
        //uint64_t * top;
        //uint64_t * ktop;
        uint64_t user_top;
        uint64_t kern_top;
        //uint64_t * func_name;
        uint64_t * kmalloc_page_ptr;
        uint64_t * kmalloc_free_ptr;
        uint64_t * cr3;
        uint64_t * virtual_cr3;
        uint64_t rip;
        volatile uint16_t sleep_count;

	volatile uint64_t kbd_global;
	uint64_t kbd_buf_index;
        uint64_t rax;
        uint64_t rbx;
        uint64_t rbp;
        uint64_t rcx;
        uint64_t rdx;
        uint64_t rdi;
        uint64_t rsi;
        uint64_t r8;
        uint64_t r9;
        uint64_t r10;
        uint64_t r11;
        uint64_t r12;
        uint64_t r13;
        uint64_t r14;
        uint64_t r15;

};


struct task_struct task_desc[10];
struct task_struct task[10];
