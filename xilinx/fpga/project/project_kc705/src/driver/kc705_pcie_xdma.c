#include <linux/module.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>


#define mydebug(format, ...) printk(KERN_DEBUG "[%s:%s:%d]<%s>:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, MODULE_NAME, ## __VA_ARGS__)
#define myprintf(format, ...) printk(KERN_ERR format, ## __VA_ARGS__)

#define BITMASK(loc) (1 << loc)

//#if defined(i386)
//#else
//#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
#else
#define __devinit
#define __devexit
#define __devexit_p(f) f
#endif

/** @name Macros for PCI probing
 * @{
 */
#define PCI_VENDOR_ID_DMA   0x10EE      /**< Vendor ID - Xilinx */

#define PCI_DEVICE_ID_DMA   0x7012      /**< Xilinx's Device ID */

/** Driver information */
#define MODULE_NAME "kc705_pcie_xdma"
#define DRIVER_DESCRIPTION "kc705 pcie xdma driver"
#define DRIVER_VERSION "1.0"

//Base Address
#define BASE_AXI_PCIe 0x80800000
#define BASE_AXI_DMA_LITE 0x80800000
#define BASE_AXI_PCIe_CTL 0x80804000
#define BASE_AXI_PCIe_BAR0 0x80000000

//PCIe:BAR0 Address Offset for the accessible Interfaces
#define OFFSET_AXI_PCIe_CTL (BASE_AXI_PCIe_CTL - BASE_AXI_PCIe)
#define OFFSET_AXI_DMA_LITE (BASE_AXI_DMA_LITE - BASE_AXI_PCIe)
//#define OFFSET_AXI_PCIe_BAR0 (BASE_AXI_PCIe_BAR0 - BASE_AXI_PCIe)

//AXI DMA Register Summary
#define MM2S_DMACR 0x00 //MM2S DMA Control register
#define MM2S_DMASR 0x04 //MM2S DMA Status register
//08h – 14h Reserved N/A
#define MM2S_SA 0x18 //MM2S Source Address. Lower 32 bits of address.
#define MM2S_SA_MSB 0x1C //MM2S Source Address. Upper 32 bits of address.
#define MM2S_LENGTH 0x28 //MM2S Transfer Length (Bytes)
#define S2MM_DMACR 0x30 //S2MM DMA Control register
#define S2MM_DMASR 0x34 //S2MM DMA Status register38h – 44h Reserved N/A
#define S2MM_DA 0x48 //S2MM Destination Address. Lower 32 bit address.
#define S2MM_DA_MSB 0x4C //S2MM Destination Address. Upper 32 bit address.
#define S2MM_LENGTH 0x58 //S2MM Buffer Length (Bytes)

//Address Map for the AXI to PCIe Address Translation Registers
#define AXIBAR2PCIEBAR_0U 0x208 //default be set to 0
#define AXIBAR2PCIEBAR_0L 0x20c //default be set to 0xa0000000
#define AXIBAR2PCIEBAR_1U 0x210 //default be set to 0
#define AXIBAR2PCIEBAR_1L 0x214 //default be set to 0xc0000000

#define MAX_BARS 6/**< Maximum number of BARs */

//#define DM_CHANNEL_TX_SIZE 0x1000
//#define DM_CHANNEL_RX_SIZE 0x1000
#define DM_CHANNEL_TX_SIZE 0x8
#define DM_CHANNEL_RX_SIZE 0x8

#define XILINX_VDMA_LOOP_COUNT 1000000

/** Driver Module information */
MODULE_AUTHOR("xiaofei");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
MODULE_ALIAS(MODULE_NAME);

/** PCI device structure which probes for targeted design */
static struct pci_device_id ids[] = {
	{
		PCI_VENDOR_ID_DMA,
		PCI_DEVICE_ID_DMA,
		PCI_ANY_ID,
		PCI_ANY_ID,
		0,
		0,
		0UL
	},
	{}     /* terminate list with empty entry */
};

/**
 * Macro to export pci_device_id to user space to allow hot plug and
 * module loading system to know what module works with which hardware device
 */
MODULE_DEVICE_TABLE(pci, ids);

/*@}*/

