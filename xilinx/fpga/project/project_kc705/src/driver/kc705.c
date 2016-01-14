#include <linux/module.h>
#include <linux/pci.h>
#include "kc705.h"
#include "pcie.h"

#include "utils/xiaofei_debug.h"

/** Driver Module information */
MODULE_AUTHOR("xiaofei");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
MODULE_ALIAS(MODULE_NAME);

static int my_pci_device_id = 0;
module_param(my_pci_device_id, int, 0644);
MODULE_PARM_DESC(my_pci_device_id, "Xilinx's Device ID");

static int __init kc705_init(void) {
	int ret = 0;

	if(my_pci_device_id != 0) {
		mydebug("my_pci_device_id:0x%x!\n", my_pci_device_id);
		((struct pci_device_id *)kc705_pcie_driver.id_table)[0].device = my_pci_device_id;
	}

	/* Just register the driver. No kernel boot options used. */
	ret = pci_register_driver(&kc705_pcie_driver);

	mydebug("kc705 initilized!\n");

	return ret;
}

static void __exit kc705_exit(void) {
	/* Then, unregister driver with PCI in order to free up resources */
	pci_unregister_driver(&kc705_pcie_driver);

	mydebug("kc705 exit!\n");
}

module_init(kc705_init);
module_exit(kc705_exit);
