#include <defs.h>
#include "fs.h"
#include <sys/tarfs.h>
#include <function.h>
#include "helper.h"
#include <structs.h>

//FD's or  Index 0,1,2 are reserved for stdin,stdout and stderr
char cwd[128] = "/";
int cwd_index = 0;

fs_node_t fs_nodes[20];
int num_fs_nodes = 0;

extern char _binary_tarfs_end;
extern void elf_parser(uint64_t parse_ptr);
extern uint64_t * kern_malloc(uint16_t, int);
fs_node_t *fs_root = 0;

extern struct task_struct* current_task;

void register_tarfs(uint64_t parse_ptr) {

	// uint64_t parse_ptr = (uint64_t)&_binary_tarfs_start;
        struct posix_header_ustar* ptr;
        uint64_t remainder;

	//uint64_t name[128]	
	//uint16_t typeflag;
        uint64_t size;
	int parent_dir_node_index = 0;
	//clear_screen();

	int index = 0;
	//kern_malloc(0,212);
	
        while(parse_ptr < (uint64_t)&_binary_tarfs_end) {

                ptr = (struct posix_header_ustar *) (parse_ptr);
                if(ptr->name[0] == '\0') {
                        parse_ptr += 512;
                        continue;
                }

	
	        //print_out("Header offset is %x, ",ptr);		
                //print_out("%s,   ",ptr->name);
                //print_out("Type is %d",ptr->typeflag[0] - '0');
                size = atoi(ptr->size,8);
                //print_out("    size is %d\n",size);



		strcpy(fs_nodes[index].name,ptr->name);
		fs_nodes[index].typeflag = ptr->typeflag[0] - '0'; 
		fs_nodes[index].size    = size;
		fs_nodes[index].file_header_offset = (uint64_t)ptr;
		
		// Fill the dirent structure for this node
		strcpy(fs_nodes[index].d_entry.d_name,ptr->name);		
		
		int t = 0;
		int slash_count = 0;

		while(ptr->name[t] != '\0') {
			if(ptr->name[t] == '/')
				slash_count++;
			t++;
		}	
		
	
		if(fs_nodes[index].typeflag == DIRTYPE) {
			fs_nodes[index].d_entry.isDir = 1;

			if((slash_count == 1) && (ptr->name[t-1] == '/'))
			{
				fs_nodes[index].parent_dir_node_index = 0;	
			}
			else
			{
				fs_nodes[index].parent_dir_node_index = parent_dir_node_index;
			}

			parent_dir_node_index = index ;
		}
		//print_out("parent dir node index is %d",parent_dir_node_index);

		if(parent_dir_node_index != index) {
			fs_nodes[index].parent_dir_node_index = parent_dir_node_index;
		}

		if(slash_count == 0) {
			fs_nodes[index].parent_dir_node_index = 0;	
		}

		index++;
		
                parse_ptr += (512);

                if(size > 0) {
                        remainder = size % 512UL;
                        parse_ptr += (size + 512 - remainder);
                }

        }

	num_fs_nodes = index ;

#if 0		
	for(index = 0; index < num_fs_nodes; index++){
                print_out("Header offset is %x, ",fs_nodes[index].file_header_offset);
                print_out("%s,   ",fs_nodes[index].name);
                print_out("Type is %d",fs_nodes[index].typeflag);
				print_out("parent dir node index is %d\n", fs_nodes[index].parent_dir_node_index);
                print_out("  Size is %d\n",fs_nodes[index].size);

		
	}
#endif
}


// Populate the fs_nodes
void register_fs(uint64_t fs_entry) {
	
	register_tarfs(fs_entry);
		
}