///** Engine bitmask is 64-bit because there are 64 engines */
//#define DMA_ENGINE_PER_SIZE     0x100   /**< Separation between engine regs */
//#define DMA_OFFSET              0       /**< Starting register offset */
///**< Size of DMA engine reg space */
//#define DMA_SIZE                (MAX_DMA_ENGINES * DMA_ENGINE_PER_SIZE)
//
///**
// * Default S2C and C2S descriptor ring size.
// * BD Space needed is (DMA_BD_CNT*sizeof(Dma_Bd)).
// */
//
//#define DMA_BD_CNT 3999


typedef void (*timer_func_t)(unsigned long);
typedef struct {
	struct timer_list *tl;
	unsigned long ms;
	timer_func_t func;
} timer_data_t;

typedef struct {
	struct pci_dev *pdev; /**< PCI device entry */
	u32 bar_mask;                    /**< Bitmask for BAR information */
	struct {
		unsigned long base_phyaddr; /**< Base address of device memory */
		unsigned long base_len; /**< Length of device memory */
		void __iomem * base_vaddr; /**< VA - mapped address */
	} bar_info[MAX_BARS];
	int msi_enable;
	void *dm_memory;
	dma_addr_t dm_addr;
} kc705_pci_dev_t;

static kc705_pci_dev_t *kc705_pci_dev = NULL;

static int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection) {
	uint32_t value = 0;
	int rtn = 0;

	if(count == 0) {
		count = XILINX_VDMA_LOOP_COUNT;
	}

	do {
		value = readl(paddr) & mask;
		count--;
	} while(count != 0 && value != expection);

	if(count == 0) {
		rtn = -1;
	}

	return rtn;
}

static int reset_dma(void) {//this operation will reset entire axi dma
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 

	//reset dma
	uint32_t default_value = 0x00010002;//0000_0000_0000_0001_0000_0000_0000_0010
	uint32_t value;
	uint32_t mask;
	int rtn = 0;

	//reset
	mask = BITMASK(2)/*reset bit*/;
	value = default_value | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	mydebug("dma_base_vaddr + MM2S_DMACR:%x\n", readl(dma_base_vaddr + MM2S_DMACR));
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMACR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}
	mydebug("dma_base_vaddr + MM2S_DMACR:%x\n", readl(dma_base_vaddr + MM2S_DMACR));

	return rtn;
}


static int default_config_dma(void) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
	uint64_t addr_tx = (uint64_t)BASE_AXI_PCIe_BAR0;
	uint64_t addr_rx = (uint64_t)BASE_AXI_PCIe_BAR0 + DM_CHANNEL_TX_SIZE;

	uint32_t value;
	uint32_t mask;
	int rtn = 0;

	mydebug("dma_base_vaddr:%p\n", dma_base_vaddr);
	mydebug("dma_base_vaddr + MM2S_DMACR:%x\n", readl(dma_base_vaddr + MM2S_DMACR));
	mydebug("dma_base_vaddr + MM2S_DMASR:%x\n", readl(dma_base_vaddr + MM2S_DMASR));
	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + MM2S_DMASR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}
	mydebug("dma_base_vaddr + MM2S_DMASR:%x\n", readl(dma_base_vaddr + MM2S_DMASR));
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + MM2S_DMACR) | mask;
	writel(value, dma_base_vaddr + MM2S_DMACR);
	value = (uint32_t)(addr_tx);
	writel(value, dma_base_vaddr + MM2S_SA);
	mydebug("dma_base_vaddr + MM2S_SA:%x\n", readl(dma_base_vaddr + MM2S_SA));
	//value = (uint32_t)(addr_tx >> 32);
	//writel(value, dma_base_vaddr + MM2S_SA_MSB);
	mydebug("dma_base_vaddr + MM2S_SA_MSB:%x\n", readl(dma_base_vaddr + MM2S_SA_MSB));
	//value = DM_CHANNEL_TX_SIZE;
	//writel(value, dma_base_vaddr + MM2S_LENGTH);
	mydebug("dma_base_vaddr + MM2S_DMACR:%x\n", readl(dma_base_vaddr + MM2S_DMACR));



	mydebug("dma_base_vaddr + S2MM_DMACR:%x\n", readl(dma_base_vaddr + S2MM_DMACR));
	mydebug("dma_base_vaddr + S2MM_DMASR:%x\n", readl(dma_base_vaddr + S2MM_DMASR));
	mask = BITMASK(0)/*RS bit*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);
	mask = BITMASK(0)/*Halted bit*/;
	rtn = wait_for_iostatus_timeout(0, dma_base_vaddr + S2MM_DMASR, mask, 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}
	mydebug("dma_base_vaddr + S2MM_DMASR:%x\n", readl(dma_base_vaddr + S2MM_DMASR));
	mask = BITMASK(12)/*IOC_IrqEn*/ | BITMASK(14)/*Err_IrqEn*/;
	value = readl(dma_base_vaddr + S2MM_DMACR) | mask;
	writel(value, dma_base_vaddr + S2MM_DMACR);
	value = (uint32_t)(addr_rx);
	writel(value, dma_base_vaddr + S2MM_DA);
	mydebug("dma_base_vaddr + S2MM_DA:%x\n", readl(dma_base_vaddr + S2MM_DA));
	//value = (uint32_t)(addr_rx >> 32);
	//writel(value, dma_base_vaddr + S2MM_DA_MSB);
	mydebug("dma_base_vaddr + S2MM_DA_MSB:%x\n", readl(dma_base_vaddr + S2MM_DA_MSB));
	//value = DM_CHANNEL_RX_SIZE;
	//writel(value, dma_base_vaddr + S2MM_LENGTH);
	mydebug("dma_base_vaddr + S2MM_DMACR:%x\n", readl(dma_base_vaddr + S2MM_DMACR));

	//{
	//	uint8_t *pcie_bar_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_PCIe_BAR0); 
	//	uint32_t *ptr = (uint32_t *)(pcie_bar_vaddr + DM_CHANNEL_TX_SIZE);
	//	int i;
	//	for(i = 0; i < 5; i++) {
	//		value = i;
	//		writel(value, ptr + i);
	//		mydebug("write %p:%x\n", ptr + i, readl(ptr + i));
	//	}
	//}

	return rtn;
}

