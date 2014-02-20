#ifndef __FILE_H
#define __FILE_H

struct file {
	char filename[128];
	uint64_t sys_fd;
	uint64_t file_offset;
	// Determines whether this file struct is open/closed

	unsigned char fd_state; // Must be init to 0 in the beginning
};

#endif /* __FILE_H */
