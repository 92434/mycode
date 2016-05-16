#ifndef _DVBS2_DMA_CONFIG_H
#define _DVBS2_DMA_CONFIG_H
#include "dvbs2_dma_thread_config.h"
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

#define STATE 1

static dma_static_config_info_t dma_info[] = {
#if STATE
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE,
		.pcie_bar_map_ctl_offset_0 = 0,
		.pcie_bar_map_ctl_offset_1 = 0,
		.pcie_map_bar_axi_addr_0 = 0,
		.pcie_map_bar_axi_addr_1 = 0,
		.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

		.dma_type = PSEUDO_DMA,
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,

		.receive_bulk_size = PCIe_MAP_BAR_SIZE,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "tsp_regs",
	},
#endif
#if STATE
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_0,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MAX_BAR_MAP_MEMORY,

		.dma_type = AXI_DMA,
#if 0
		.dma_thread = dma_threads,
		.dma_thread_count = DMA_THREAD_COUNT, 
		.is_auto_receive = true,
#else
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif

		.receive_bulk_size = 16 * 4,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "i2s_dma",
	},
#endif
#if STATE
	{
		.dma_lite_offset = OFFSET_AXI_DVBS2_0,
		.pcie_bar_map_ctl_offset_0 = 0,
		.pcie_bar_map_ctl_offset_1 = 0,
		.pcie_map_bar_axi_addr_0 = 0,
		.pcie_map_bar_axi_addr_1 = 0,
		.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

		.dma_type = PSEUDO_DMA,
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,

		.receive_bulk_size = PCIe_MAP_BAR_SIZE,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "dvbs2_regs",
	},
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_1,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MAX_BAR_MAP_MEMORY,

		.dma_type = AXI_DMA,
#if 1
		.dma_thread = dma_threads,
		.dma_thread_count = DMA_THREAD_COUNT, 
		.is_auto_receive = true,
#else
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif

		.receive_bulk_size = 16 * 4,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "asi_dma",//after asi
	},
#endif
#if STATE
	{
		.dma_lite_offset = OFFSET_AXI_LCM_LITE,
		.pcie_bar_map_ctl_offset_0 = AXIBAR2PCIEBAR_0U,
		.pcie_bar_map_ctl_offset_1 = AXIBAR2PCIEBAR_1U,
		.pcie_map_bar_axi_addr_0 = BASE_AXI_PCIe_BAR0,
		.pcie_map_bar_axi_addr_1 = BASE_AXI_PCIe_BAR1,
		.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

		.dma_type = PSEUDO_DMA,
#if 0
		.dma_thread = eg9013f_nz_fb_thread,
		.dma_thread_count = DMA_THREAD_EG9013F_NZ_COUNT,
#else
		.dma_thread = NULL,
		.dma_thread_count = 0,
#endif
		.is_auto_receive = false,

		.receive_bulk_size = PCIe_MAP_BAR_SIZE,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "lcm_regs",
	},
#endif
};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))

#endif//#ifndef _DVBS2_DMA_CONFIG_H