static int start_dma_mm2s(void) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 

	uint32_t value;

	value = DM_CHANNEL_TX_SIZE;
	writel(value, dma_base_vaddr + MM2S_LENGTH);
	return 0;
}

static int start_dma_s2mm(void) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 

	uint32_t value;

	value = DM_CHANNEL_RX_SIZE;
	writel(value, dma_base_vaddr + S2MM_LENGTH);
	return 0;
}

static int init_dma_engine(void) {
	reset_dma();
	default_config_dma();

	return 0;
}

static int prepare_dm_addr_map(void) {
	uint64_t dm_addr = (uint64_t)kc705_pci_dev->dm_addr;

	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *sg_vaddr_map_ctl_reg = (uint32_t *)(base_vaddr + OFFSET_AXI_PCIe_CTL + AXIBAR2PCIEBAR_0U);

	int i;
	mydebug("base_vaddr:%p\n", base_vaddr);
	mydebug("(void *)dm_addr:%p\n", (void *)dm_addr);

	//bind bar0 
	mydebug("sg_vaddr_map_ctl_reg:%p\n", sg_vaddr_map_ctl_reg);
	writel((uint32_t)(dm_addr >> 32), sg_vaddr_map_ctl_reg);
	mydebug("(uint32_t)(dm_addr >> 32):%x\n", (uint32_t)(dm_addr >> 32));
	mydebug("readl(sg_vaddr_map_ctl_reg):%x\n", readl(sg_vaddr_map_ctl_reg));
	sg_vaddr_map_ctl_reg++;
	mydebug("sg_vaddr_map_ctl_reg:%p\n", sg_vaddr_map_ctl_reg);
	writel((uint32_t)dm_addr, sg_vaddr_map_ctl_reg);
	mydebug("(uint32_t)dm_addr:%x\n", (uint32_t)dm_addr);
	mydebug("readl(sg_vaddr_map_ctl_reg):%x\n", readl(sg_vaddr_map_ctl_reg));
	printk("\n\npcie_ctl:\n");
	for(i = 0; i < 0xfff; i += sizeof(uint32_t)) {
		if((i != 0) && (i % (8 * sizeof(uint32_t)) == 0)) {
			printk("\n");
		}
		printk("%08x(@0x%03x) ", readl(base_vaddr + OFFSET_AXI_PCIe_CTL + i), i);
	}
	printk("\n\ndma_lite:\n");
	for(i = 0; i < 0xfff; i += sizeof(uint32_t)) {
		if((i != 0) && (i % (8 * sizeof(uint32_t)) == 0)) {
			printk("\n");
		}
		printk("%08x(@0x%03x) ", readl(base_vaddr + OFFSET_AXI_DMA_LITE + i), i);
	}


	return 0;
}

