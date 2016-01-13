#include "pcie.h"
#include "kc705.h"

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

#include "pcie_gpio.h"
#include "pcie_device.h"
#include "pcie_dma.h"
static void add_local_device(kc705_pci_dev_t *kc705_pci_dev) {
	add_kc705_gpio(kc705_pci_dev);

	init_kc705_dev_class(kc705_pci_dev);

	setup_kc705_dev(kc705_pci_dev);

	add_dma_device(kc705_pci_dev);
}

static void remove_local_device(kc705_pci_dev_t *kc705_pci_dev) {

	remove_dma_device(kc705_pci_dev);

	uninstall_kc705_dev(kc705_pci_dev);

	uninit_kc705_dev_class(kc705_pci_dev);

	remove_kc705_gpio(kc705_pci_dev);
}

#include "pcie_performance.h"
#include "pcie_tr_thread.h"
#include "pcie_dma_thread.h"
void start_dma(kc705_pci_dev_t *kc705_pci_dev) {
	start_statistic_timer(kc705_pci_dev);

	alloc_tr_thread(kc705_pci_dev);

	start_dma_thread(kc705_pci_dev);
}

void stop_dma(kc705_pci_dev_t *kc705_pci_dev) {
	stop_dma_thread(kc705_pci_dev);

	free_tr_thread(kc705_pci_dev);

	stop_statistic_timer(kc705_pci_dev);
}

static int ioremap_pcie_bars(kc705_pci_dev_t *kc705_pci_dev) {
	int ret = 0;
	struct pci_dev *pdev = kc705_pci_dev->pdev;

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

	if(kc705_pci_dev == NULL) {
		mydebug("\n");
		return status;
	}

	status = dma_process_isr(kc705_pci_dev);

	/* Handle DMA and any user interrupts */
	return status;
}

