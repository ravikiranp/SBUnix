#include<defs.h>
#ifndef _STDIO_H
#define _STDIO_H

int printf(const char *format, ...);
//int scanf(const char *format, ...);
int scanf(char *format, ...);
void waitpid(uint16_t task_id);
uint16_t getpid();
void sleep(uint16_t sec);
uint16_t fork();
void * malloc(uint64_t);
int open(const char *);
int close(int);
int read(int,void *,uint64_t);
void execvpe(const char *);

#endif
