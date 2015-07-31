#ifndef _KC705_H
#define _KC705_H

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

/** Driver information */
#define MODULE_NAME "kc705_pcie_xcdma"
#define DRIVER_DESCRIPTION "kc705 pcie xcdma driver"
#define DRIVER_VERSION "1.0"

#endif //#define _KC705_H
