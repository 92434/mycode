#include <linux/module.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/interrupt.h>


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
static int my_pci_device_id = 0;
module_param(my_pci_device_id, int, 0644);
MODULE_PARM_DESC(my_pci_device_id, "Xilinx's Device ID");

/** Driver information */
#define MODULE_NAME "kc705_pcie_xcdma"
#define DRIVER_DESCRIPTION "kc705 pcie xcdma driver"
#define DRIVER_VERSION "1.0"

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

//Address Map for the AXI to PCIe Address Translation Registers
#define AXIBAR2PCIEBAR_0U 0x208 //default be set to 0
#define AXIBAR2PCIEBAR_0L 0x20c //default be set to 0xa0000000
#define AXIBAR2PCIEBAR_1U 0x210 //default be set to 0
#define AXIBAR2PCIEBAR_1L 0x214 //default be set to 0xc0000000

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


#define MAX_BARS 6/**< Maximum number of BARs */

#define DM_CHANNEL_TX_SIZE 32
#define DM_CHANNEL_RX_SIZE 16

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


#ifdef test_timer
typedef void (*timer_func_t)(unsigned long);
typedef struct {
	struct timer_list *tl;
	unsigned long ms;
	timer_func_t func;
} timer_data_t;
#endif

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
	struct completion cmp;
	struct work_struct work;
} kc705_pci_dev_t;

static DEFINE_MUTEX(work_lock);
static kc705_pci_dev_t *kc705_pci_dev = NULL;
static LIST_HEAD(sg_list_info);

static uint32_t cdma_tail_des_axi_addr = BASE_AXI_PCIe_BAR0;
static int flush_sg_des_items(kc705_transfer_descriptor_t *axi_pcie_sg_port, struct list_head *sg_list) {
	kc705_transfer_descriptor_t *pdes = axi_pcie_sg_port; //buffer for sg, axi address 0x80800000
	kc705_sg_item_info_t *info;
	uint32_t bar_sg_next_addr_offset = BASE_AXI_PCIe_BAR0 + SG_DESCRIPTOR_SIZE;
	

	list_for_each_entry(info, sg_list, sg_item) {
		//kc705_sg_item_info_t *next_info = list_next_entry(info, sg_item);

		if(info->sg_item.next != sg_list) {
		//if(&(next_info->sg_item) != sg_list) {
			info->des.des.NXTDESC_PNTR = bar_sg_next_addr_offset;
			cdma_tail_des_axi_addr = bar_sg_next_addr_offset;
			bar_sg_next_addr_offset += SG_DESCRIPTOR_SIZE;
		} else {
			info->des.des.NXTDESC_PNTR = BASE_AXI_PCIe_BAR0;
		}


		pdes->des = info->des.des;

		pdes++;

	}

	return 0;
}

static int alloc_sg_des_item(uint32_t SA, uint32_t DA, uint32_t CONTROL, struct list_head *sg_list) {
	int rtn = 0;
	kc705_sg_item_info_t *sg_item = (kc705_sg_item_info_t *)vzalloc(sizeof(kc705_sg_item_info_t));

	if(sg_item == NULL) {
		rtn = -1;
		mydebug("alloc sg_item failed.\n");
		return rtn;
	}

	sg_item->des.des.SA = SA;
	sg_item->des.des.DA = DA;
	sg_item->des.des.CONTROL = CONTROL;
	list_add_tail(&sg_item->sg_item, sg_list);
	return 0;
}

static void free_sg_des_items(struct list_head *sg_list) {
	kc705_sg_item_info_t *sg_item, *sg_item_next;
	list_for_each_entry_safe(sg_item, sg_item_next, sg_list, sg_item) {
		list_del(&sg_item->sg_item);

		vfree(sg_item);
	}
}

