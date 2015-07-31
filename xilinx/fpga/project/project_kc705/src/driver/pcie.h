#ifndef _PCIE_H
#define _PCIE_H
/** @name Macros for PCI probing
 * @{
 */
#define PCI_VENDOR_ID_DMA   0x10EE      /**< Vendor ID - Xilinx */
#define PCI_DEVICE_ID_DMA   0x7012      /**< Xilinx's Device ID */

//Base Address
//regs
#define BASE_AXI_PCIe 0x81000000
#define BASE_Translation_BRAM 0x81000000
#define BASE_AXI_PCIe_CTL 0x81008000
#define BASE_AXI_CDMA_LITE 0x8100c000
//memory
#define BASE_AXI_DDR_ADDR 0x00000000
#define BASE_AXI_PCIe_BAR0 0x80800000
#define BASE_AXI_PCIe_BAR1 0x80000000

//PCIe:BAR0 Address Offset for the accessible Interfaces
#define OFFSET_Translation_BRAM (BASE_Translation_BRAM - BASE_AXI_PCIe)
#define OFFSET_AXI_PCIe_CTL (BASE_AXI_PCIe_CTL - BASE_AXI_PCIe)
#define OFFSET_AXI_CDMA_LITE (BASE_AXI_CDMA_LITE - BASE_AXI_PCIe)

#define AXI_PCIe_BAR0_SIZE 0x10000
#define AXI_PCIe_BAR1_SIZE 0x10000

#define MAX_BARS 6/**< Maximum number of BARs */

#define DM_CHANNEL_TX_SIZE 16
#define DM_CHANNEL_RX_SIZE 16

typedef struct {
	struct pci_dev *pdev; /**< PCI device entry */
	u32 bar_mask;                    /**< Bitmask for BAR information */
	struct {
		unsigned long base_phyaddr; /**< Base address of device memory */
		unsigned long base_len; /**< Length of device memory */
		void __iomem * base_vaddr; /**< VA - mapped address */
	} bar_info[MAX_BARS];
	int msi_enable;
	int bar_map_memory_size[MAX_BARS];
	void *bar_map_memory[MAX_BARS];
	dma_addr_t bar_map_addr[MAX_BARS];
	struct work_struct work;
} kc705_pci_dev_t;

extern struct pci_driver kc705_pcie_driver;
void dump_memory(void *addr, int size);
void dump_regs(void);
#endif //#define _PCIE_H
