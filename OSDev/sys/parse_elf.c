
#include <sys/tarfs.h>
#include <defs.h>
#include <function.h>
#include "elf_header.h"

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

unsigned char xstrcmp(char* str1, char* str2); 
uint64_t atoi_t(volatile char* num_string);

extern struct parse_info* elf_parser(uint64_t parse_ptr);

struct parse_info* parse_elf(char* path) {
	
	uint64_t parse_ptr = (uint64_t)&_binary_tarfs_start;
	struct posix_header_ustar* ptr;
	uint64_t size;	
	uint64_t remainder;
		//print_out("   name is %s ",parse_ptr->name);
		//print_out("  size id %s",parse_ptr->size);

	while(parse_ptr < (uint64_t)&_binary_tarfs_end) {

		ptr = (struct posix_header_ustar *) (parse_ptr);
		if(ptr->name[0] == '\0') {
			parse_ptr += 512;
			continue;
		}
		//print_out("   name is %s ",ptr->name);
		//print_out("  size is %s",ptr->size);
		size = atoi_t(ptr->size);
		
		//print_out("  size is %d",size);
		
		parse_ptr += (512);

		//if(xstrcmp(ptr->name,"bin/hello")) 
		if(xstrcmp(ptr->name,path)) 
			break;	
		
		if(size > 0) {
			remainder = size % 512UL;
			//print_out("  remainder is %d ",remainder);
			parse_ptr += (size + 512 - remainder);
		}
			
	}
	
	struct parse_info *loadptr;
//	clear_screen();
	loadptr = elf_parser(parse_ptr);
	//print_out("\n ---- \nentry point is %x",loadptr->entry_point);
	//print_out("\n file start ptr is %x",loadptr->file_start_ptr);
	
	/*
	struct load_info* temp = loadptr->load_list;
	
	while(temp != NULL){
		print_out("\n File offset is %d",temp->file_offset);	
		print_out("\n virtual addr is %x",temp->v_addr);
		print_out("\n File seg size is %d",temp->file_seg_size);
		print_out("\n Mem seg size is %d",temp->mem_seg_size);	
		print_out("\n Flags are %d",temp->flags);	
		temp = temp->next;
	}
	*/
	
	return loadptr;
	//return parse_ptr;

}

/* Convert the number string to a integer
 *
 */

uint64_t atoi_t(volatile char* num_string) {

	int last_index = 0;
	int dec_factor = 1;
	uint64_t num = 0;
	
	while(num_string[last_index+1] != '\0') {
		last_index++;
	}

	while(last_index != -1) {
		num = num + dec_factor * (num_string[last_index] - '0' );
		dec_factor *= 8;
		last_index--;
	}
	return num;
	
}

/* Compare two strings
 * Return 1 on success, 0 on Failure
 */

unsigned char xstrcmp(char* str1, char* str2) {
	
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
