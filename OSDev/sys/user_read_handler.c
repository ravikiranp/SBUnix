#include <function.h>
#include<defs.h>
#include<structs.h>
#include<stdio.h>
#include "fs.h"

void user_read_handler()
{
	uint64_t * param = (uint64_t *)0xfffffe0000000000;
	param[0] = kern_read((int)param[0],(void *)param[1],(uint64_t)param[2]);
}
