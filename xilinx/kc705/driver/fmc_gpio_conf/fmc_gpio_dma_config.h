#ifndef _FMC_GPIO_DMA_CONFIG_H
#define _FMC_GPIO_DMA_CONFIG_H
#include "fmc_gpio_dma_thread_config.h"
typedef struct dma_static_config {
	int dma_lite_offset;
	int pcie_bar_map_ctl_offset_0;
	int pcie_bar_map_ctl_offset_1;
	int pcie_map_bar_axi_addr_0;
	int pcie_map_bar_axi_addr_1;
	int dma_bar_map_num;

	dma_type_t dma_type;
	dma_thread_info_t *dma_thread;
	int dma_thread_count;
	bool is_auto_receive;

	int receive_bulk_size;
	int send_bulk_size;
	char *devname;
} dma_static_config_info_t;

static dma_static_config_info_t dma_info[] = {
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_1,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

		.dma_type = AXI_CDMA,
#if 1
		.dma_thread = dma_threads,
		.dma_thread_count = DMA_THREAD_COUNT, 
		.is_auto_receive = true,
#else
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif

		.receive_bulk_size = DMA_BLOCK_SIZE,
		.send_bulk_size = DMA_BLOCK_SIZE,
		.devname = "kc705_cdma",
	},
};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))

#endif//#ifndef _FMC_GPIO_DMA_CONFIG_H
