#include <function.h>
#include<defs.h>
#include<structs.h>
#include<stdio.h>
#include "fs.h"

void user_close_handler()
{
	int * fd = (int *)0xfffffe0000000000;
	fd[0] = kern_close((int)fd[0]);
}
