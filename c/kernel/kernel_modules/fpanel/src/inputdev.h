#ifndef __INPUT_DEV_H__
#define __INPUT_DEV_H__

#include "common.h"
#include <linux/input.h>

struct inputdev_info
{
	const char			*name;
	const unsigned int 	*keytable;
	size_t				keytable_size;
	unsigned int		keycodesize;
	unsigned int 		keycodemax;
	struct input_dev	*input;
};

int inputdev_init(struct inputdev_info *info);
void inputdev_exit(struct inputdev_info *info);

#endif