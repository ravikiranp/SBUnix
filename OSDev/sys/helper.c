#include <defs.h>
#include <function.h>
/*
 * Copy src string to dest string.
 * Return 0 on success. 1 on failure
 *
 */

unsigned char strcpy(char* dest,const char* src) {

        //if((src == NULL) || (dest == NULL) )
         //       return 0;

        int i = 0;

        while(src[i] != '\0') {
                dest[i] = src[i];
                i++;
        }

        dest[i] = '\0';

        return 1;
}

/* Compare two strings
 * Return 1 on success, 0 on Failure
 */

void dummy1() {
}

unsigned char strcmp(const char* str1, const char* str2) {

        int i = 0;

        if((str1 == NULL) || (str2 == NULL))
                return 0;

        while(str1[i] == str2[i]) {

                if(str1[i] == '\0')
                        return 1;
                i++;
        }


return 0;
}



uint64_t atoi(volatile char* num_string, int base) {

        int last_index = 0;
        int dec_factor = 1;
        uint64_t num = 0;
	uint64_t multiplier = 1 ;
	char ch;

        while(num_string[last_index+1] != '\0') {
                last_index++;
        }
	
	int i = 0;
	for(i =0 ; i <= last_index; i++) {
	 	ch = num_string[i];
                if(base == 10) {
                        // Check for a valid decimal number..
                        if(ch < '0' || ch > '9')
                                return -1;
                }
                if(base == 16) {
                        // Check for a valid hex number
                	if(!( (ch >= 'a' && ch <= 'f')  || (ch >= 'A' && ch <= 'F') || (ch >= '0' && ch <= '9') ))
                                return -1;
                }

	}

        while(last_index != -1) {
		ch = num_string[last_index];
		if(base == 16) {
			if(ch >= '0' && ch <= '9') {
				multiplier = ch - '0';
			}
			 else if(ch >= 'a' && ch <= 'f') {
				multiplier = 10 + ch - 'a';
			} 
			else
			{
				multiplier = 10 + ch - 'A';
			}
		}
		if((base == 10) || (base == 8)) {
			multiplier = ch - '0';
		}

                //num = num + dec_factor * (num_string[last_index] - '0' );
                num = num + dec_factor * multiplier;
                dec_factor *= base;
                last_index--;
        }
        return num;

}

void memcpy(char* buf1,char* buf2,uint64_t count) {
	
	if(buf1 == NULL || buf2 == NULL)
		return;
	
//	print_out("\n-----first_char at offset is %c---------\n",buf1[0]);
	int index = 0;
	
	for(index = 0; index < count; index++) {
	//	print_out("---copying %c from buf1 to buf 2, index is %d---",buf1[index],index);
		dummy1();
		buf2[index] = buf1[index];
	}	
	
}
