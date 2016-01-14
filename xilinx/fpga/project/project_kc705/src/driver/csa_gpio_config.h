#ifndef _CSA_GPIO_CONFIG_H
#define _CSA_GPIO_CONFIG_H
typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

static gpio_chip_info_t gpio_chip_info[] = {
#if defined(TEST_FMC_HPC_GPIOS)
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_0,
		.ngpios = 2,
		.ngpio = {32, 32},

	},
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_1,
		.ngpios = 2,
		.ngpio = {32, 16},

	},
#elif defined(TEST_FMC_LPC_GPIOS)
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_2,
		.ngpios = 2,
		.ngpio = {32, 6},

	},
#endif
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

#endif//#ifndef _CSA_GPIO_CONFIG_H
