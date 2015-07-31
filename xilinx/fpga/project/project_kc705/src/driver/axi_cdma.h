#ifndef _AXI_CDMA_H
#define _AXI_CDMA_H
//AXI CDMA Register Summary
#define CDMA_CR 0x00 //CDMA Control.
#define CDMA_SR 0x04 //CDMA Status.
#define CDMA_CURDESC_PNTR 0x08 //Current Descriptor Pointer.
//#define Reserved 0x0C //N/A
#define CDMA_TAILDESC_PNTR 0x10 //Tail Descriptor Pointer.
//#define Reserved 0x14 //N/A
#define CDMA_SA 0x18 //Source Address.
//#define Reserved 0x1C //N/A
#define CDMA_DA 0x20 //Destination Address.
//#define Reserved 0x24 //N/A
#define CDMA_BTT 0x28 //Bytes to Transfer.

#define SG_DESCRIPTOR_SIZE 0x40

////Transfer Descriptor Word Summary
//#define DES_NXTDESC_PNTR 0x00 //[31:5]
////#define DES_RESERVED 0x04
//#define DES_SA 0x08
////#define DES_RESERVED 0x0c
//#define DES_DA 0x10
////#define DES_RESERVED 0x14
//#define DES_CONTROL 0x18 //[22:0] size to transfer
//#define DES_STATUS 0x1c //bit31:Cmplt bit30:DMADecErr bit29:DMASlvErr bit28:DMAIntErr

typedef union {
	struct {
		uint32_t NXTDESC_PNTR;
		uint32_t PAD0;
		uint32_t SA;
		uint32_t PAD1;
		uint32_t DA;
		uint32_t PAD2;
		uint32_t CONTROL;
		uint32_t STATUS;
	} des;
	unsigned char des_space[0x40];
} kc705_transfer_descriptor_t;

typedef struct {
	struct list_head sg_item;
	kc705_transfer_descriptor_t des;
} kc705_sg_item_info_t;

extern struct completion cmp;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
irqreturn_t isr(int irq, void *dev_id, struct pt_regs *regs);
#else
irqreturn_t isr(int irq, void *dev_id);
#endif
int prepare_bars_map(kc705_pci_dev_t *kc705_pci_dev);
int init_dma(kc705_pci_dev_t *kc705_pci_dev);
int alloc_sg_list_chain(uint32_t tx_axiaddr, uint32_t tx_size, uint32_t rx_axiaddr, uint32_t rx_size);
void free_sg_list_chain(void);
int dma_worker_thread(void *ppara);
#endif// #ifndef _AXI_CDMA_H
