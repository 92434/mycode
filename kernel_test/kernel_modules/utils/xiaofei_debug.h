#ifndef _XIAOFEI_DEBUG_H
#define _XIAOFEI_DEBUG_H
#include <linux/version.h>

#define mydebug(format, ...) printk(KERN_DEBUG "[%s:%s:%d]:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)
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

#endif//#ifndef _XIAOFEI_DEBUG_H
