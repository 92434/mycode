#ifndef __LOCAL_COMMON_H__
#define __LOCAL_COMMON_H__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33)
#include <generated/autoconf.h>
#else
#include <linux/autoconf.h>
#endif

#include <linux/types.h>
#include "kal.h"
#include "basetype.h"

#endif