static int wait_for_iostatus_timeout(unsigned long count, uint8_t *paddr, uint32_t mask, uint32_t expection) {
#define XILINX_VDMA_LOOP_COUNT 1000000
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

static int configure_cdma_engine(void) {
	int rtn;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *cdma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_CDMA_LITE); 

	//reset cdma
	uint32_t default_value = 0x00010002;//0000_0000_0000_0001(irq threshold)_0000_0000_0000_001(tail ptr enable)0
	uint32_t value;
	
	//reset
	value = default_value | BITMASK(2);//reset bit
	writel(value, cdma_base_vaddr + CDMA_CR);
	rtn = wait_for_iostatus_timeout(0, cdma_base_vaddr + CDMA_CR, BITMASK(2), 0);
	if(rtn != 0) {
		mydebug("time out!\n");
	}

	//enable sg mode
	value = readl(cdma_base_vaddr + CDMA_CR);
	writel(value, cdma_base_vaddr + CDMA_CR);
	value = readl(cdma_base_vaddr + CDMA_CR) | BITMASK(3);//sg mode
	writel(value, cdma_base_vaddr + CDMA_CR);

	//enable interrupt
	value = readl(cdma_base_vaddr + CDMA_CR) | BITMASK(12)/*ioc interrupt*/ | BITMASK(14)/*err interrupt*/;
	writel(value, cdma_base_vaddr + CDMA_CR);

	return rtn;
}

static int start_cdma(uint32_t tail_des_axi_addr) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint8_t *cdma_base_vaddr = (uint8_t *)(base_vaddr + OFFSET_AXI_CDMA_LITE); 

	//update cdma curdesc pointer
	writel(BASE_AXI_PCIe_BAR0, cdma_base_vaddr + CDMA_CURDESC_PNTR);

	writel(tail_des_axi_addr, cdma_base_vaddr + CDMA_TAILDESC_PNTR);
	return 0;
}

static int dma_trans_sync(void) {
	unsigned long tmo;
	int rtn = 0;

	tmo = msecs_to_jiffies(1000);
	tmo = wait_for_completion_timeout(&(kc705_pci_dev->cmp), tmo);
	if (0 == tmo) {
		printk(KERN_ERR "<%s> Error: rx transfer timed out\n", MODULE_NAME);
		rtn = -1;
	}

	return rtn;
}

static int prepare_bars_map(void) {
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;
	uint32_t *dm_vddr_info_base = (uint32_t *)base_vaddr;
	uint32_t *sg_vaddr_map_ctl_reg = (uint32_t *)(bram_vaddr + OFFSET_AXI_PCIe_CTL + AXIBAR2PCIEBAR_0U);

	uint64_t memory_tx = (uint64_t)kc705_pci_dev->bar_map_addr[1];
	uint64_t memory_rx = (uint64_t)kc705_pci_dev->bar_map_addr[2];
	uint64_t memory_sg = (uint64_t)kc705_pci_dev->bar_map_addr[0];


	//bind sg 
	writel((memory_sg >> 32) & 0xffffffff, sg_vaddr_map_ctl_reg);
	sg_vaddr_map_ctl_reg++;
	writel(memory_sg & 0xffffffff, sg_vaddr_map_ctl_reg);

	//prepare tx && rx buffer address for dm map ctl register at TR BRAM
	writel((memory_tx >> 32) & 0xffffffff, dm_vddr_info_base);
	dm_vddr_info_base++;
	writel(memory_tx & 0xffffffff, dm_vddr_info_base);

	dm_vddr_info_base++;
	writel((memory_rx >> 32) & 0xffffffff, dm_vddr_info_base);
	dm_vddr_info_base++;
	writel(memory_rx & 0xffffffff, dm_vddr_info_base);

	return 0;
}

static int prepare_sg_des_chain(uint32_t tx_axiaddr, uint32_t tx_size, uint32_t rx_axiaddr, uint32_t rx_size, struct list_head *sg_list) {
	int rtn = 0;
	int offset = 0;

	rtn = alloc_sg_des_item(
			BASE_Translation_BRAM + offset,
			BASE_AXI_PCIe_CTL + AXIBAR2PCIEBAR_1U,
			sizeof(uint64_t),
			sg_list
		);
	if(rtn != 0) {
		goto failed;
	}

	offset += sizeof(uint64_t);

	rtn = alloc_sg_des_item(BASE_AXI_PCIe_BAR1, tx_axiaddr, tx_size, sg_list);
	if(rtn != 0) {
		goto failed;
	}

	rtn = alloc_sg_des_item(
			BASE_Translation_BRAM + offset,
			BASE_AXI_PCIe_CTL + AXIBAR2PCIEBAR_1U,
			sizeof(uint64_t),
			sg_list
		);
	if(rtn != 0) {
		goto failed;
	}

	rtn = alloc_sg_des_item(rx_axiaddr, BASE_AXI_PCIe_BAR1, rx_size, sg_list);
	if(rtn != 0) {
		goto failed;
	}


	flush_sg_des_items((kc705_transfer_descriptor_t *)kc705_pci_dev->bar_map_memory[0], sg_list);

failed:
	free_sg_des_items(sg_list);

	return rtn;
}

