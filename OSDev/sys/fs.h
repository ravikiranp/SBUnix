#ifndef __FS_H
#define __FS_H

#include <defs.h>



#define REGTYPE  0            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  1            /* link */
#define SYMTYPE  2            /* reserved */
#define CHRTYPE  3            /* character special */
#define BLKTYPE  4            /* block special */
#define DIRTYPE  5            /* directory */
#define FIFOTYPE 6            /* FIFO special */
#define CONTTYPE 7            /* reserved */

extern int kern_open(const char *);
extern int kern_close(int);
extern int kern_read(int,void *,uint64_t);

struct fs_node;

struct dir_struct {
        char d_name[128];
        uint64_t node_index;
        uint64_t next_node_index;
};

struct dirent {
	char d_name[128];
	uint64_t d_reclen; // Length of this record
	uint64_t d_off;  // offset to the next dir
	unsigned char isDir;
};

extern struct dir_struct * kern_opendir(const char *);
extern struct dirent * kern_readdir(struct dir_struct *);
extern int closedir(struct dir_struct *);
 
typedef struct fs_node {
   char 		name[128];
   unsigned char 	perm_flag;
   unsigned char	typeflag;
   uint64_t 		size;

   // Pointers to parent fs_nodes
   uint64_t parent_dir_node_index;

   // Struct to hold directory info if this is a directory node
   struct dirent d_entry;

   uint64_t file_header_offset;

} fs_node_t;


extern fs_node_t *fs_root;


void register_fs(uint64_t entry_point);


#endif /* __FS_H */
