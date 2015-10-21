#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/version.h>

#include "kc705.h"
#include "pcie.h"
#include "dma_common.h"

int setup_pcie_dma_dev(pcie_dma_t *dma, char *nameformat, ...);
void uninstall_pcie_dma_dev(pcie_dma_t *dma);
int setup_kc705_dev(kc705_pci_dev_t *kc705_pci_dev, char *namefmt, ...);
void uninstall_kc705_dev(kc705_pci_dev_t *kc705_pci_dev);
void *kc705_add_gpio_chip(uint8_t *base_addr, int ngpios, int *pngpio, char *namefmt, ...);
void kc705_remove_gpio_chip(void *ppara);

/**
 * Macro to export pci_device_id to user space to allow hot plug and
 * module loading system to know what module works with which hardware device
 */
MODULE_DEVICE_TABLE(pci, ids);

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

static kc705_pci_dev_t *kc705_pci_dev = NULL;
static DEFINE_MUTEX(work_lock);

extern dma_op_t axi_dma_op;
extern dma_op_t axi_cdma_op;
extern dma_op_t pseudo_dma_op;

typedef enum {
	AXI_DMA = 0,
	AXI_CDMA,
	PSEUDO_DMA,
} dma_type_t;

typedef struct dma_static_config {
	int dma_lite_offset;
	int pcie_bar_map_ctl_offset;
	int pcie_map_bar_axi_addr_0;
	int pcie_map_bar_axi_addr_1;
	int dma_bar_map_num;
	dma_type_t dma_type;
} dma_static_config_info_t;

static dma_static_config_info_t dma_info[] = {
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_0,
		.pcie_bar_map_ctl_offset = AXIBAR2PCIEBAR_0U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MAX_BAR_MAP_MEMORY,
		.dma_type = AXI_DMA,
	},
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_1,
		.pcie_bar_map_ctl_offset = AXIBAR2PCIEBAR_2U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR2,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR3,
		.dma_bar_map_num = 3,
		.dma_type = AXI_CDMA,
	},
	{
		.dma_lite_offset = 0,
		.pcie_bar_map_ctl_offset = 0,
		.pcie_map_bar_axi_addr_0 = 0,
		.pcie_map_bar_axi_addr_1 = 0,
		.dma_bar_map_num = 3,
		.dma_type = PSEUDO_DMA,
	},
};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))
#define MAX_TEST_THREAD 10

typedef struct {
	int chip_addr_offset;
	int ngpios;
	int ngpio[2];
} gpio_chip_info_t;

static gpio_chip_info_t gpio_chip_info[] = {
	//{
	//	.chip_addr_offset = OFFSET_AXI_GPIO_LITE_0,
	//	.ngpios = 2,
	//	.ngpio = {32, 32},

	//},
	//{
	//	.chip_addr_offset = OFFSET_AXI_GPIO_LITE_1,
	//	.ngpios = 2,
	//	.ngpio = {32, 27},

	//},
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_2,
		.ngpios = 2,
		.ngpio = {32, 32},

	},
	{
		.chip_addr_offset = OFFSET_AXI_GPIO_LITE_3,
		.ngpios = 1,
		.ngpio = {9, 0},

	},
};

#define GPIOCHIP_MAX (sizeof(gpio_chip_info) / sizeof(gpio_chip_info_t))

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
	printk("Bridge %p\n", (void *)me->bridge);
}

void dump_regs(uint8_t *reg_addr, int size) {
	int i;
	uint8_t *base_vaddr = reg_addr;

	mydebug("\n");
	for(i = 0; i < size; i += sizeof(uint32_t)) {
		if((i != 0) && (i % (8 * sizeof(uint32_t)) == 0)) {
			printk("\n");
		}
		printk("%08x(@0x%03x) ", readl(base_vaddr + i), i);
	}

	printk("\n");
}

