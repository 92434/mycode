#ifndef _KC705_TSP_GPIO_CONFIG_H
#define _KC705_TSP_GPIO_CONFIG_H
#define DEFINED_TSP 2

typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

static gpio_chip_info_t gpio_chip_info[] = {
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_0,
		.ngpios = 1,
#if (DEFINED_TSP == 0)
		.ngpio = {2},
#elif (DEFINED_TSP == 1)
		.ngpio = {2},
#elif (DEFINED_TSP == 2)
#warning("DEFINED_TSP == 2")
		.ngpio = {20},
#endif//if (DEFINED_TSP == 0)

	},
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

#endif//#ifndef _KC705_TSP_GPIO_CONFIG_H
