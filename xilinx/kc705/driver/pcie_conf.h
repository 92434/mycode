#ifndef _PCIE_CONF_H
#define _PCIE_CONF_H

#if defined(DVBS2)
#include "dvbs2_conf/dvbs2_pcie_config.h"
#elif defined(KC705_CSA)
#include "csa_conf/csa_pcie_config.h"
#elif defined(KC705_FMC_GPIO)
#include "fmc_gpio_conf/fmc_gpio_pcie_config.h"
#elif defined(KC705_I2S_RECEIVER_TEST)
#include "i2s_receiver_conf/i2s_receiver_pcie_config.h"
#elif defined(KC705_IIC_SLAVE)
#include "iic_slave_conf/iic_slave_pcie_config.h"
#elif defined(KC705_DVBS2)
#include "kc705_dvbs2_conf/kc705_dvbs2_pcie_config.h"
#endif

#endif //#define _PCIE_CONF_H