void write_regs(uint8_t *reg_addr, int size) {
	int i;
	uint8_t *base_vaddr = reg_addr;

	mydebug("\n");
	//for(i = 0; i < size; i += sizeof(uint32_t)) {
	for(i = 0; i < size; i++) {
		writeb(i, base_vaddr + i);
	}
}

void dump_memory(void *addr, int size) {
	uint8_t *memory = (uint8_t *)addr;
	int i;

	printk("addr:0x%p\n", addr);
	for(i = 0; i < size; i++) {
		if((i != 0) && (i % (8 * sizeof(uint32_t)) == 0)) {
			printk("\n");
		}
		printk("%02x ", memory[i]);
	}

	printk("\n");
}

#define SPEED_MB (1024 * 1024)

//static struct timeval start_time = {0};
long unsigned int get_op_tx_count(pcie_dma_t *dma);
long unsigned int get_op_rx_count(pcie_dma_t *dma);
static void test_performance(kc705_pci_dev_t *kc705_pci_dev) {
	int i;
	long unsigned int total_tx_speed = 0;
	long unsigned int total_rx_speed = 0;
	//struct timeval stop_time;

	//do_gettimeofday(&stop_time);

	//mydebug("stop_time.tv_sec:%lu\n", stop_time.tv_sec);
	//mydebug("start_time.tv_sec:%lu\n", start_time.tv_sec);
	//mydebug("stop_time.tv_usec:%lu\n", stop_time.tv_usec);
	//mydebug("start_time.tv_usec:%lu\n", start_time.tv_usec);

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		long unsigned int tx_speed = get_op_tx_count(dma);
		long unsigned int rx_speed = get_op_rx_count(dma);

		printk("DMA[%02d](MB/s)|U:%4lu.%06lu|D:%4lu.%06lu|A:%4lu.%06lu\n",
			i,
			tx_speed / SPEED_MB, (1000 * 1000) * (tx_speed % SPEED_MB) / SPEED_MB,
			rx_speed / SPEED_MB, (1000 * 1000) * (rx_speed % SPEED_MB) / SPEED_MB,
			(rx_speed + tx_speed) / SPEED_MB, (1000 * 1000) * ((rx_speed + tx_speed) % SPEED_MB) / SPEED_MB);

		total_tx_speed += tx_speed;
		total_rx_speed += rx_speed;

	}

	printk("all    (MB/s)|U:%4lu.%06lu|D:%4lu.%06lu|A:%4lu.%06lu\n",
		total_tx_speed / SPEED_MB, (1000 * 1000) * (total_tx_speed % SPEED_MB) / SPEED_MB,
		total_rx_speed / SPEED_MB, (1000 * 1000) * (total_rx_speed % SPEED_MB) / SPEED_MB,
		(total_tx_speed + total_rx_speed) / SPEED_MB, (1000 * 1000) * ((total_tx_speed + total_rx_speed) % SPEED_MB) / SPEED_MB);
	printk("\n");

	//do_gettimeofday(&start_time);
}

static void work_func(struct work_struct *work) {
	kc705_pci_dev_t *kc705_pci_dev = container_of(work, kc705_pci_dev_t, work);

	mutex_lock(&work_lock);
	test_performance(kc705_pci_dev);
	mutex_unlock(&work_lock);
}

static void timer_func(unsigned long __opaque) {
	timer_data_t *ptimer_data = (timer_data_t *)__opaque;
	unsigned long tmo = msecs_to_jiffies(ptimer_data->ms);
	struct timer_list *tl = ptimer_data->tl;

	schedule_work(&(kc705_pci_dev->work));
	tl->expires = jiffies + tmo;
	add_timer(tl);
}

static int init_pcie_tr(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	pcie_tr_t *tr;

	kc705_pci_dev->pcie_tr_list = init_list_buffer();
	if(kc705_pci_dev->pcie_tr_list == NULL) {
		ret = -1;
		return ret;
	}

	tr = (pcie_tr_t *)vzalloc(sizeof(pcie_tr_t) * 1024);
	if(tr == NULL) {
		mydebug("alloc tr failed.\n");
		ret = -1;
		return ret;
	}
	add_list_buffer_item((char *)tr, (void *)NULL, sizeof(pcie_tr_t) * 1024, kc705_pci_dev->pcie_tr_list);

	return ret;
}