static int test_dma(void) {
	start_dma_mm2s();
	start_dma_s2mm();
	{
		uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
		uint8_t *dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 
		int rtn;
		uint8_t *ptr = (uint8_t *)kc705_pci_dev->dm_memory;
		int i;

		mydebug("dma_base_vaddr + MM2S_DMASR:%x\n", readl(dma_base_vaddr + MM2S_DMASR));
		mydebug("dma_base_vaddr + S2MM_DMASR:%x\n", readl(dma_base_vaddr + S2MM_DMASR));

		rtn = wait_for_iostatus_timeout(XILINX_VDMA_LOOP_COUNT, dma_base_vaddr + MM2S_DMASR, BITMASK(1)/*Idle*/, BITMASK(1));
		if(rtn != 0) {
			mydebug("tx time out!\n");
		}

		rtn = wait_for_iostatus_timeout(XILINX_VDMA_LOOP_COUNT, dma_base_vaddr + S2MM_DMASR, BITMASK(1)/*Idle*/, BITMASK(1));
		if(rtn != 0) {
			mydebug("rx time out!\n");
		}

		mydebug("dma_base_vaddr + MM2S_DMASR:%x\n", readl(dma_base_vaddr + MM2S_DMASR));
		mydebug("dma_base_vaddr + S2MM_DMASR:%x\n", readl(dma_base_vaddr + S2MM_DMASR));
		mydebug("dma_base_vaddr + MM2S_DMASR:%x\n", readl(dma_base_vaddr + MM2S_DMACR));
		mydebug("dma_base_vaddr + S2MM_DMASR:%x\n", readl(dma_base_vaddr + S2MM_DMACR));

		for(i = 0; i < DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", ptr[i]);
		}
		printk("\n");

		for(i = 0; i < DM_CHANNEL_TX_SIZE; i++) {
			ptr[i] = 0;
		}

		for(i = 0; i < DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", ptr[i]);
		}
		printk("\n");
		{
			uint32_t value;
			uint64_t addr_rx = (uint64_t)BASE_AXI_PCIe_BAR0;
			uint64_t addr_tx = (uint64_t)BASE_AXI_PCIe_BAR0 + DM_CHANNEL_TX_SIZE;

			value = (uint32_t)(addr_tx);
			writel(value, dma_base_vaddr + MM2S_SA);
			value = (uint32_t)(addr_rx);
			writel(value, dma_base_vaddr + S2MM_DA);
		}
		start_dma_mm2s();
		start_dma_s2mm();
		rtn = wait_for_iostatus_timeout(XILINX_VDMA_LOOP_COUNT * 10, dma_base_vaddr + MM2S_DMASR, BITMASK(1)/*Idle*/, BITMASK(1));
		if(rtn != 0) {
			mydebug("tx time out!\n");
		}

		rtn = wait_for_iostatus_timeout(XILINX_VDMA_LOOP_COUNT * 10, dma_base_vaddr + S2MM_DMASR, BITMASK(1)/*Idle*/, BITMASK(1));
		if(rtn != 0) {
			mydebug("rx time out!\n");
		}

		for(i = 0; i < DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", ptr[i]);
		}
		printk("\n");
	}
	return 0;
}

#ifdef test_timer
static int start_timer(timer_data_t *pdata) {
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;
	timer_func_t func = pdata->func;

	init_timer(tl);
	tl->expires = jiffies + tmo;
	tl->data = (unsigned long)pdata;
	tl->function = func;
	add_timer(tl);

	return 0;
}

static timer_data_t *alloc_timer(struct timer_list *tl, unsigned long ms, timer_func_t func) {
	timer_data_t *pdata = (timer_data_t *)vzalloc(sizeof(timer_data_t));
	if(pdata == NULL) {
		return NULL;
	}

	pdata->tl = tl;
	pdata->ms = ms;
	pdata->func = func;

	start_timer(pdata);

	return pdata;
}

static void free_timer(void *pdata) {

	struct timer_list *tl = ((timer_data_t *)pdata)->tl;

	if(pdata == NULL) {
		return;
	}
	del_timer(tl);
	vfree(pdata);
}
#endif

