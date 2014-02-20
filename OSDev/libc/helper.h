#ifndef __HELPER_H
#define __HELPER_H

uint64_t atoi(volatile char* num_string, int base);

unsigned char strcmp(const char* str1, const char* str2);

unsigned char strcpy(char* des,const char* src);

void memcpy(char* buf1,char* buf2,uint64_t count);

#endif /* __HELPER_H */