static void uninit_pcie_tr(kc705_pci_dev_t *kc705_pci_dev) {
	buffer_node_t *node;

	node = list_entry(kc705_pci_dev->pcie_tr_list->first, buffer_node_t, list);
	vfree(node->buffer);
	
	uninit_list_buffer(kc705_pci_dev->pcie_tr_list);
}

int tr_wait(pcie_dma_t *dma, struct completion *tr_cmp);
int put_pcie_tr(pcie_dma_t *dma,
		uint64_t tx_dest_axi_addr,
		uint64_t rx_src_axi_addr,
		int tx_size,
		int rx_size,
		uint8_t *tx_data,
		uint8_t *rx_data,
		bool wait) {
	int ret = 0;

	pcie_tr_t tr;
	tr.dma = dma;
	tr.tx_dest_axi_addr = tx_dest_axi_addr;
	tr.rx_src_axi_addr = rx_src_axi_addr;
	tr.tx_size = tx_size;
	tr.rx_size = rx_size;
	tr.tx_data = tx_data;
	tr.rx_data = rx_data;

	if(wait) {
		struct completion *tr_cmp = (struct completion *)vzalloc(sizeof(struct completion));
		if(tr_cmp == NULL) {
			mydebug("alloc tr_cmp failed.\n");
			ret = -1;
			goto exit;
		} else {
			tr.tr_cmp = tr_cmp;
			init_completion(tr.tr_cmp);
		}
	} else {
		tr.tr_cmp = NULL;
	}

	spin_lock(&kc705_pci_dev->alloc_lock);
	ret = write_buffer((char *)&tr, sizeof(pcie_tr_t), kc705_pci_dev->pcie_tr_list);
	spin_unlock(&kc705_pci_dev->alloc_lock);

	//mydebug("tr.tr_cmp:%p\n", tr.tr_cmp);
	if(tr.tr_cmp != NULL) {
		tr_wait(tr.dma, tr.tr_cmp);
		vfree(tr.tr_cmp);
	}

exit:
	return ret;
}

static void reset_pcie_tr(void) {
	spin_lock(&kc705_pci_dev->alloc_lock);
	buffer_reset(kc705_pci_dev->pcie_tr_list);
	spin_unlock(&kc705_pci_dev->alloc_lock);
}

int read_buffer(char *buffer, int size, list_buffer_t *list);
static int get_pcie_tr(kc705_pci_dev_t *kc705_pci_dev, pcie_tr_t *tr) {
	int ret;

	ret = read_buffer((char *)tr, sizeof(pcie_tr_t), kc705_pci_dev->pcie_tr_list);

	return ret;
}

static int pcie_tr_thread(void *ppara) {
	int ret = 0;
	int i;

	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)ppara;

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		dma->dma_op.init_dma(dma);
	}

	while(!kthread_should_stop()) {
		int size;
		pcie_tr_t tr;

		set_current_state(TASK_UNINTERRUPTIBLE);  
		//schedule_timeout(1*HZ); 
		
		size = get_pcie_tr(kc705_pci_dev, &tr);
		if(size == 0) {
			pcie_dma_t *dma = kc705_pci_dev->dma;

			tr.dma = dma;
			tr.tx_dest_axi_addr = 0;
			tr.rx_src_axi_addr = 0;
			tr.tx_size = DMA_BLOCK_SIZE;
			tr.rx_size = DMA_BLOCK_SIZE;
			tr.tx_data = NULL;
			tr.rx_data = NULL;
			tr.tr_cmp = NULL;
		}

		tr.dma->dma_op.dma_tr(&tr);
	 }

	return ret;
}

