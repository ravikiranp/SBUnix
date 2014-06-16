#include <sys/idt.h>
#include <defs.h>

#define TIMER_IRQ	32
#define KBRD_IRQ	33

#define IRQ_8		8
#define IRQ_10  	10	
#define IRQ_11  	11	
#define IRQ_12  	12	
#define IRQ_13  	13	
#define IRQ_14  	14
#define IRQ_80  	128
#define IRQ_81  	129
#define IRQ_82  	130
#define IRQ_83  	131
#define IRQ_84  	132
#define IRQ_85  	133
#define IRQ_86  	134 //kern_print
#define IRQ_87  	135 //kern_scanf
#define IRQ_88  	136 //for user_malloc
#define IRQ_89  	137
#define IRQ_50  	80
#define IRQ_51  	81
#define IRQ_52  	82	




#define IRQ_90  	144
#define IRQ_91  	139
#define IRQ_92  	140 //kern_print
#define IRQ_93  	141 //kern_scanf
#define IRQ_94  	142 //for user_malloc

typedef struct _idt_desc {
	
	uint16_t 	offset_low;
	uint16_t 	cs_selector;
	unsigned char	ZeroByte;
	unsigned char	TypeAttr;
	uint16_t 	offset_mid;
	uint32_t 	offset_high;
	uint32_t	Zeroed;
	
}__attribute__((packed)) idt_desc;

typedef struct _idtr_t {
	uint16_t limit;
	uint64_t offset;
}__attribute__((packed)) idtr;

static idt_desc idt_table[256];

static idtr idtr_reg = {
	sizeof(idt_table),
	(uint64_t)idt_table,
};


void build_idt_desc(unsigned char idt_index,uint64_t BaseAddr,unsigned char selector,unsigned char attr) {
	
	idt_table[idt_index].offset_low  = BaseAddr & 0x0000ffff ;
	idt_table[idt_index].cs_selector = selector;
	idt_table[idt_index].ZeroByte 	 = 0x0;
	idt_table[idt_index].TypeAttr	 = attr;
	idt_table[idt_index].offset_mid  = (BaseAddr >> 16) & 0x0000ffff;
	idt_table[idt_index].offset_high = (BaseAddr >> 32) & 0xffffffff;
	idt_table[idt_index].Zeroed	 = 0x0;	
}

void timer_isr();

void setup_timer_isr() {
	/* Setup Timer Interrupt Gate */
	unsigned char attr = PRES | DPL_0 | S | INTR_GATE;
	build_idt_desc(TIMER_IRQ,(uint64_t)timer_isr,8,attr);

}

void keybrd_isr();

void setup_kbrd_isr() {
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
	build_idt_desc(KBRD_IRQ,(uint64_t)keybrd_isr,8,attr);
}

void TSS_isr();

void setup_TSS_exp() {
	unsigned char attr = PRES | DPL_0 | S | INTR_GATE;
        build_idt_desc(IRQ_10,(uint64_t)TSS_isr,8,attr);
}	

void page_fault_isr();

void setup_page_fault_exp() {
	unsigned char attr = PRES | DPL_0 | S | INTR_GATE;
        build_idt_desc(IRQ_14,(uint64_t)page_fault_isr,8,attr);
}	
	
void GPF_isr();

void setup_GPF_exp() {
	unsigned char attr = PRES | DPL_0 | S | INTR_GATE;
        build_idt_desc(IRQ_13,(uint64_t)GPF_isr,8,attr);
}	

void syscall_int();

void setup_syscall_int() {
	
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_80,(uint64_t)syscall_int,8,attr);
}

void kern_fork_int();

void setup_kern_fork_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_81,(uint64_t)kern_fork_int,8,attr);
}

void kern_exec_int();

void setup_kern_exec_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_52,(uint64_t)kern_exec_int,8,attr);
}

void kern_getpid_int();

void setup_kern_getpid_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_82,(uint64_t)kern_getpid_int,8,attr);
}

void kern_sleep_int();

void setup_kern_sleep_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_83,(uint64_t)kern_sleep_int,8,attr);
}

void kern_waitpid_int();

void setup_kern_waitpid_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_84,(uint64_t)kern_waitpid_int,8,attr);
}

void kern_exit_int();

void setup_kern_exit_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_85,(uint64_t)kern_exit_int,8,attr);
}

void kern_print_int();

void setup_kern_print_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_86,(uint64_t)kern_print_int,8,attr);
}

void kern_scan_int();

void setup_kern_scan_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_87,(uint64_t)kern_scan_int,8,attr);
}

void user_malloc_int();

void setup_user_malloc_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_88,(uint64_t)user_malloc_int,8,attr);
}

void user_open_int();

void setup_user_open_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_89,(uint64_t)user_open_int,8,attr);
}

void user_read_int();

void setup_user_read_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_51,(uint64_t)user_read_int,8,attr);
}

void user_close_int();

void setup_user_close_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_50,(uint64_t)user_close_int,8,attr);
}

/*
void user_opendir_int();

void setup_user_opendir_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_92,(uint64_t)user_opendir_int,8,attr);
}
*/
void user_readdir_int();

void setup_user_readdir_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_93,(uint64_t)user_readdir_int,8,attr);
}

void user_closedir_int(); 


void setup_user_closedir_int() {

        unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_94,(uint64_t)user_closedir_int,8,attr);
}

/*    Setup Double Fault ISR  */
void double_fault_isr();

void setup_double_fault_isr() {
	
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_8,(uint64_t)double_fault_isr,8,attr);
}


/* Setup Invalid TSS ISR*/

void InvalidTSS_isr();

void setup_InvalidTSS_isr() {
	
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_10,(uint64_t)InvalidTSS_isr,8,attr);
}

/* Segment Not Present Exception */
void SegmentNP_isr();

void setup_SegmentNP_isr() {
	
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_11,(uint64_t)SegmentNP_isr,8,attr);
}

/* Stack exception*/
void StackException_isr();

void setup_StackException_isr() {
	
	unsigned char attr = PRES | DPL_3 | S | INTR_GATE;
        build_idt_desc(IRQ_12,(uint64_t)StackException_isr,8,attr);
}

void _x86_asm_load_idt(idtr *idtr_register);
void _x86_asm_remap_intr();

void load_idt() {
	setup_timer_isr();
	setup_kbrd_isr();
	setup_page_fault_exp();
	setup_GPF_exp();
	setup_syscall_int();
	setup_kern_fork_int();
	setup_kern_exec_int();
	setup_kern_getpid_int();
	setup_kern_sleep_int();
	setup_kern_waitpid_int();
	setup_kern_exit_int();
	setup_kern_print_int();
	setup_kern_scan_int();
	setup_user_malloc_int();
	setup_user_open_int();
	setup_user_read_int();
	setup_user_close_int();
	//setup_user_opendir_int();
	setup_user_readdir_int();
	setup_user_closedir_int();
	setup_double_fault_isr();
	setup_InvalidTSS_isr();
	setup_SegmentNP_isr();
	setup_StackException_isr();

	_x86_asm_load_idt(&idtr_reg);
	_x86_asm_remap_intr();
}
