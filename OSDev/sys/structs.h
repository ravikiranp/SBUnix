#include<defs.h>

struct task_struct
{
	uint64_t stack[64];
	uint64_t * top;
	uint64_t * func_name;
};

struct tasks
{
	struct task_struct task;
	struct tasks *next;
};
	
