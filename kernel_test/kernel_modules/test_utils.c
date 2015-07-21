#include <linux/module.h>
#include <linux/version.h>

#include "utils/xiaofei_debug.h"

#define MODULE_NAME "test_module"
#define DRIVER_DESCRIPTION "test_module"
#define DRIVER_VERSION "1.0"

/** Driver Module information */
MODULE_AUTHOR("xiaofei");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
MODULE_ALIAS(MODULE_NAME);

#ifdef test_timer
#include "utils/xiaofei_timer.h"
timer_data_t *pdata = NULL;
#endif


static int __init test_init(void) {
#ifdef test_timer
	pdata = alloc_timer(1000, NULL);
#endif
	return 0;
}

static void __exit test_exit(void) {
#ifdef test_timer
	free_timer(pdata);
#endif
}

module_init(test_init);
module_exit(test_exit);
