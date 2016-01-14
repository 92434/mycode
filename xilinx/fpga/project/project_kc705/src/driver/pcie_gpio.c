#include "pcie.h"
#if defined(KC705_DVBS2)
#include "dvbs2_gpio_config.h"
#elif defined(KC705_CSA)
#include "csa_gpio_config.h"
#endif
#include "utils/xiaofei_debug.h"

int alloc_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;

	if(GPIOCHIP_MAX == 0) {
		return ret;
	}

	kc705_pci_dev->gpiochip = (void **)vzalloc(sizeof(void *) * GPIOCHIP_MAX);
	if(kc705_pci_dev->gpiochip == NULL) {
		mydebug("alloc kc705_pci_dev->gpiochip failed.\n");
		ret = -1;
	}

	return ret;
}

void free_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev) {
	if(kc705_pci_dev->gpiochip != NULL) {
		vfree(kc705_pci_dev->gpiochip);
		kc705_pci_dev->gpiochip = NULL;
	}
}

#include "gpio.h"
#include "gpio_proc.h"
void add_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < GPIOCHIP_MAX; i++) {
		uint8_t *gpio_chip_base_addr = kc705_pci_dev->bar_info[0].base_vaddr + gpio_chip_info[i].chip_addr_offset;
		int ngpios = gpio_chip_info[i].ngpios;
		int *pngpio = gpio_chip_info[i].ngpio;
		kc705_pci_dev->gpiochip[i] = kc705_add_gpio_chip(gpio_chip_base_addr, ngpios, pngpio, "gpiochip%d", i);
	}

	create_gpio_proc_dir();
}

void remove_kc705_gpio(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	remove_gpio_proc_dir();

	for(i = 0; i < GPIOCHIP_MAX; i++) {
		if(kc705_pci_dev->gpiochip[i] != NULL) {
			kc705_remove_gpio_chip(kc705_pci_dev->gpiochip[i]);
			kc705_pci_dev->gpiochip[i] = NULL;
		}
	}
}
