#ifndef _CSA_DEBUG_GPIO_CONFIG_H
#define _CSA_DEBUG_GPIO_CONFIG_H
typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

static gpio_chip_info_t gpio_chip_info[] = {
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

#endif//#ifndef _CSA_DEBUG_GPIO_CONFIG_H
