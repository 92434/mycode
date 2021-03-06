#ifndef _DMA_CONF_H
#define _DMA_CONF_H

#if defined(DVBS2)
#include "dvbs2_conf/dvbs2_dma_config.h"
#elif defined(KC705_CSA)
#include "csa_conf/csa_dma_config.h"
#elif defined(KC705_CSA_DEBUG)
#include "csa_debug_conf/csa_debug_dma_config.h"
#elif defined(KC705_FMC_GPIO)
#include "fmc_gpio_conf/fmc_gpio_dma_config.h"
#elif defined(KC705_I2S_RECEIVER)
#include "i2s_receiver_conf/i2s_receiver_dma_config.h"
#elif defined(KC705_IIC_SLAVE)
#include "iic_slave_conf/iic_slave_dma_config.h"
#elif defined(KC705_DVBS2)
#include "kc705_dvbs2_conf/kc705_dvbs2_dma_config.h"
#elif defined(KC705_TSP)
#include "kc705_tsp_conf/kc705_tsp_dma_config.h"
#endif

#endif //#define _DMA_CONF_H
