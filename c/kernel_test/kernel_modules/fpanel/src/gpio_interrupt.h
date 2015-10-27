#ifndef __GPIO_INTERRUPT_H__
#define __GPIO_INTERRUPT_H__

#include "common.h"

struct gpioint
{
	u_int32 gpio;
	u_int32 edge;
	PFNISR isr;
};

int gpio_interrupt_init(struct gpioint *info);
int gpio_interrupt_exit(struct gpioint *info);

#endif