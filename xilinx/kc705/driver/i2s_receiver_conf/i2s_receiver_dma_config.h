#ifndef _I2S_RECEIVER_DMA_CONFIG_H
#define _I2S_RECEIVER_DMA_CONFIG_H
#include "i2s_receiver_dma_thread_config.h"
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
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_0,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MAX_BAR_MAP_MEMORY,

		.dma_type = AXI_DMA,
#if 1
		.dma_thread = i2s_receiver_dma_threads,
		.dma_thread_count = I2S_RECEIVER_DMA_THREAD, 
		.is_auto_receive = true,
#else//#if 1
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif//#if 1

		.receive_bulk_size = 87 * 4,
		.send_bulk_size = 87 * 4,
		.devname = "i2s_receiver_dma",
	},
};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))

#endif//#ifndef _I2S_RECEIVER_DMA_CONFIG_H