static int test_thread(void *ppara) {
	int ret = 0;
	kc705_pci_dev_t *kc705_pci_dev = (kc705_pci_dev_t *)ppara;
	uint8_t *tx_data = NULL;
	uint8_t *rx_data = NULL;

	while(!kthread_should_stop()) {
		//pcie_tr_t tr;

		pcie_dma_t *dma = kc705_pci_dev->dma + 1;

		set_current_state(TASK_UNINTERRUPTIBLE);  
		schedule_timeout(msecs_to_jiffies(1000)); 


		put_pcie_tr(dma, BASE_AXI_DDR_ADDR + 0, BASE_AXI_DDR_ADDR + 0, DMA_BLOCK_SIZE, DMA_BLOCK_SIZE, NULL, NULL, false);

		dma++;

		//mydebug("%p:tx_data:%p\n", current, tx_data);
		//mydebug("%p:rx_data:%p\n", current, rx_data);
		if(tx_data == NULL) {
			tx_data = (uint8_t *)vzalloc(DMA_BLOCK_SIZE);
			if(tx_data == NULL) {
				mydebug("alloc tx_data failed.\n");
			}
		}

		if(rx_data == NULL) {
			rx_data = (uint8_t *)vzalloc(DMA_BLOCK_SIZE);
			if(rx_data == NULL) {
				mydebug("alloc rx_data failed.\n");
			}
		}

		//mydebug("%p:tx_data:%p\n", current, tx_data);
		//mydebug("%p:rx_data:%p\n", current, rx_data);

		//put_pcie_tr(dma, OFFSET_AXI_TSP_LITE + 0, OFFSET_AXI_TSP_LITE + 0, 189, 0, tx_data, NULL, true);
		//put_pcie_tr(dma, OFFSET_AXI_TSP_LITE + 0, OFFSET_AXI_TSP_LITE + 0, 0, 189, NULL, rx_data, true);
		put_pcie_tr(dma, OFFSET_AXI_TSP_LITE + 0, OFFSET_AXI_TSP_LITE + 0, 189, 189, tx_data, rx_data, true);
		//put_pcie_tr(dma, OFFSET_AXI_TSP_LITE + 0, OFFSET_AXI_TSP_LITE + 0, 189, 189, NULL, NULL, true);
		//tr.dma = dma;
		//tr.tx_dest_axi_addr = 0;
		//tr.rx_src_axi_addr = 0;
		//tr.tx_size = 189;
		//tr.rx_size = 189;
		//tr.tx_data = tx_data;
		//tr.rx_data = rx_data;
		//tr.tr_cmp = NULL;
		//tr.dma->dma_op.dma_tr(&tr);

	}

	if(tx_data != NULL) {
		vfree(tx_data);
	}
	if(rx_data != NULL) {
		vfree(rx_data);
	}
	mydebug("\n");

	return ret;
}

int create_gpio_proc_dir(void);
static void add_local_device(void) {
	int i;

	for(i = 0; i < GPIOCHIP_MAX; i++) {
		uint8_t *gpio_chip_base_addr = kc705_pci_dev->bar_info[0].base_vaddr + gpio_chip_info[i].chip_addr_offset;
		int ngpios = gpio_chip_info[i].ngpios;
		int *pngpio = gpio_chip_info[i].ngpio;
		kc705_pci_dev->gpiochip[i] = kc705_add_gpio_chip(gpio_chip_base_addr, ngpios, pngpio, "gpiochip%d", i);
	}

	create_gpio_proc_dir();

	setup_kc705_dev(kc705_pci_dev, "%s", "kc705_pcie");

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		setup_pcie_dma_dev(dma, "%s_%d", "pciedma", i);
	}
}

void remove_gpio_proc_dir(void);
static void remove_local_device(void) {
	int i;

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		uninstall_pcie_dma_dev(dma);
	}

	uninstall_kc705_dev(kc705_pci_dev);

	remove_gpio_proc_dir();

	for(i = 0; i < GPIOCHIP_MAX; i++) {
		if(kc705_pci_dev->gpiochip[i] != NULL) {
			kc705_remove_gpio_chip(kc705_pci_dev->gpiochip[i]);
			kc705_pci_dev->gpiochip[i] = NULL;
		}
	}
}

