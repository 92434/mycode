#ifndef _I2S_RECEIVER_GPIO_CONFIG_H
#define _I2S_RECEIVER_GPIO_CONFIG_H
typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

static gpio_chip_info_t gpio_chip_info[] = {
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_0,
		.ngpios = 1,
#if defined(KC705_I2S_RECEIVER)
#define DEFINED_I2S 1

#if (DEFINED_I2S == 0)
		.ngpio = {10, 0},
#elif (DEFINED_I2S == 1) 
#warning("DEFINED_I2S == 1")
		.ngpio = {30, 0},
#endif//if (DEFINED_I2S == 0)
#endif//#if defined(KC705_I2S_RECEIVER)
	},
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

#endif//#ifndef _I2S_RECEIVER_GPIO_CONFIG_H