static void dump_regs(void) {
	int i;
	uint8_t *base_vaddr = kc705_pci_dev->bar_info[0].base_vaddr;

	printk("\n\ntr_bram:\n");
	for(i = 0; i < 0xfff; i += sizeof(uint32_t)) {
		if((i != 0) && (i % (8 * sizeof(uint32_t)) == 0)) {
			printk("\n");
		}
		printk("%08x(@0x%03x) ", readl(base_vaddr + OFFSET_Translation_BRAM + i), i);
	}

	printk("\n\npcie_ctrl:\n");
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
		printk("%08x(@0x%03x) ", readl(base_vaddr + OFFSET_AXI_CDMA_LITE + i), i);
	}
	printk("\n");
}

static void prepare_test_data(void) {
	uint8_t *tx_addr = kc705_pci_dev->bar_map_memory[1];
	uint8_t *rx_addr = kc705_pci_dev->bar_map_memory[2];
	int i;

	for(i = 0; i < kc705_pci_dev->bar_map_memory_size[1]; i++) {
		tx_addr[i] = 8 + i;
	}

	for(i = 0; i < kc705_pci_dev->bar_map_memory_size[2]; i++) {
		rx_addr[i] = 0;
	}
}

static void test_result(void) {
	uint8_t *tx_addr = kc705_pci_dev->bar_map_memory[1];
	uint8_t *rx_addr = kc705_pci_dev->bar_map_memory[2];

	int i;

	mydebug("tx_addr:%p\n", (void *)tx_addr);
	for(i = 0; i < DM_CHANNEL_TX_SIZE; i++) {
		if((i != 0) && (i % 16 == 0)) {
			printk("\n");
		}
		printk("%02x ", tx_addr[i]);
	}

	printk("\n");

	mydebug("rx_addr:%p\n", (void *)rx_addr);
	for(i = 0; i < DM_CHANNEL_RX_SIZE; i++) {
		if((i != 0) && (i % 16 == 0)) {
			printk("\n");
		}
		printk("%02x ", rx_addr[i]);
	}
	printk("\n");
}

static int test_cdma(void) {
	prepare_bars_map();
	prepare_test_data();
	configure_cdma_engine();
	prepare_sg_des_chain(BASE_AXI_DDR_ADDR, DM_CHANNEL_TX_SIZE, BASE_AXI_DDR_ADDR + DM_CHANNEL_RX_SIZE, DM_CHANNEL_RX_SIZE, &sg_list_info);
	start_cdma(cdma_tail_des_axi_addr);
	dma_trans_sync();
	dump_regs();
	test_result();
	return 0;
}

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
	struct pci_dev *pdev = dev_id;
	kc705_pci_dev_t * kc705_pci_dev = pci_get_drvdata(pdev);

	kc705_pci_dev = kc705_pci_dev;
	mydebug("\n");
	/* Handle DMA and any user interrupts */
	return IRQ_NONE;
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

	kc705_pci_dev->bar_map_memory_size[0] = AXI_PCIe_BAR0_SIZE;
	kc705_pci_dev->bar_map_memory_size[1] = AXI_PCIe_BAR1_SIZE;
	kc705_pci_dev->bar_map_memory_size[2] = AXI_PCIe_BAR1_SIZE;
	//alloc memory for cdma
	for(i = 0; i < MAX_BARS; i++) {
		if(kc705_pci_dev->bar_map_memory_size[i] != 0) {
			kc705_pci_dev->bar_map_memory[i] = dma_zalloc_coherent(&(pdev->dev), kc705_pci_dev->bar_map_memory_size[i], &(kc705_pci_dev->bar_map_addr[i]), GFP_KERNEL);
			if(kc705_pci_dev->bar_map_memory[i] == NULL) {
				rtn = -1;
				mydebug("dma_zalloc_coherent failed.\n");
				goto pci_enable_device_failed;
			} else {
				mydebug("kc705_pci_dev->bar_map_memory[%d]:%p\n", i, kc705_pci_dev->bar_map_memory[i]);
				mydebug("kc705_pci_dev->bar_map_addr[%d]:%p\n", i, kc705_pci_dev->bar_map_addr[i]);
				mydebug("kc705_pci_dev->bar_map_memory_size[%d]:%x\n", i, kc705_pci_dev->bar_map_memory_size[i]);
			}
		}
	}

	rtn = pci_enable_device(pdev);
	if(rtn < 0)
	{
		mydebug("PCI device enable failed.\n");
		goto pci_enable_device_failed;
	}


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
	for(i = 0; i < MAX_BARS; i++) {
		if(kc705_pci_dev->bar_map_memory[i] != 0) {
			dma_free_coherent(&(pdev->dev), kc705_pci_dev->bar_map_memory_size[i], kc705_pci_dev->bar_map_memory[i], kc705_pci_dev->bar_map_addr[i]);
		}
	}
	vfree(kc705_pci_dev);