static void read_pci_root_configuration(struct pci_dev * pdev) {
	int i;
	u8 valb;
	struct pci_bus * parent;
	struct pci_bus * me;

	/* Read PCI configuration space for all devices on this bus */
	parent = pdev->bus->parent;
	for(i=0; i<256; i++)
	{
		pci_bus_read_config_byte(parent, 8, i, &valb);
		printk("%02x ", valb);
		if(!((i+1) % 16)) printk("\n");
	}

	printk("Device %p details:\n", pdev);
	printk("Bus_list %p\n", &(pdev->bus_list));
	printk("Bus %p\n", pdev->bus);
	printk("Subordinate %p\n", pdev->subordinate);
	printk("Sysdata %p\n", pdev->sysdata);
	printk("Procent %p\n", pdev->procent);
	printk("Devfn %d\n", pdev->devfn);
	printk("Vendor %x\n", pdev->vendor);
	printk("Device %x\n", pdev->device);
	printk("Subsystem_vendor %x\n", pdev->subsystem_vendor);
	printk("Subsystem_device %x\n", pdev->subsystem_device);
	printk("Class %d\n", pdev->class);
	printk("Hdr_type %d\n", pdev->hdr_type);
	printk("Rom_base_reg %d\n", pdev->rom_base_reg);
	printk("Pin %d\n", pdev->pin);
	printk("Driver %p\n", pdev->driver);
	printk("Dma_mask %lx\n", (unsigned long)(pdev->dma_mask));
	printk("Vendor_compatible: ");
	//for(i=0; i<DEVICE_COUNT_COMPATIBLE; i++)
	//  printk("%x ", pdev->vendor_compatible[i]);
	//printk("\n");
	//printk("Device_compatible: ");
	//for(i=0; i<DEVICE_COUNT_COMPATIBLE; i++)
	//  printk("%x ", pdev->device_compatible[i]);
	//printk("\n");
	printk("Cfg_size %d\n", pdev->cfg_size);
	printk("Irq %d\n", pdev->irq);
	printk("Transparent %d\n", pdev->transparent);
	printk("Multifunction %d\n", pdev->multifunction);
	//printk("Is_enabled %d\n", pdev->is_enabled);
	printk("Is_busmaster %d\n", pdev->is_busmaster);
	printk("No_msi %d\n", pdev->no_msi);
	printk("No_dld2 %d\n", pdev->no_d1d2);
	printk("Block_ucfg_access %d\n", pdev->block_cfg_access);
	printk("Broken_parity_status %d\n", pdev->broken_parity_status);
	printk("Msi_enabled %d\n", pdev->msi_enabled);
	printk("Msix_enabled %d\n", pdev->msix_enabled);
	printk("Rom_attr_enabled %d\n", pdev->rom_attr_enabled);

	me = pdev->bus;
	printk("Bus details:\n");
	printk("Parent %p\n", me->parent);
	printk("Children %p\n", &(me->children));
	printk("Devices %p\n", &(me->devices));
	printk("Self %p\n", me->self);
	printk("Sysdata %p\n", me->sysdata);
	printk("Procdir %p\n", me->procdir);
	printk("Number %d\n", me->number);
	printk("Primary %d\n", me->primary);
//	printk("Secondary %d\n", me->secondary);
//	printk("Subordinate %d\n", me->subordinate);
	printk("Name %s\n", me->name);
	printk("Bridge_ctl %d\n", me->bridge_ctl);
	printk("Bridge %p\n", me->bridge);
}

