#include <linux/module.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/interrupt.h>


#define mydebug(format, ...) printk("[%s:%s:%d]<%s>:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, MODULE_NAME, ## __VA_ARGS__)
#define myprintf(format, ...) printk(format, ## __VA_ARGS__)

//#define __devinit
//#define __devexit
//#define __devexit_p(f) f

/** @name Macros for PCI probing
 * @{
 */
#define PCI_VENDOR_ID_DMA   0x10EE      /**< Vendor ID - Xilinx */

#define PCI_DEVICE_ID_DMA   0x7042      /**< Xilinx's Device ID */

/** Driver information */
#define MODULE_NAME "kc705_pcie_xcdma"
#define DRIVER_DESCRIPTION "kc705 pcie xcdma driver"
#define DRIVER_VERSION "1.0"


#define MAX_BARS 6/**< Maximum number of BARs */

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
	u32 barMask;                    /**< Bitmask for BAR information */
	struct {
		unsigned long basePAddr; /**< Base address of device memory */
		unsigned long baseLen; /**< Length of device memory */
		void __iomem * baseVAddr; /**< VA - mapped address */
	} barInfo[MAX_BARS];
	int MSIEnabled;
} kc705_pci_dev_t;

static kc705_pci_dev_t *kc705_pci_dev = NULL;


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
	printk("Secondary %d\n", me->secondary);
	printk("Subordinate %d\n", me->subordinate);
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
	pdev = pdev;

	/* Handle DMA and any user interrupts */
	return IRQ_NONE;
}

static int __devinit kc705_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int rtn = 0;
	int i;

	kc705_pci_dev = (kc705_pci_dev_t *)vzalloc(sizeof(kc705_pci_dev_t));
	if(kc705_pci_dev == NULL) {
		mydebug(KERN_ERR "alloc kc705_pci_dev failed.\n");
		rtn = -1;
		goto alloc_kc705_pci_dev_failed;
	}

	rtn = pci_enable_device(pdev);
	if(rtn < 0)
	{
		mydebug(KERN_ERR "PCI device enable failed.\n");
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
		mydebug(KERN_ERR "Could not request PCI regions.\n");
		goto pci_request_regions_failed;
	}

	/* Returns success if PCI is capable of 32-bit DMA */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	rtn = pci_set_dma_mask(pdev, DMA_32BIT_MASK);
#else
	rtn = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
#endif
	if(rtn < 0) {
		mydebug(KERN_ERR "pci_set_dma_mask failed\n");
		goto pci_set_dma_mask_failed;
	}

	for(i = 0; i < MAX_BARS; i++) {
		u32 size;

		/* Atleast BAR0 must be there. */
		if((size = pci_resource_len(pdev, i)) == 0) {
			if(i == 0) {
				mydebug(KERN_ERR "BAR 0 not valid, aborting.\n");
				goto pci_resource_len_failed;
			} else {
				continue;
			}
		} else {/* Set a bitmask for all the BARs that are present. */
			(kc705_pci_dev->barMask) |= ( 1 << i );
		}

		/* Check all BARs for memory-mapped or I/O-mapped. The driver is
		 * intended to be memory-mapped.
		 */
		if(!(pci_resource_flags(pdev, i) & IORESOURCE_MEM)) {
			mydebug(KERN_ERR "BAR %d is of wrong type, aborting.\n", i);
			goto bar_resource_type_failed;
		}

		/* Map bus memory to CPU space. The ioremap may fail if size
		 * requested is too long for kernel to provide as a single chunk
		 * of memory, especially if users are sharing a BAR region. In
		 * such a case, call ioremap for more number of smaller chunks
		 * of memory. Or mapping should be done based on user request
		 * with user size. Neither is being done now - maybe later.
		 */
		if((kc705_pci_dev->barInfo[i].baseVAddr = ioremap((kc705_pci_dev->barInfo[i].basePAddr), size)) == 0UL) {
			mydebug(KERN_ERR "Cannot map BAR %d space, invalidating.\n", i);
			(kc705_pci_dev->barMask) &= ~( 1 << i );
		} else {
			mydebug(
					KERN_INFO "[BAR %d] Base PA %x Len %d VA %x\n",
					i,
					(u32)(kc705_pci_dev->barInfo[i].basePAddr),
					(u32)(kc705_pci_dev->barInfo[i].baseLen),
					(u32)(kc705_pci_dev->barInfo[i].baseVAddr)
			       );
		}
	}

	kc705_pci_dev->pdev=pdev;

	/* Save private data pointer in device structure */
	pci_set_drvdata(pdev, kc705_pci_dev);

	/* Now enable interrupts using MSI mode */
	if(!pci_enable_msi(pdev)) {
		mydebug(KERN_INFO "MSI enabled\n");
		kc705_pci_dev->MSIEnabled = 1;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
	rtn = request_irq(pdev->irq, isr, SA_SHIRQ, MODULE_NAME, pdev);
#else
	rtn = request_irq(pdev->irq, isr, IRQF_SHARED, MODULE_NAME, pdev);
#endif
	if(rtn) {
		mydebug(KERN_ERR "Could not allocate interrupt %d\n", pdev->irq);
		mydebug(KERN_ERR "Unload driver and try running with polled mode instead\n");
		rtn = 0;
		goto request_irq_failed;
	}

	return 0;

pci_set_dma_mask_failed:
	pci_release_regions(pdev);
pci_request_regions_failed:
	pci_disable_device(pdev);
pci_enable_device_failed:
	vfree(kc705_pci_dev);
request_irq_failed:
bar_resource_type_failed:
pci_resource_len_failed:
alloc_kc705_pci_dev_failed:
	return rtn;
}

static void __devexit kc705_remove(struct pci_dev *pdev) {
	int i;

	free_irq(pdev->irq, pdev);

	if(kc705_pci_dev->MSIEnabled) {
		pci_disable_msi(pdev);
	}
	pci_set_drvdata(pdev, NULL);

	for(i = 0; i < MAX_BARS; i++) {
		if((kc705_pci_dev->barMask) & ( 1 << i )) {
			iounmap(kc705_pci_dev->barInfo[i].baseVAddr);
		}
	}

	pci_release_regions(pdev);

	pci_disable_device(pdev);

	vfree(kc705_pci_dev);
}

static struct pci_driver kc705_pcie_driver = {
	.name = MODULE_NAME,
	.id_table = ids,
	.probe = kc705_probe,
	.remove = __devexit_p(kc705_remove)
};


#define test_timer 1
#ifdef test_timer
//timer test
struct timer_list my_timer;
timer_data_t *pdata = NULL;

static void my_timer_func(unsigned long __opaque) {
	timer_data_t *pdata = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(pdata->ms);
	struct timer_list *tl = pdata->tl;

	mydebug(KERN_ERR "\n");
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

	mydebug(KERN_ERR "kc705 initilized!(%s)\n", "xiaofei");
	return rtn;
}

static void __exit kc705_exit(void) {
#ifdef test_timer
	free_timer(pdata);
#endif//#ifdef test_timer
	/* Then, unregister driver with PCI in order to free up resources */
	pci_unregister_driver(&kc705_pcie_driver);

	mydebug(KERN_ERR "kc705 exit!\n");
}

module_init(kc705_init);
module_exit(kc705_exit);