void start_dma(void) {
	int i;

	if(kc705_pci_dev->ptimer_data == NULL) {
		INIT_WORK(&(kc705_pci_dev->work), work_func);
		kc705_pci_dev->ptimer_data = alloc_timer(1000, timer_func);
	}

	if(kc705_pci_dev->pcie_tr_thread == NULL) {
		kc705_pci_dev->pcie_tr_thread = alloc_work_thread(pcie_tr_thread, kc705_pci_dev, "%s", "pcie_tr");
	}

	for(i = 0; i < MAX_TEST_THREAD; i++) {
		if(kc705_pci_dev->test_thread[i] == NULL) {
			kc705_pci_dev->test_thread[i] = alloc_work_thread(test_thread, kc705_pci_dev, "%s_%d", "test", i);
		}
	}
}

void stop_dma(void) {
	int i;

	for(i = 0; i < MAX_TEST_THREAD; i++) {
		if(kc705_pci_dev->test_thread[i] != NULL) {
			free_work_thread(kc705_pci_dev->test_thread[i]);
			kc705_pci_dev->test_thread[i] = NULL;
		}
	}

	if(kc705_pci_dev->pcie_tr_thread != NULL) {
		free_work_thread(kc705_pci_dev->pcie_tr_thread);
		kc705_pci_dev->pcie_tr_thread = NULL;
	}

	if(kc705_pci_dev->ptimer_data != NULL) {
		free_timer(kc705_pci_dev->ptimer_data);
		kc705_pci_dev->ptimer_data = NULL;
	}

	reset_pcie_tr();

}

#define DMA_BAR_MEM_START_INDEX 1
static int prepare_dma_memory(kc705_pci_dev_t *kc705_pci_dev, struct pci_dev *pdev) {
	int ret = 0;
	int i, j;

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		dma->kc705_pci_dev = kc705_pci_dev;

		dma->list = init_list_buffer();
		if(dma->list == NULL) {
			ret = -1;
			return ret;
		}

		dma->dma_lite_offset = dma_info[i].dma_lite_offset;
		dma->pcie_bar_map_ctl_offset = dma_info[i].pcie_bar_map_ctl_offset;
		dma->pcie_map_bar_axi_addr_0 = dma_info[i].pcie_map_bar_axi_addr_0;
		dma->pcie_map_bar_axi_addr_1 = dma_info[i].pcie_map_bar_axi_addr_1;
		dma->dma_bar_map_num = dma_info[i].dma_bar_map_num;
		dma->dma_op = (dma_info[i].dma_type == AXI_DMA) ? axi_dma_op :
					(dma_info[i].dma_type == AXI_CDMA) ? axi_cdma_op :
					(dma_info[i].dma_type == PSEUDO_DMA) ? pseudo_dma_op :
					pseudo_dma_op;

		for(j = 0; j < dma->dma_bar_map_num; j++) {
			dma->bar_map_memory_size[j] = PCIe_MAP_BAR_SIZE;
		}

		//alloc memory cdma
		for(j = 0; j < MAX_BAR_MAP_MEMORY; j++) {
			if(dma->bar_map_memory_size[j] != 0) {
				dma->bar_map_memory[j] = dma_zalloc_coherent(&(pdev->dev), dma->bar_map_memory_size[j], &(dma->bar_map_addr[j]), GFP_KERNEL);
				if(dma->bar_map_memory[j] == NULL) {
					ret = -1;
					mydebug("dma_zalloc_coherent failed.\n");
					return ret;
				} else {
					//mydebug("dma->bar_map_memory[%d]:%p\n", j, dma->bar_map_memory[j]);
					//mydebug("dma->bar_map_addr[%d]:%p\n", j, (void *)dma->bar_map_addr[j]);
					//mydebug("dma->bar_map_memory_size[%d]:%x\n", j, dma->bar_map_memory_size[j]);
					if(j > DMA_BAR_MEM_START_INDEX) {//
						add_list_buffer_item((char *)dma->bar_map_memory[j], (void *)dma->bar_map_addr[j], dma->bar_map_memory_size[j], dma->list);
					}
				}
			}
		}
	}

	return ret;
}