request_irq_failed:
bar_resource_type_failed:
pci_resource_len_failed:
alloc_kc705_pci_dev_failed:
	return rtn;
}

static void work_func(struct work_struct *work) {
	kc705_pci_dev_t *kc705_pci_dev = container_of(work, kc705_pci_dev_t, work);

	mutex_lock(&work_lock);
	kc705_pci_dev = kc705_pci_dev;
	//dump_regs();
	//test_result();
	mutex_unlock(&work_lock);
}

static int __devinit kc705_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int rtn = 0;

	rtn = kc705_probe_pcie(pdev, ent);
	INIT_WORK(&(kc705_pci_dev->work), work_func);
	test_dma();
	return rtn;
}

static void kc705_remove_pcie(struct pci_dev *pdev) {
	int i;

	mutex_lock(&work_lock);
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

	for(i = 0; i < MAX_BARS; i++) {
		if(kc705_pci_dev->bar_map_memory[i] != 0) {
			dma_free_coherent(&(pdev->dev), kc705_pci_dev->bar_map_memory_size[i], kc705_pci_dev->bar_map_memory[i], kc705_pci_dev->bar_map_addr[i]);
		}
	}

	vfree(kc705_pci_dev);
	mutex_unlock(&work_lock);
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
timer_data_t *pdata = NULL;

static void default_timer_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	//myprintf("!\n");
	schedule_work(&(kc705_pci_dev->work));
	tl->expires = jiffies + tmo;
	add_timer(tl);
}

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

static timer_data_t *alloc_timer(unsigned long ms, timer_func_t func) {
	timer_data_t *pdata = NULL;
	struct timer_list *tl = NULL;

	if(func == NULL) {
		func = default_timer_func;
	}

	pdata = (timer_data_t *)vzalloc(sizeof(timer_data_t));
	if(pdata == NULL) {
		mydebug("no memory for timer_data!\n");
		goto alloc_timer_data_failed;
	}

	tl = (struct timer_list *)vzalloc(sizeof(struct timer_list));
	if(tl == NULL) {
		mydebug("no memory for timer_list!\n");
		goto alloc_timer_list_failed;
	}

	pdata->tl = tl;
	pdata->ms = ms;
	pdata->func = func;

	start_timer(pdata);

	return pdata;

	vfree(tl);
alloc_timer_list_failed:
	vfree(pdata);
alloc_timer_data_failed:
	return NULL;
}

static void free_timer(void *pdata) {

	struct timer_list *tl;

	if(pdata == NULL) {
		mydebug("no timer_data!\n");
		return;
	}

	tl = ((timer_data_t *)pdata)->tl;
	if(tl == NULL) {
		mydebug("no timer_list!\n");
		goto free_timer_data;
	}

	del_timer(tl);
	vfree(tl);

free_timer_data:
	vfree(pdata);
}

#endif

static int __init kc705_init(void) {
	int rtn = 0;

	if(my_pci_device_id != 0) {
		mydebug("my_pci_device_id:0x%x!\n", my_pci_device_id);
		ids[0].device = my_pci_device_id;
	}

	/* Just register the driver. No kernel boot options used. */
	rtn = pci_register_driver(&kc705_pcie_driver);
#ifdef test_timer
	pdata = alloc_timer(1000, NULL);
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
