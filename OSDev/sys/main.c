#include <defs.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/idt.h>
#include <function.h>
#include <structs.h>
#include"elf_header.h"

char printf_buf[4096];
//char * printf_buf_ptr = printf_buf[0];

extern void switch_to(uint64_t);
extern void print_buf(char *,...);
//extern void load_file(uint64_t *, uint64_t, uint64_t, uint64_t *, uint64_t *);
extern void load_file();
extern void load_shell();
extern struct parse_info* parse_elf(char*);
uint64_t file_cr3;

extern void register_fs(uint64_t);
extern char _binary_tarfs_start;

int total = 0;
uint64_t ** error_code;
int total_tasks=0;
uint16_t parse_taskid = 0;
uint16_t globl_dummy = 0x22;

struct task_struct* cur_task;

struct task_struct * current_task;
struct task_struct * next_task;

struct parse_info * file; 
struct load_info * load_file_list;

extern void kernel_thread();
extern void func1();
extern void func2();
extern void func3();
extern void func4();
extern void func5();
extern void initialize_tasks();

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			//print_out("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
			total = smap->base + smap->length;
		}
	}
	//print_out("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	
	total = (total/1048576) +1;
        //print_out(" The Highest memory is %d ",total);
        //print_out("This is the address of physfree -- %p             ", physfree);
        initialize_free_pages();
        initialize_tasks();
	register_fs((uint64_t)&_binary_tarfs_start);
        create_page_tables(physfree);
        //print_out("     Successfully created page tables");

	__asm__("cli;");

	file = parse_elf("bin/shell");
        load_file_list = file->load_list;
        load_shell();
	/*
	file = parse_elf("bin/hello");
	load_file_list = file->load_list;	
	load_file();
	*/
	file = parse_elf("bin/try");
	load_file_list = file->load_list;	
	load_file();

	switch_to(0);
	// kernel starts here
	while(1);
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *temp1, *temp2;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	__asm__("cli");
        load_idt();
	setup_tss();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	for(
		temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
		*temp1;
		temp1 += 1, temp2 += 2
	) *temp2 = *temp1;
	while(1);
}
