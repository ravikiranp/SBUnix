TEAM: SAMHITHA KV and RAVIKIRAN PATIL

Contents
--------
1) Build Command
2) Shell
3) Auto-growing Stack
4) Segv Handling
5) stdin/stdout/stderr
6) libc API
7) Binaries: sleep,ls,ps,sh(sh is broken)


-----------------------------------------------
Build Command
-----------------------------------------------

--------------------------------------------------------------------------
SHELL 
--------------------------------------------------------------------------
The shell process is the task that is first created assigned task id 1. 
It is this process that the kernel switches to first from ring 0 to ring 3.
Shell cannot be istantiated again by the user.

1) PATH is maintained as a global variable in the Shell binary.
   It has to be set MANUALLY. Each directory delimited by a ':'
2) Once a binary name is typed at the shell and it is found in
   one of the PATH directories, the shell executes it with the
   arguments that follow the filename.
3) In case the argument is a filename, the complete path has to be given
   SBUnix> sh temp/exec.sh
4) Upto 5 arguments can be passed to a binary when it is being executed.
   Limit: Each argument can be upto 30 characters.
   
---------------------------------------------------------------------------   
AUTO-GROWING STACK ( MAX-LIMIT : 4K)
---------------------------------------------------------------------------
1) Three pages are allocated for the stack for every process, in the order
   of kernel stack, ulimit guard page and user-stack in increasing 
   addresses.
2) u-limit guard page is mapped without user permission, and when the
   user stack grows beyond its allocated space into the guard page,
   a user-mode fault is generated and the faulting process is killed.
3) The maximum limit that can be configured by the user with the ulimit
   is 4K.

---------------------------------------------------------------------------
SEGV Handling
---------------------------------------------------------------------------
1) Any access(user/superviser) to the page at virtual address 0x0000 
   generates a pagefault and after the check the current task is killed.
   

---------------------------------------------------------------------------
stdin/stdout/stderr
---------------------------------------------------------------------------
1) A page is mapped per process as a stdin/stdout stream. printf and
   scanf functions write-to and read-from this page.


---------------------------------------------------------------------------
libc API 
---------------------------------------------------------------------------
1) int printf(const char* format,...);
2) int scanf(const char* format,...);
3) void* malloc(uint64_t size_bytes);
4) uint16_t fork();
5) void execvpe(const char* filename,int argc,char* argv[]);
6) void sleep(uint16_t seconds);
7) void exit();
8) void wait();
9) void waitpid(uint16_t pid);
10) uint16_t getpid();



11) int open(const char* filename);
12) int read(int fd,void* buf,uint64_t size_bytes);
13) int close(fd);

For opendir,readdir and closedir a new data-structure is defined
to replace struct DIR.

struct dir_struct {
	char d_name[128];
};

The dirent struct is defined as follows:

struct dirent {
	char d_name[128];
};

14)struct dir_struct* opendir(const char* filename);
15) struct dirent* readdir(struct dir_struct* dirp);
16) int closedir(struct dir_struct* dirp);
