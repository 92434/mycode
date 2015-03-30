/*
 * string.c
 */

#include "mboot.h"
#include <stdarg.h>


u64 patten[8] = {
	0x0000000000000000,
	0xffffffffffffffff,
	0xaaaaaaaaaaaaaaaa,
	0xcccccccccccccccc,
	0xf0f0f0f0f0f0f0f0,
	0xff00ff00ff00ff00,
	0xffff0000ffff0000,
	0xffffffff00000000,
};

void mem_test(void)
{
	int i, j;
	u64 pt, data;

	for(i=0; i<8; i++){
		pt = patten[i];
		printk("check patten %08x%08x ...\n", (pt>>32), (pt&0xffffffff));
		for(j=0; j<0x00100000; j+=8){
			*(u64*)(0xa0000000+j) = pt;
		}

		for(j=0; j<0x00100000; j+=8){
			data = *(u64*)(0xa0000000+j);
			if(data!=pt){
				printk("  error at %08x: %08x%08x\n", (0xa0000000+j), (data>>32), (data&0xffffffff));
				return;
			}
		}
	}

	printk("check done. no error found.\n");
}

