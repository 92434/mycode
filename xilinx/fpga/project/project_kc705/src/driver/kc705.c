#include <linux/module.h>
#include <linux/version.h>
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

static int __init kc705_init(void) {
	int rtn = 0;

	/* Just register the driver. No kernel boot options used. */
	rtn = pci_register_driver(&kc705_pcie_driver);

	mydebug("kc705 initilized!\n");

	return rtn;
}

static void __exit kc705_exit(void) {
	/* Then, unregister driver with PCI in order to free up resources */
	pci_unregister_driver(&kc705_pcie_driver);

	mydebug("kc705 exit!\n");
}

module_init(kc705_init);
module_exit(kc705_exit);
