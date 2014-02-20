#ifndef __IDT_H
#define __IDT_H

#include <defs.h>

/* Type attribute Constants */
#define PRES 		0x80
#define DPL_0 		0x00
#define DPL_1 		0x20
#define DPL_2 		0x40
#define DPL_3 		0x60
#define S 		0x00
#define INTR_GATE 	0x0E 

void load_idt();
	
#endif /* __IDT_H */