int tarfs_open(const char* filename) {

		int index = 0;

		for(index = 0; index < num_fs_nodes; index++){
				if(strcmp(filename,fs_nodes[index].name)){

						//elf_parser(fs_nodes[index].file_header_offset + 512);
						//print_out("Header offset is %x, ",fs_nodes[index].file_header_offset);
						return index;	
				}
				//print_out("%s,   ",fs_nodes[index].name);
				//print_out("Type is %d",fs_nodes[index].typeflag);
				//print_out("  Size is %d\n",fs_nodes[index].size);
		}

		// file not found. Return -1 to indicate error
		//print_out("\nFile not found!!");
		return -1;
		// return index of fs_node with the requested 
}


int kern_closedir(struct dir_struct* dir_ptr) {
	
	if(dir_ptr == NULL)
	   return -1;
	
	
	strcpy(dir_ptr->d_name,"");
	dir_ptr->node_index = -1;
	dir_ptr->next_node_index = -1;
	return 0;

}	

struct dir_struct* kern_opendir(const char* name) {

	int index;
	struct dir_struct* dir_ptr;

	index = tarfs_open(name);
		
	if(index == -1)
		return NULL;


	if(fs_nodes[index].typeflag != DIRTYPE) 
		return NULL;

	//dir_ptr = (struct dir_struct *)kern_malloc(current_task->task_id,sizeof(struct dir_struct));
	dir_ptr = (struct dir_struct *)kern_malloc(current_task->task_id,sizeof(struct dir_struct));
	
	strcpy(dir_ptr->d_name,fs_nodes[index].name);
	dir_ptr->node_index = index ;
	dir_ptr->next_node_index = index;
	
	return dir_ptr;		
}

struct dirent* kern_readdir(struct dir_struct* dir_ptr) {
	
	if(dir_ptr == NULL)
		return NULL;
	
	int found = 0;
		//starting from the node index, pick the node which has the same parent as dir_ptr
		int i;
		int cur_node_parent_index = dir_ptr->node_index;
		
		// forward search from dir_ptr->node_index to num_fs_nodes..

		for(i = 0; i < num_fs_nodes; i++) {
			if((fs_nodes[i].parent_dir_node_index == cur_node_parent_index)&& (i > dir_ptr->next_node_index)) {
				found = 1;
				dir_ptr->next_node_index = i;
				break;
			}
		}
	//	print_out("\nindex is %d",i);
	//	print_out("\nfound is %d",found);
		if(found == 1) 
			return &fs_nodes[dir_ptr->next_node_index].d_entry;
		else
			return NULL;	
			
}



int tarfs_close(int fd) {
	return -1;
}


long tarfs_read(uint64_t ftable_index,uint64_t file_offset,void* buf,uint64_t count) {
	if(ftable_index < 0 || count < 0)
		return -1;
	
	if(file_offset > fs_nodes[ftable_index].size) {
		print_out("File offset greater than the size..");
		return -1;
	}
	//	
	uint64_t file_start_ptr = fs_nodes[ftable_index].file_header_offset + 512;	
	//elf_parser(fs_nodes[fd].file_header_offset + 512);	
	
	//Check for count exceeding EOF etc.. 
		
	uint64_t bytes_to_copy = 0;
	
	
	//char* temp1 = (char*)buf;	
	print_out("\n In tarfs_read..fileoffset is %d\n ",file_offset);	
	// Move to filepos and read count bytes from it into buf
	if((file_start_ptr + file_offset + count) <= (file_start_ptr + fs_nodes[ftable_index].size)) {
		bytes_to_copy = count;
		memcpy((char *)(file_start_ptr+file_offset),(char *)buf,bytes_to_copy);
		//	temp1[count] = '\0';
		//print_out("\n tarfs_read ..buf is %s\n",temp1);	
	}
	else
	{
		if((fs_nodes[ftable_index].size - file_offset) > 0) {
			bytes_to_copy = fs_nodes[ftable_index].size - file_offset; 	
			memcpy((char *)(file_start_ptr+file_offset),(char *)buf,bytes_to_copy);
		}			
	}

	print_out("\ncopied %d bytes from file offset: %d to buffer",bytes_to_copy,file_offset);
	return bytes_to_copy;
}



/*
 * Returns 0 on success, -1 on error
 */
