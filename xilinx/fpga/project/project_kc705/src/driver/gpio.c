#include <linux/gpio.h>

#include "utils/xiaofei_debug.h"

//AXI GPIO Register Summary
#define GPIO_DATA 0x000 //Channel1 AXI GPIO Data Register.
#define GPIO_TRI 0x004 //Channel1 AXI GPIO 3-state Control Register.
#define GPIO2_DATA 0x008 //Channel2 AXI GPIO Data Register.
#define GPIO2_TRI 0x00C //Channel2 AXI GPIO 3-state Control.
#define GIER 0x11C //Global Interrupt Enable Register.
#define IP_IER 0x128 //IP Interrupt Enable Register (IP IER).
#define IP_ISR 0x120 //IP Interrupt Status Register.

#define DEFAULT_CHANNEL_PIN_NUMBER 32
#define NAME_SIZE 20
#define GPIO_CHIP_CHANNEL_SIZE 2

typedef struct {
	char name[NAME_SIZE];
	struct gpio_chip chip;
	uint8_t *base_addr;
	unsigned int channel_index;
	bool initilized;
} kc705_gpio_chip_channel_t;

typedef struct {
	kc705_gpio_chip_channel_t gpio_channel[GPIO_CHIP_CHANNEL_SIZE];
} kc705_gpio_chip_t;

static int gpio_channel_get(uint8_t *gpio_bank_base, unsigned int channel_index, unsigned int offset) {
	uint32_t value = 0;
	switch(channel_index) {
		case 0:
			value = readl(gpio_bank_base + GPIO_DATA);
			break;
		case 1:
			value = readl(gpio_bank_base + GPIO2_DATA);
			break;
		default:
			break;
	}

	return (value >> offset) & 1;
}

static void gpio_channel_set(uint8_t *gpio_bank_base, unsigned int channel_index, unsigned int offset, bool high) {
	uint32_t value;

	if(high) {
		switch(channel_index) {
			case 0:
				value = readl(gpio_bank_base + GPIO_DATA) | BITMASK(offset);
				writel(value, gpio_bank_base + GPIO_DATA);
				break;
			case 1:
				value = readl(gpio_bank_base + GPIO2_DATA) | BITMASK(offset);
				writel(value, gpio_bank_base + GPIO2_DATA);
				break;
			default:
				break;
		}
	} else {
		switch(channel_index) {
			case 0:
				value = readl(gpio_bank_base + GPIO_DATA) & ~(BITMASK(offset));
				writel(value, gpio_bank_base + GPIO_DATA);
				break;
			case 1:
				value = readl(gpio_bank_base + GPIO2_DATA) & ~(BITMASK(offset));
				writel(value, gpio_bank_base + GPIO2_DATA);
				break;
			default:
				break;
		}
	}

}

static int gpio_channel_set_input(uint8_t *gpio_bank_base, unsigned int channel_index, unsigned int offset) {
	uint32_t value;
	switch(channel_index) {
		case 0:
			value = readl(gpio_bank_base + GPIO_TRI) | BITMASK(offset);
			writel(value, gpio_bank_base + GPIO_TRI);
			break;
		case 1:
			value = readl(gpio_bank_base + GPIO2_TRI) | BITMASK(offset);
			writel(value, gpio_bank_base + GPIO2_TRI);
			break;
		default:
			break;
	}

	return 0;
}

static int gpio_channel_set_output(uint8_t *gpio_bank_base, unsigned int channel_index, unsigned int offset) {
	uint32_t value;
	switch(channel_index) {
		case 0:
			value = readl(gpio_bank_base + GPIO_TRI) & ~(BITMASK(offset));
			writel(value, gpio_bank_base + GPIO_TRI);
			break;
		case 1:
			value = readl(gpio_bank_base + GPIO2_TRI) & ~(BITMASK(offset));
			writel(value, gpio_bank_base + GPIO2_TRI);
			break;
		default:
			break;
	}

	return 0;
}

static int gpio_channel_get_direction(uint8_t *gpio_bank_base, unsigned int channel_index, unsigned int offset) {
	uint32_t value = 0;
	switch(channel_index) {
		case 0:
			value = readl(gpio_bank_base + GPIO_TRI);
			break;
		case 1:
			value = readl(gpio_bank_base + GPIO2_TRI);
			break;
		default:
			break;
	}

	return ((value >> offset) & 1);
}

//static int gpio_bank_set_global_interrupt(uint8_t *gpio_bank_base, bool enable) {
//	uint32_t value = 0;
//
//	if(enable) {
//		value = BITMASK(31);
//	}
//	
//	writel(value, gpio_bank_base + GIER);
//
//	return 0;
//}
//
//static int gpio_channel_set_interrupt(uint8_t *gpio_bank_base, unsigned int channel_index, bool enable) {
//	uint32_t value;
//
//	value = readl(gpio_bank_base + IP_IER);
//
//	switch(channel_index) {
//		case 0:
//		case 1:
//			break;
//		default:
//			return -1;
//			break;
//	}
//
//	if(enable) {
//		value |= BITMASK(channel_index);
//	} else {
//		value &= !(BITMASK(channel_index));
//	}
//	writel(value, gpio_bank_base + IP_IER);
//
//	return 0;
//}