static void read_pci_configuration(struct pci_dev * pdev) {
	int i;
	u8 valb;
	u16 valw;
	u32 valdw;
	unsigned long reg_base, reg_len;

	/* Read PCI configuration space */
	printk(KERN_INFO "PCI Configuration Space:\n");
	for(i=0; i<0x40; i++)
	{
		pci_read_config_byte(pdev, i, &valb);
		printk("0x%x ", valb);
		if((i % 0x10) == 0xf) printk("\n");
	}
	printk("\n");

	/* Now read each element - one at a time */

	/* Read Vendor ID */
	pci_read_config_word(pdev, PCI_VENDOR_ID, &valw);
	printk("Vendor ID: 0x%x, ", valw);

	/* Read Device ID */
	pci_read_config_word(pdev, PCI_DEVICE_ID, &valw);
	printk("Device ID: 0x%x, ", valw);

	/* Read Command Register */
	pci_read_config_word(pdev, PCI_COMMAND, &valw);
	printk("Cmd Reg: 0x%x, ", valw);

	/* Read Status Register */
	pci_read_config_word(pdev, PCI_STATUS, &valw);
	printk("Stat Reg: 0x%x, ", valw);

	/* Read Revision ID */
	pci_read_config_byte(pdev, PCI_REVISION_ID, &valb);
	printk("Revision ID: 0x%x, ", valb);

	/* Read Class Code */
	/*
	   pci_read_config_dword(pdev, PCI_CLASS_PROG, &valdw);
	   printk("Class Code: 0x%lx, ", valdw);
	   valdw &= 0x00ffffff;
	   printk("Class Code: 0x%lx, ", valdw);
	 */
	/* Read Reg-level Programming Interface */
	pci_read_config_byte(pdev, PCI_CLASS_PROG, &valb);
	printk("Class Prog: 0x%x, ", valb);

	/* Read Device Class */
	pci_read_config_word(pdev, PCI_CLASS_DEVICE, &valw);
	printk("Device Class: 0x%x, ", valw);

	/* Read Cache Line */
	pci_read_config_byte(pdev, PCI_CACHE_LINE_SIZE, &valb);
	printk("Cache Line Size: 0x%x, ", valb);

	/* Read Latency Timer */
	pci_read_config_byte(pdev, PCI_LATENCY_TIMER, &valb);
	printk("Latency Timer: 0x%x, ", valb);

	/* Read Header Type */
	pci_read_config_byte(pdev, PCI_HEADER_TYPE, &valb);
	printk("Header Type: 0x%x, ", valb);

	/* Read BIST */
	pci_read_config_byte(pdev, PCI_BIST, &valb);
	printk("BIST: 0x%x\n", valb);

	/* Read all 6 BAR registers */
	for(i=0; i<=5; i++)
	{
		/* Physical address & length */
		reg_base = pci_resource_start(pdev, i);
		reg_len = pci_resource_len(pdev, i);
		printk("BAR%d: Addr:0x%lx Len:0x%lx,  ", i, reg_base, reg_len);

		/* Flags */
		if((pci_resource_flags(pdev, i) & IORESOURCE_MEM)) {
			printk("Region is for memory\n");
		} else if((pci_resource_flags(pdev, i) & IORESOURCE_IO)) {
			printk("Region is for I/O\n");
		}
	}
	printk("\n");

	/* Read CIS Pointer */
	pci_read_config_dword(pdev, PCI_CARDBUS_CIS, &valdw);
	printk("CardBus CIS Pointer: 0x%x, ", valdw);

	/* Read Subsystem Vendor ID */
	pci_read_config_word(pdev, PCI_SUBSYSTEM_VENDOR_ID, &valw);
	printk("Subsystem Vendor ID: 0x%x, ", valw);

	/* Read Subsystem Device ID */
	pci_read_config_word(pdev, PCI_SUBSYSTEM_ID, &valw);
	printk("Subsystem Device ID: 0x%x\n", valw);

	/* Read Expansion ROM Base Address */
	pci_read_config_dword(pdev, PCI_ROM_ADDRESS, &valdw);
	printk("Expansion ROM Base Address: 0x%x\n", valdw);

	/* Read IRQ Line */
	pci_read_config_byte(pdev, PCI_INTERRUPT_LINE, &valb);
	printk("IRQ Line: 0x%x, ", valb);

	/* Read IRQ Pin */
	pci_read_config_byte(pdev, PCI_INTERRUPT_PIN, &valb);
	printk("IRQ Pin: 0x%x, ", valb);

	/* Read Min Gnt */
	pci_read_config_byte(pdev, PCI_MIN_GNT, &valb);
	printk("Min Gnt: 0x%x, ", valb);

	/* Read Max Lat */
	pci_read_config_byte(pdev, PCI_MAX_LAT, &valb);
	printk("Max Lat: 0x%x\n", valb);
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static irqreturn_t isr(int irq, void *dev_id, struct pt_regs *regs)
#else
static irqreturn_t isr(int irq, void *dev_id)
#endif
{
	irqreturn_t status = IRQ_NONE;

	struct pci_dev *pdev = dev_id;
	kc705_pci_dev_t * kc705_pci_dev = pci_get_drvdata(pdev);
	uint8_t *base_vaddr;
	uint8_t *dma_base_vaddr;
	uint32_t value;

	mydebug("\n");

	if(kc705_pci_dev == NULL) {
		mydebug("\n");
		return status;
	}

	base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	dma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_DMA_LITE); 

	value = readl(dma_base_vaddr + MM2S_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + MM2S_DMASR);
		status = IRQ_HANDLED;
	}

	value = readl(dma_base_vaddr + S2MM_DMASR);
	if((value & BITMASK(12)/*IOC_Irq*/) != 0) {
		mydebug("\n");
		value |= BITMASK(12);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	} else if((value & BITMASK(14)/*Err_Irq*/) != 0) {
		mydebug("\n");
		value |= BITMASK(14);
		writel(value, dma_base_vaddr + S2MM_DMASR);
		status = IRQ_HANDLED;
	}

	/* Handle DMA and any user interrupts */
	return status;
}

