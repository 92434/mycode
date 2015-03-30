#include "inputdev.h"
#include <linux/slab.h>

int inputdev_init(struct inputdev_info *info)
{
	int i, err;
	struct input_dev *input;
	input = input_allocate_device();
	if (!input)
		return -ENOMEM;

	input->name = info->name;
	input->id.bustype = BUS_HOST;
	input->evbit[0] = BIT(EV_KEY);
	input->keycode = kmemdup(info->keytable, info->keytable_size, GFP_KERNEL);
	if (!input->keycode) {
		err = -ENOMEM;
		goto fail;
	}

	input->keycodesize = info->keycodesize;
	input->keycodemax = info->keytable_size;
	for (i = 0; i < info->keycodemax; i++)
		set_bit(info->keytable[i], input->keybit);

	err = input_register_device(input);
	if (err) {
		printk(KERN_ERR "register device failed\n");
		goto fail_free_keymap;
	}

	info->input = input;

	return 0;

fail_free_keymap:
	kfree(input->keycode);
fail:
	input_free_device(input);
	return err;
}

void inputdev_exit(struct inputdev_info *info)
{
	kfree(info->input->keycode);
	input_unregister_device(info->input);
}