static int kc705_gpio_get(struct gpio_chip *chip, unsigned offset) {
	kc705_gpio_chip_channel_t *gpio = container_of(chip, kc705_gpio_chip_channel_t, chip);

	return gpio_channel_get(gpio->base_addr, gpio->channel_index, offset);
}

static void kc705_gpio_set(struct gpio_chip *chip, unsigned offset, int value) {
	kc705_gpio_chip_channel_t *gpio = container_of(chip, kc705_gpio_chip_channel_t, chip);
	gpio_channel_set(gpio->base_addr, gpio->channel_index, offset, !!value);
}

static int kc705_gpio_direction_input(struct gpio_chip *chip, unsigned offset) {
	kc705_gpio_chip_channel_t *gpio = container_of(chip, kc705_gpio_chip_channel_t, chip);

	return gpio_channel_set_input(gpio->base_addr, gpio->channel_index, offset);
}

static int kc705_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value) {
	kc705_gpio_chip_channel_t *gpio = container_of(chip, kc705_gpio_chip_channel_t, chip);

	gpio_channel_set_output(gpio->base_addr, gpio->channel_index, offset);
	gpio_channel_set(gpio->base_addr, gpio->channel_index, offset, !!value);

	return 0;
}

static int kc705_gpio_get_direction(struct gpio_chip *chip, unsigned offset) {
	kc705_gpio_chip_channel_t *gpio = container_of(chip, kc705_gpio_chip_channel_t, chip);

	return gpio_channel_get_direction(gpio->base_addr, gpio->channel_index, offset);
}

static struct gpio_chip chip_example = {
	//.label = label,
	//.request = request,
	//.free = free,
	.base = -1,
	.get_direction = kc705_gpio_get_direction,
	.direction_input = kc705_gpio_direction_input,
	.direction_output = kc705_gpio_direction_output,
	.get = kc705_gpio_get,
	.set = kc705_gpio_set,
	//.dbg_show = dbg_show,
	//.to_irq = to_irq,
	.ngpio = DEFAULT_CHANNEL_PIN_NUMBER,
};

void *kc705_add_gpio_chip(uint8_t *base_addr, int ngpios, int *pngpio, char *namefmt, ...) {
	void *ret;
	int i;
	char base_name[NAME_SIZE] = {0};
	kc705_gpio_chip_t *kc705_gpio_chip;
	va_list args;

	va_start(args, namefmt);
	vsnprintf(base_name, sizeof(base_name), namefmt, args);
	va_end(args);

	kc705_gpio_chip = (kc705_gpio_chip_t *)vzalloc(sizeof(kc705_gpio_chip_t));
	if(kc705_gpio_chip == NULL) {
		mydebug("alloc kc705_gpio_chip failed.\n");
		ret = NULL;
		return ret;
	}

	for(i = 0; i < ngpios; i++) {
		kc705_gpio_chip->gpio_channel[i].base_addr = base_addr;
		kc705_gpio_chip->gpio_channel[i].channel_index = i;
		snprintf(kc705_gpio_chip->gpio_channel[i].name, sizeof(kc705_gpio_chip->gpio_channel[i].name), "%s_channel%d", base_name, i);
		kc705_gpio_chip->gpio_channel[i].chip = chip_example;
		kc705_gpio_chip->gpio_channel[i].chip.label = kc705_gpio_chip->gpio_channel[i].name;
		kc705_gpio_chip->gpio_channel[i].chip.ngpio = pngpio[i];
		if(gpiochip_add(&(kc705_gpio_chip->gpio_channel[i].chip)) != 0) {
			mydebug("gpiochip_add %d failed.\n", i);
		} else {
			kc705_gpio_chip->gpio_channel[i].initilized = true;
		}
		mydebug("kc705_gpio_chip->gpio_channel[i].chip.base:%d\n", kc705_gpio_chip->gpio_channel[i].chip.base);
	}
	
	ret = (void *)kc705_gpio_chip;
	return ret;
}

void kc705_remove_gpio_chip(void *ppara) {
	kc705_gpio_chip_t *kc705_gpio_chip = (kc705_gpio_chip_t *)ppara;
	int i;
	
	for(i = 0; i < GPIO_CHIP_CHANNEL_SIZE; i++) {
		if(kc705_gpio_chip->gpio_channel[i].initilized) {
			gpiochip_remove(&(kc705_gpio_chip->gpio_channel[i].chip));
		}
	}

	vfree(kc705_gpio_chip);
}
