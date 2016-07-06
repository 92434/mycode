#ifndef _GPIO_H
#define _GPIO_H
void *kc705_add_gpio_chip(uint8_t *base_addr, int ngpios, int *pngpio, char *namefmt, ...);
void kc705_remove_gpio_chip(void *ppara);
#endif//#ifndef _GPIO_H