int kern_open(const char* filename)
{
	//open the file and return the task fd
	int ret_val = -1;
	//clear_screen();
	
	//print_out("!!! File name is %s\n",filename);	
	ret_val = tarfs_open(filename);
	if(ret_val == -1) 
	{
		print_out("Error opening file!! \n");
		return -1;
	}


	int num_open_files = current_task->num_open_files;
	strcpy(current_task->open_files[num_open_files].filename,filename );
	current_task->open_files[num_open_files].sys_fd = ret_val;
	current_task->open_files[num_open_files].file_offset = 0;
	current_task->open_files[num_open_files].fd_state = 1;
	current_task->num_open_files = num_open_files + 1;
	
	return num_open_files;

	//current_task->
}


/*
 * Returns 0 on success, -1 on error
 */

int kern_close(int fd) {
	//int num_open_files = current_task->num_open_files;
	
	//print_out(" the file desc is %d ",fd);
		if(current_task->open_files[fd].fd_state == 1) {
			current_task->open_files[fd].fd_state = 0;
			current_task->open_files[fd].file_offset = 0;
			current_task->open_files[fd].sys_fd = -1; 
			return 0;
		}
		else

		return -1;
}



int kern_read(int fd,void* buf,uint64_t count) {
	
	if(fd < 0) {
		print_out("Negative file descriptor!!");
		return -1;
	}

	// Check if the file is open. If not then
	if(fd >= current_task->num_open_files){
		print_out("file with fd: %d is not open!!\n",fd);
		return -1;
	}
	
	
	if(current_task->open_files[fd].fd_state != 1) {
		print_out("file with fd: %d is not open!!\n",fd);
		return -1;
	
	}

	
	uint64_t file_offset = current_task->open_files[fd].file_offset;
	int ftable_index = current_task->open_files[fd].sys_fd;
	long bytes_read = 0;

	bytes_read = tarfs_read(ftable_index,file_offset,buf,count);

//	char* temp1 = (char*) buf; 
	if(bytes_read >= 0) {
		//print_out("\nbytes read is %d",bytes_read);
		//temp1[bytes_read] = '\0';
		//print_out("\nIn kern_read ..buf is %s\n",temp1);
		current_task->open_files[fd].file_offset = file_offset + bytes_read;	
		//update the file offset
		return bytes_read;
	}
	else
	{	
		print_out("Error reading from the file..");
		return -1;
	}
	  	
	
}

void kern_ls() {
	//check cwd and display all dirs starting from it

	int index = 0;
	print_out("\n");	

	for(index = 0; index < num_fs_nodes; index++) {
		if(fs_nodes[index].parent_dir_node_index == cwd_index)
			print_out("%s    ",fs_nodes[index].name);
	}	
}


void kern_cd(char* dir_name) {

	char buf[128];

	if(dir_name == NULL) {
		print_out("\ndir_name is NULL!!");
		return;
	}

	if(strcmp(dir_name,".")) {
		//Do nothing
		return;
	}

	if(strcmp(dir_name,"..")) {
		cwd_index = fs_nodes[cwd_index].parent_dir_node_index;
		return;
	}

	int index = 0;


	// If the last char is not a /, then append one
	strcpy(buf,dir_name);
	print_out("buf is %s\n",buf);
	
	index = 0;
	while(buf[index++] != '\0');
	

	if(buf[index-2] != '/') {
	    
	    buf[index-1] = '/';
	    buf[index] = '\0';
	}
	
	print_out("buf is %s\n",buf);

	index = tarfs_open(buf);

	if(index == -1) {
		print_out("\nNo such file or directory");
		return;
	}

	if(fs_nodes[index].typeflag != DIRTYPE) {
		print_out("\n%s is not a directory!!",dir_name);	
		return;
	}

	
	

	

	cwd_index = index;
	//strcat(cwd,buf);
		
	print_out("cwd is %s\n",fs_nodes[cwd_index].name);
	// Check if the 
	
}
