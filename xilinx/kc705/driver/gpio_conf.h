#ifndef _GPIO_CONF_H
#define _GPIO_CONF_H

#if defined(DVBS2)
#include "dvbs2_conf/dvbs2_gpio_config.h"
#elif defined(KC705_CSA)
#include "csa_conf/csa_gpio_config.h"
#elif defined(KC705_FMC_GPIO)
#include "fmc_gpio_conf/fmc_gpio_gpio_config.h"
#elif defined(KC705_I2S_RECEIVER_TEST)
#include "i2s_receiver_conf/i2s_receiver_gpio_config.h"
#elif defined(KC705_IIC_SLAVE)
#include "iic_slave_conf/iic_slave_gpio_config.h"
#elif defined(KC705_DVBS2)
#include "kc705_dvbs2_conf/kc705_dvbs2_gpio_config.h"
#elif defined(KC705_TSP)
#include "kc705_tsp_conf/kc705_tsp_gpio_config.h"
#endif

#endif //#define _GPIO_CONF_H
