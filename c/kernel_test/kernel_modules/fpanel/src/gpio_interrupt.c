#include "gpio_interrupt.h"
#include "gpio_gen.h"
#include <linux/sched.h>

int gpio_interrupt_init(struct gpioint *info)
{
	CNXT_STATUS ret = CNXT_STATUS_OK;
	u_int32 gpio = info->gpio;
	u_int32 edge = info->edge;
	PFNISR isr = info->isr;

	{
		int printk(const char *fmt, ...);
		printk("[xiaofei]: %s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
		printk("[xiaofei]: %x\n", ret);
		printk("[xiaofei]: %x\n", (unsigned int)info->gpio);
	}
	//gpio = info->gpio;
	ret = cnxt_gpio_set_input(gpio);
	if(ret != CNXT_STATUS_OK)
		goto fail;

	//gpio = info->gpio;
	//edge = info->edge;
	ret = cnxt_gpio_set_int_edge(gpio, edge);
	if(ret != CNXT_STATUS_OK)
		goto fail;

	//gpio = info->gpio;
	//isr = info->isr;
	ret = cnxt_gpio_int_register_isr(gpio, isr, FALSE);
	if(ret != CNXT_STATUS_OK)
		goto fail;

	//gpio = info->gpio;
	ret = cnxt_gpio_int_enable(gpio);
	if(ret != CNXT_STATUS_OK)
		goto fail;

	return 0;
fail:
	return -1;
}

int gpio_interrupt_exit(struct gpioint *info)
{
	CNXT_STATUS ret;

	ret = cnxt_gpio_int_disable(info->gpio);
	if(ret != CNXT_STATUS_OK)
		return -1;

	ret = cnxt_gpio_int_deregister_isr(info->gpio, info->isr);
	if(ret != CNXT_STATUS_OK)
		return -1;

	return ret;
}
