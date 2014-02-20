#include <function.h>

void TSS_isr_handler () {
	print_out("     Inside the TSS handler      ");
	while(1);
}
