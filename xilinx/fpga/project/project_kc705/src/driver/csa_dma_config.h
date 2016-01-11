#ifndef _CSA_DMA_CONFIG_H
#define _CSA_DMA_CONFIG_H
#include "csa_dma_thread_config.h"

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
	int receive_bulk_size;
	int send_bulk_size;
} dma_static_config_info_t;

static dma_static_config_info_t dma_info[] = {
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_0,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_0U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR0,
		.dma_bar_map_num = MAX_BAR_MAP_MEMORY,
		.dma_type = AXI_DMA,
		.dma_thread = csa_dma_threads;
		.dma_thread_count = CSA_DMA_THREAD, 
		.receive_bulk_size = (4 * 3) * 4,
		.send_bulk_size = 5 * 4,
	},
	//{
	//	.dma_lite_offset = OFFSET_AXI_DMA_LITE_1,
	//	.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_1U,
	//	.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
	//	.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR1,
	//	.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
	//	.dma_bar_map_num = MAX_BAR_MAP_MEMORY,
	//	.dma_type = AXI_DMA,
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_DMA_LITE_2,
	//	.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_2U,
	//	.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_2U,
	//	.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR2,
	//	.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR2,
	//	.dma_bar_map_num = MAX_BAR_MAP_MEMORY,
	//	.dma_type = AXI_DMA,
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_DMA_LITE_3,
	//	.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_3U,
	//	.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_4U,
	//	.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR3,
	//	.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR4,
	//	.dma_bar_map_num = 3,
	//	.dma_type = AXI_CDMA,
	//},
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE,
		.pcie_bar_map_ctl_offset_0 = 0,
		.pcie_bar_map_ctl_offset_1 = 0,
		.pcie_map_bar_axi_addr_0 = 0,
		.pcie_map_bar_axi_addr_1 = 0,
		.dma_bar_map_num = 3,
		.dma_type = PSEUDO_DMA,
		.dma_thread = NULL;
		.dma_thread_count = 0, 
		.receive_bulk_size = PCIe_MAP_BAR_SIZE,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
	},
};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))

#endif//#ifndef _CSA_DMA_CONFIG_H