static void uninit_dma_memory(kc705_pci_dev_t *kc705_pci_dev, struct pci_dev *pdev) {
	int i, j;

	if(kc705_pci_dev->dma == NULL) {
		return;
	}

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;

		for(j = 0; j < MAX_BAR_MAP_MEMORY; j++) {
			if(dma->bar_map_memory[j] != 0) {
				dma_free_coherent(&(pdev->dev), dma->bar_map_memory_size[j], dma->bar_map_memory[j], dma->bar_map_addr[j]);
			}
		}
		uninit_list_buffer(dma->list);
	}

}

static int ioremap_pcie_bars(kc705_pci_dev_t *kc705_pci_dev, struct pci_dev *pdev) {
	int ret = 0;
	int i;

	for(i = 0; i < MAX_BARS; i++) {

		/* Atleast BAR0 must be there. */
		if((kc705_pci_dev->bar_info[i].base_len = pci_resource_len(pdev, i)) == 0) {
			if(i == 0) {
				mydebug("BAR 0 not valid, aborting.\n");
				ret = -1;
				return ret;
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
			ret = -1;
			return ret;
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
			ret = -1;
			return ret;
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

	return ret;
}

static void iounmap_pcie_bars(kc705_pci_dev_t *kc705_pci_dev) {
	int i;

	for(i = 0; i < MAX_BARS; i++) {
		if((kc705_pci_dev->bar_mask) & ( 1 << i )) {
			iounmap(kc705_pci_dev->bar_info[i].base_vaddr);
		}
	}
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
	int i;

	if(kc705_pci_dev == NULL) {
		mydebug("\n");
		return status;
	}

	for(i = 0; i < DMA_MAX; i++) {
		pcie_dma_t *dma = kc705_pci_dev->dma + i;
		if(dma->dma_op.process_isr(dma) == IRQ_HANDLED) {
			status = IRQ_HANDLED;
		}
	}

	/* Handle DMA and any user interrupts */
	return status;
}

static int kc705_probe_pcie(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int ret = 0;

	mydebug("pdev->dev.of_node:%p\n", pdev->dev.of_node);

	//alloc memory for driver
	kc705_pci_dev = (kc705_pci_dev_t *)vzalloc(sizeof(kc705_pci_dev_t));
	if(kc705_pci_dev == NULL) {
		mydebug("alloc kc705_pci_dev failed.\n");
		ret = -1;
		goto alloc_kc705_pci_dev_failed;
	}

	kc705_pci_dev->dma = (pcie_dma_t *)vzalloc(sizeof(pcie_dma_t) * DMA_MAX);
	if(kc705_pci_dev->dma == NULL) {
		mydebug("alloc kc705_pci_dev->dma failed.\n");
		ret = -1;
		goto alloc_kc705_pci_dma_failed;
	}

	kc705_pci_dev->gpiochip = (void **)vzalloc(sizeof(void *) * GPIOCHIP_MAX);
	if(kc705_pci_dev->gpiochip == NULL) {
		mydebug("alloc kc705_pci_dev->dma failed.\n");
		ret = -1;
		goto alloc_kc705_pci_gpiochip_failed;
	}

	ret = prepare_dma_memory(kc705_pci_dev, pdev);
	if(ret != 0) {
		goto free_pcie_resource;
	}

	ret = init_pcie_tr(kc705_pci_dev);
	if(ret != 0) {
		goto init_pcie_tr_failed;
	}


	ret = pci_enable_device(pdev);
	if(ret < 0) {
		mydebug("PCI device enable failed.\n");
		goto free_dma_resource;
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
	ret = pci_request_regions(pdev, MODULE_NAME);
	if(ret < 0) {
		mydebug("Could not request PCI regions.\n");
		goto pci_request_regions_failed;
	}

	ret = ioremap_pcie_bars(kc705_pci_dev, pdev);
	if(ret != 0) {
		goto remap_pcie_bars_failed;
	}
	
	mydebug("txp addr:%p\n", kc705_pci_dev->bar_info[0].base_vaddr + OFFSET_AXI_TSP_LITE);
	//write_regs(kc705_pci_dev->bar_info[0].base_vaddr + OFFSET_AXI_TSP_LITE, 47);
	//dump_regs(kc705_pci_dev->bar_info[0].base_vaddr + OFFSET_AXI_TSP_LITE, 47);

	kc705_pci_dev->pdev = pdev;

	/* Save private data pointer in device structure */
	pci_set_drvdata(pdev, kc705_pci_dev);

	/* Returns success if PCI is capable of 32-bit DMA */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = pci_set_dma_mask(pdev, DMA_32BIT_MASK);
#else
	ret = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
#endif
	if(ret < 0) {
		mydebug("pci_set_dma_mask failed\n");
		goto pci_set_dma_mask_failed;
	}

	/* Now enable interrupts using MSI mode */
	if(!pci_enable_msi(pdev)) {
		mydebug("MSI enabled\n");
		kc705_pci_dev->msi_enable = 1;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
	ret = request_irq(pdev->irq, isr, SA_SHIRQ, MODULE_NAME, pdev);
#else
	ret = request_irq(pdev->irq, isr, IRQF_SHARED, MODULE_NAME, pdev);
#endif
	if(ret != 0) {
		mydebug("Could not allocate interrupt %d\n", pdev->irq);
		mydebug("Unload driver and try running with polled mode instead\n");
		ret = 0;
		goto request_irq_failed;
	}

	/* Display PCI configuration space of device. */
	read_pci_configuration(pdev);

	/* Display PCI information on parent. */
	read_pci_root_configuration(pdev);


	add_local_device();
	//start_dma();

	return 0;

request_irq_failed:
pci_set_dma_mask_failed:
	iounmap_pcie_bars(kc705_pci_dev);
remap_pcie_bars_failed:
	pci_release_regions(pdev);
pci_request_regions_failed:
	pci_disable_device(pdev);
free_dma_resource:
	uninit_pcie_tr(kc705_pci_dev);
init_pcie_tr_failed:
	uninit_dma_memory(kc705_pci_dev, pdev);
free_pcie_resource:
	vfree(kc705_pci_dev->gpiochip);
alloc_kc705_pci_gpiochip_failed:
	vfree(kc705_pci_dev->dma);
alloc_kc705_pci_dma_failed:
	vfree(kc705_pci_dev);
alloc_kc705_pci_dev_failed:
	kc705_pci_dev = NULL;
	return ret;
}

static void kc705_remove_pcie(struct pci_dev *pdev) {
	mutex_lock(&work_lock);
	if(kc705_pci_dev != NULL) {

		remove_local_device();

		stop_dma();


		if(pdev->irq != 0) {
			free_irq(pdev->irq, pdev);
		}

		if(kc705_pci_dev->msi_enable) {
			pci_disable_msi(pdev);
		}

		pci_set_drvdata(pdev, NULL);

		iounmap_pcie_bars(kc705_pci_dev);

		pci_release_regions(pdev);

		pci_disable_device(pdev);

		uninit_pcie_tr(kc705_pci_dev);

		uninit_dma_memory(kc705_pci_dev, pdev);

		vfree(kc705_pci_dev->gpiochip);

		vfree(kc705_pci_dev->dma);

		vfree(kc705_pci_dev);
	}
	mutex_unlock(&work_lock);
}

struct pci_driver kc705_pcie_driver = {
	.name = MODULE_NAME,
	.id_table = ids,
	.probe = kc705_probe_pcie,
	.remove = __devexit_p(kc705_remove_pcie)
};
