#include <function.h>
#include<defs.h>
#include<structs.h>
#include<stdio.h>
#include"fs.h"

int user_open_handler()
{
	uint64_t * filename = (uint64_t *)0xfffffe0000000000;
	print_out(" the file name is %s ",(char *)filename[0]);
	filename[0] = kern_open((char *)filename[0]);
	print_out(" The return value is %d ",filename[0]);
	return ((int)filename[0]);
}