static int kc705_probe_pcie(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int rtn = 0;
	int i;

	//alloc memory for driver
	kc705_pci_dev = (kc705_pci_dev_t *)vzalloc(sizeof(kc705_pci_dev_t));
	if(kc705_pci_dev == NULL) {
		mydebug("alloc kc705_pci_dev failed.\n");
		rtn = -1;
		goto alloc_kc705_pci_dev_failed;
	}

	//alloc memory for dma
	mydebug("\n");
	kc705_pci_dev->dm_memory = dma_zalloc_coherent(&(pdev->dev), DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE, &(kc705_pci_dev->dm_addr), GFP_KERNEL);
	if(kc705_pci_dev->dm_memory == NULL) {
		rtn = -1;
		mydebug("dma_zalloc_coherent failed.\n");
		goto alloc_dm_memory_failed;
	}
	mydebug("\n");

	{
		uint8_t *ptr = (uint8_t *)kc705_pci_dev->dm_memory;
		for(i = 0; i < DM_CHANNEL_TX_SIZE; i++) {
			ptr[i] = i;
		}
		for(i = 0; i < DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE; i++) {
			if((i != 0) && (i % 16 == 0)) {
				printk("\n");
			}
			printk("%02x ", ptr[i]);
		}
		printk("\n");
	}

	rtn = pci_enable_device(pdev);
	if(rtn < 0)
	{
		mydebug("PCI device enable failed.\n");
		goto pci_enable_device_failed;
	}

	/* Display PCI configuration space of device. */
	read_pci_configuration(pdev);

	/* Display PCI information on parent. */
	read_pci_root_configuration(pdev);

	/*
	 * Enable bus-mastering on device. Calls pcibios_set_master() to do
	 * the needed architecture-specific settings.
	 */
	pci_set_master(pdev);

	/* Reserve PCI I/O and memory resources. Mark all PCI regions
	 * associated with PCI device as being reserved by owner. Do not
	 * access any address inside the PCI regions unless this call returns
	 * successfully.
	 */
	rtn = pci_request_regions(pdev, MODULE_NAME);
	if(rtn < 0) {
		mydebug("Could not request PCI regions.\n");
		goto pci_request_regions_failed;
	}

	for(i = 0; i < MAX_BARS; i++) {

		/* Atleast BAR0 must be there. */
		if((kc705_pci_dev->bar_info[i].base_len = pci_resource_len(pdev, i)) == 0) {
			if(i == 0) {
				mydebug("BAR 0 not valid, aborting.\n");
				goto pci_resource_len_failed;
			} else {
				continue;
			}
		} else {/* Set a bitmask for all the BARs that are present. */
			(kc705_pci_dev->bar_mask) |= ( 1 << i );
		}

		kc705_pci_dev->bar_info[i].base_phyaddr = pci_resource_start(pdev, i);

		/* Check all BARs for memory-mapped or I/O-mapped. The driver is
		 * intended to be memory-mapped.
		 */
		if(!(pci_resource_flags(pdev, i) & IORESOURCE_MEM)) {
			mydebug("BAR %d is of wrong type, aborting.\n", i);
			goto bar_resource_type_failed;
		}

		/* Map bus memory to CPU space. The ioremap may fail if size
		 * requested is too long for kernel to provide as a single chunk
		 * of memory, especially if users are sharing a BAR region. In
		 * such a case, call ioremap for more number of smaller chunks
		 * of memory. Or mapping should be done based on user request
		 * with user size. Neither is being done now - maybe later.
		 */
		if((kc705_pci_dev->bar_info[i].base_vaddr = ioremap((kc705_pci_dev->bar_info[i].base_phyaddr), kc705_pci_dev->bar_info[i].base_len)) == 0UL) {
			mydebug("Cannot map BAR %d space, invalidating.\n", i);
			(kc705_pci_dev->bar_mask) &= ~( 1 << i );
		} else {
			mydebug(
					"[BAR %d] Base PA %p Len %d VA %p\n",
					i,
					(void *)(kc705_pci_dev->bar_info[i].base_phyaddr),
					(u32)(kc705_pci_dev->bar_info[i].base_len),
					(void *)(kc705_pci_dev->bar_info[i].base_vaddr)
			       );
		}
	}

	kc705_pci_dev->pdev = pdev;

	/* Save private data pointer in device structure */
	pci_set_drvdata(pdev, kc705_pci_dev);

	/* Returns success if PCI is capable of 32-bit DMA */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	rtn = pci_set_dma_mask(pdev, DMA_32BIT_MASK);
#else
	rtn = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
#endif
	if(rtn < 0) {
		mydebug("pci_set_dma_mask failed\n");
		goto pci_set_dma_mask_failed;
	}

	/* Now enable interrupts using MSI mode */
	if(!pci_enable_msi(pdev)) {
		mydebug("MSI enabled\n");
		kc705_pci_dev->msi_enable = 1;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
	rtn = request_irq(pdev->irq, isr, SA_SHIRQ, MODULE_NAME, pdev);
#else
	rtn = request_irq(pdev->irq, isr, IRQF_SHARED, MODULE_NAME, pdev);
#endif
	if(rtn) {
		mydebug("Could not allocate interrupt %d\n", pdev->irq);
		mydebug("Unload driver and try running with polled mode instead\n");
		rtn = 0;
		goto request_irq_failed;
	}

	prepare_dm_addr_map();

	init_dma_engine();

	/* Display PCI configuration space of device. */
	read_pci_configuration(pdev);

	/* Display PCI information on parent. */
	read_pci_root_configuration(pdev);

	return 0;

pci_set_dma_mask_failed:
	pci_release_regions(pdev);
pci_request_regions_failed:
	pci_disable_device(pdev);
pci_enable_device_failed:
	dma_free_coherent(&(pdev->dev), DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE, kc705_pci_dev->dm_memory, kc705_pci_dev->dm_addr);
alloc_dm_memory_failed:
	vfree(kc705_pci_dev);
request_irq_failed:
bar_resource_type_failed:
pci_resource_len_failed:
alloc_kc705_pci_dev_failed:
	return rtn;
}

static int __devinit kc705_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int rtn = 0;

	rtn = kc705_probe_pcie(pdev, ent);
	test_dma();
	return rtn;
}