#include "pcie_utils.h"
static int kc705_probe_pcie(struct pci_dev *pdev, const struct pci_device_id *ent) {
	int ret = 0;
	kc705_pci_dev_t *kc705_pci_dev = NULL;

	mydebug("pdev->dev.of_node:%p\n", pdev->dev.of_node);

	//alloc memory for driver
	kc705_pci_dev = (kc705_pci_dev_t *)vzalloc(sizeof(kc705_pci_dev_t));
	if(kc705_pci_dev == NULL) {
		mydebug("alloc kc705_pci_dev failed.\n");
		ret = -1;
		goto alloc_kc705_pci_dev_failed;
	}

	kc705_pci_dev->pdev = pdev;
	/* Save private data pointer in device structure */
	pci_set_drvdata(kc705_pci_dev->pdev, kc705_pci_dev);

	ret = alloc_kc705_dma(kc705_pci_dev);
	if(ret != 0) {
		ret = -1;
		goto alloc_kc705_pci_dma_failed;
	}

	ret = alloc_dma_thread(kc705_pci_dev);
	if(ret != 0) {
		ret = -1;
		goto alloc_kc705_pci_dma_thread_failed;
	}

	ret = alloc_kc705_gpio(kc705_pci_dev);
	if(ret != 0) {
		ret = -1;
		goto alloc_kc705_pci_gpiochip_failed;
	}

	ret = alloc_pcie_tr(kc705_pci_dev);
	if(ret != 0) {
		goto init_pcie_tr_failed;
	}

	ret = pci_enable_device(kc705_pci_dev->pdev);
	if(ret < 0) {
		mydebug("PCI device enable failed.\n");
		goto free_dma_resource;
	}

	/*
	 * Enable bus-mastering on device. Calls pcibios_set_master() to do
	 * the needed architecture-specific settings.
	 */
	pci_set_master(kc705_pci_dev->pdev);

	/* Reserve PCI I/O and memory resources. Mark all PCI regions
	 * associated with PCI device as being reserved by owner. Do not
	 * access any address inside the PCI regions unless this call returns
	 * successfully.
	 */
	ret = pci_request_regions(kc705_pci_dev->pdev, MODULE_NAME);
	if(ret < 0) {
		mydebug("Could not request PCI regions.\n");
		goto pci_request_regions_failed;
	}

	ret = ioremap_pcie_bars(kc705_pci_dev);
	if(ret != 0) {
		goto remap_pcie_bars_failed;
	}
	
	/* Returns success if PCI is capable of 32-bit DMA */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = pci_set_dma_mask(kc705_pci_dev->pdev, DMA_32BIT_MASK);
#else
	ret = pci_set_dma_mask(kc705_pci_dev->pdev, DMA_BIT_MASK(32));
#endif
	if(ret < 0) {
		mydebug("pci_set_dma_mask failed\n");
		goto pci_set_dma_mask_failed;
	}

	/* Now enable interrupts using MSI mode */
	if(!pci_enable_msi(kc705_pci_dev->pdev)) {
		mydebug("MSI enabled\n");
		kc705_pci_dev->msi_enable = 1;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
	ret = request_irq(kc705_pci_dev->pdev->irq, isr, SA_SHIRQ, MODULE_NAME, kc705_pci_dev->pdev);
#else
	ret = request_irq(kc705_pci_dev->pdev->irq, isr, IRQF_SHARED, MODULE_NAME, kc705_pci_dev->pdev);
#endif
	if(ret != 0) {
		mydebug("Could not allocate interrupt %d\n", kc705_pci_dev->pdev->irq);
		mydebug("Unload driver and try running with polled mode instead\n");
		ret = 0;
		goto request_irq_failed;
	}

	/* Display PCI configuration space of device. */
	read_pci_configuration(kc705_pci_dev->pdev);

	/* Display PCI information on parent. */
	read_pci_root_configuration(kc705_pci_dev->pdev);

	kc705_pci_dev->devname = "kc705_pcie";
	add_local_device(kc705_pci_dev);
	start_dma(kc705_pci_dev);

	return 0;

request_irq_failed:
pci_set_dma_mask_failed:
	iounmap_pcie_bars(kc705_pci_dev);
remap_pcie_bars_failed:
	pci_release_regions(kc705_pci_dev->pdev);
pci_request_regions_failed:
	pci_disable_device(kc705_pci_dev->pdev);
free_dma_resource:
	free_pcie_tr(kc705_pci_dev);
init_pcie_tr_failed:
	free_kc705_gpio(kc705_pci_dev);
alloc_kc705_pci_gpiochip_failed:
	free_dma_thread(kc705_pci_dev);
alloc_kc705_pci_dma_thread_failed:
	free_kc705_dma(kc705_pci_dev);
alloc_kc705_pci_dma_failed:
	pci_set_drvdata(kc705_pci_dev->pdev, NULL);
	vfree(kc705_pci_dev);
alloc_kc705_pci_dev_failed:
	return ret;
}

static void kc705_remove_pcie(struct pci_dev *pdev) {
	kc705_pci_dev_t *kc705_pci_dev = pci_get_drvdata(pdev);

	if(kc705_pci_dev == NULL) {
		return;
	}

	stop_dma(kc705_pci_dev);

	remove_local_device(kc705_pci_dev);

	if(pdev->irq != 0) {
		free_irq(pdev->irq, pdev);
	}

	if(kc705_pci_dev->msi_enable) {
		pci_disable_msi(pdev);
	}

	iounmap_pcie_bars(kc705_pci_dev);

	pci_release_regions(pdev);
	
	pci_clear_master(pdev);

	pci_disable_device(pdev);

	free_pcie_tr(kc705_pci_dev);

	free_kc705_gpio(kc705_pci_dev);

	free_dma_thread(kc705_pci_dev);

	free_kc705_dma(kc705_pci_dev);

	pci_set_drvdata(pdev, NULL);

	vfree(kc705_pci_dev);
}

struct pci_driver kc705_pcie_driver = {
	.name = MODULE_NAME,
	.id_table = ids,
	.probe = kc705_probe_pcie,
	.remove = __devexit_p(kc705_remove_pcie)
};
