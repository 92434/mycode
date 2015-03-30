#include "common.h"
#include "inputdev.h"
#include "gpio_interrupt.h"
#include "gt208l.h"

#include "gpio_gen.h"

#include <linux/slab.h>
#include <linux/workqueue.h>

#define GPIO_GT208L_INT	118

//#define KEY_DEBUG

struct fpanel {
	struct inputdev_info inputdev;
	struct gpioint gpioint;
	struct work_struct work;
};

static struct fpanel *fp = NULL;
static const unsigned int keytable[] =
{
	[0x00] = KEY_POWER,
	[0x01] = KEY_MENU,
	[0x02] = KEY_LEFT,	
	[0x03] = KEY_RIGHT,
	[0x04] = KEY_UP,
	[0x05] = KEY_DOWN,
	[0x06] = KEY_OK,
};

static void wq_proc(struct work_struct *w)
{
	u8 regval;
	int i;
	static int lastkey = 0xff;

	regval = gt208l_read_key();

	for(i = 0; i < ARRAY_SIZE(keytable); i++) {
		if(regval & (1 << i))
			break;
	}

	if(i < 7) {
		input_report_key(fp->inputdev.input, keytable[i], 1);
		if(lastkey != 0xff){
			gt280l_set_led((6 - lastkey)%7, 1);
		}
		lastkey = i;
		gt280l_set_led((6 - i)%7, 0);
	} else {
		input_report_key(fp->inputdev.input, keytable[lastkey], 0);
		if (lastkey != 0xff && regval == 0){
			gt280l_set_led((6-lastkey)%7, 1);
		}
		lastkey = 0xff;			
	}
}

static CNXT_STATUS button_int_handler(u_int32 uIntID, bool bFIQ)
{
	schedule_work(&fp->work);

	return CNXT_STATUS_INT_HANDLED;
}

static int __init fpanel_init(void)
{
	int err;

	fp = kmalloc(sizeof(struct fpanel), GFP_KERNEL);
	if(fp == NULL) {
		printk(KERN_ERR "fpanel_init() malloc  failed\n");
		return -ENOMEM;
	}

	//create input device
	fp->inputdev.name			= "frontpanel buttons";
	fp->inputdev.keytable		= keytable;
	fp->inputdev.keytable_size	= sizeof(keytable);
	fp->inputdev.keycodesize	= sizeof(keytable[0]);
	fp->inputdev.keycodemax		= ARRAY_SIZE(keytable);

	err = inputdev_init(&fp->inputdev);
	if(err != 0)
		return err;

	//config gpio as interrupt input and its handler function
	fp->gpioint.gpio	= GPIO_GT208L_INT;
	fp->gpioint.edge	= NEG_EDGE;
	fp->gpioint.isr	= button_int_handler;

	err = gpio_interrupt_init(&fp->gpioint);
	if(err != 0)
		goto out1;

	//init chip GT280L
	err = gt208l_init();
	if(err != 0)
		goto out2;

	//init bottom-half workqueue for interrupt 
	INIT_WORK(&fp->work, wq_proc);

	return 0;

out1:
	inputdev_exit(&fp->inputdev);
out2:
	gpio_interrupt_exit(&fp->gpioint);
	return err;
}

static void __exit fpanel_exit(void)
{
	inputdev_exit(&fp->inputdev);
	gpio_interrupt_exit(&fp->gpioint);
	gt208l_exit();
}

module_init(fpanel_init);
module_exit(fpanel_exit);
MODULE_DESCRIPTION("front panel leds and touch-keys.");
MODULE_LICENSE("GPL");