static void kc705_remove_pcie(struct pci_dev *pdev) {
	int i;

	free_irq(pdev->irq, pdev);

	if(kc705_pci_dev->msi_enable) {
		pci_disable_msi(pdev);
	}

	pci_set_drvdata(pdev, NULL);

	for(i = 0; i < MAX_BARS; i++) {
		if((kc705_pci_dev->bar_mask) & ( 1 << i )) {
			iounmap(kc705_pci_dev->bar_info[i].base_vaddr);
		}
	}

	pci_release_regions(pdev);

	pci_disable_device(pdev);

	dma_free_coherent(&(pdev->dev), DM_CHANNEL_TX_SIZE + DM_CHANNEL_RX_SIZE, kc705_pci_dev->dm_memory, kc705_pci_dev->dm_addr);

	vfree(kc705_pci_dev);
}

static void __devexit kc705_remove(struct pci_dev *pdev) {
	kc705_remove_pcie(pdev);
}

static struct pci_driver kc705_pcie_driver = {
	.name = MODULE_NAME,
	.id_table = ids,
	.probe = kc705_probe,
	.remove = __devexit_p(kc705_remove)
};


#ifdef test_timer
//timer test
struct timer_list my_timer;
timer_data_t *pdata = NULL;

static void my_timer_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	{
		int test_dma(void);
		test_dma();
	}
	myprintf("!\n");
	tl->expires = jiffies + tmo;
	add_timer(tl);
}
//timer test
#endif//#ifdef test_timer

static int __init kc705_init(void) {
	int rtn = 0;
	/* Just register the driver. No kernel boot options used. */
	rtn = pci_register_driver(&kc705_pcie_driver);
#ifdef test_timer
	pdata = alloc_timer(&my_timer, 1000, my_timer_func);
#endif//#ifdef test_timer

	mydebug("kc705 initilized!\n");
	return rtn;
}

static void __exit kc705_exit(void) {
#ifdef test_timer
	free_timer(pdata);
#endif//#ifdef test_timer
	/* Then, unregister driver with PCI in order to free up resources */
	pci_unregister_driver(&kc705_pcie_driver);

	mydebug("kc705 exit!\n");
}

module_init(kc705_init);
module_exit(kc705_exit);
