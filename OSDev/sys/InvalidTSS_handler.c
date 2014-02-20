#include <function.h>

void InvalidTSS_handler() {
	print_out(" Invalid TSS fault!!!  ");
	while(1);
}
