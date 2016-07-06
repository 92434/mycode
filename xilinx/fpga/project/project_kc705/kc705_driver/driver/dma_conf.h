#ifndef _DMA_CONF_H
#define _DMA_CONF_H

#if defined(KC705_DVBS2)
#include "dvbs2_conf/dvbs2_dma_config.h"
#elif defined(KC705_CSA)
#include "csa_conf/csa_dma_config.h"
#elif defined(KC705_FMC_GPIO)
#include "fmc_gpio_conf/fmc_gpio_dma_config.h"
#elif defined(KC705_I2S_RECEIVER_TEST)
#include "i2s_receiver_conf/i2s_receiver_dma_config.h"
#endif

#endif //#define _DMA_CONF_H
