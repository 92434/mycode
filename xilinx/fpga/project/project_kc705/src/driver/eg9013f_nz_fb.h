#ifndef _EG9013F_NZ_H
#define _EG9013F_NZ_H
#define EG9013F_NZ_WIDTH 640
#define EG9013F_NZ_HEIGHT 480
#define EG9013F_NZ_SIZE (EG9013F_NZ_WIDTH * EG9013F_NZ_HEIGHT / 8)
int eg9013f_nz_fb_add(kc705_pci_dev_t *kc705_pci_dev);
int eg9013f_nz_fb_remove(kc705_pci_dev_t *kc705_pci_dev);
#endif//#ifndef _EG9013F_NZ_H
