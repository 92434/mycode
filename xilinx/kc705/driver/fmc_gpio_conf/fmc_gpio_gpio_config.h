#ifndef _FMC_GPIO_GPIO_CONFIG_H
#define _FMC_GPIO_GPIO_CONFIG_H
typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

#define FMC_HPC 1

static gpio_chip_info_t gpio_chip_info[] = {
#if FMC_HPC
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_0,
		.ngpios = 2,
		.ngpio = {32, 32},
	},
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_1,
		.ngpios = 2,
		.ngpio = {32, 26},
	},
#else
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_2,
		.ngpios = 2,
		.ngpio = {32, 32},
	},
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_3,
		.ngpios = 1,
		.ngpio = {9},
	},
#endif
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

#endif//#ifndef _FMC_GPIO_GPIO_CONFIG_H
