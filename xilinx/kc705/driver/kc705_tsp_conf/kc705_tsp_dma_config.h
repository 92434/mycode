#ifndef _KC705_TSP_DMA_CONFIG_H
#define _KC705_TSP_DMA_CONFIG_H
#include "kc705_tsp_dma_thread_config.h"
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

#define DEFINED_TSP 2
#define ENABLE_DMA 1

static dma_static_config_info_t dma_info[] = {
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE_0,
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
		.devname = "tsp_regs_0",
	},
#if (DEFINED_TSP == 2)
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE_1,
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
		.devname = "tsp_regs_1",
	},
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE_2,
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
		.devname = "tsp_regs_2",
	},
	{
		.dma_lite_offset = OFFSET_AXI_TSP_LITE_3,
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
		.devname = "tsp_regs_3",
	},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_4,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_4",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_5,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_5",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_6,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_6",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_7,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_7",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_8,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_8",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_9,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_9",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_10,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_10",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_11,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_11",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_12,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_12",
	//},
	//{
	//	.dma_lite_offset = OFFSET_AXI_TSP_LITE_13,
	//	.pcie_bar_map_ctl_offset_0 = 0,
	//	.pcie_bar_map_ctl_offset_1 = 0,
	//	.pcie_map_bar_axi_addr_0 = 0,
	//	.pcie_map_bar_axi_addr_1 = 0,
	//	.dma_bar_map_num = MIN_BAR_MAP_MEMORY,

	//	.dma_type = PSEUDO_DMA,
	//	.dma_thread = NULL,
	//	.dma_thread_count = 0, 
	//	.is_auto_receive = false,

	//	.receive_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.send_bulk_size = PCIe_MAP_BAR_SIZE,
	//	.devname = "tsp_regs_13",
	//},
#endif//if (DEFINED_TSP == 0)

#if (DEFINED_TSP == 0)
#if (ENABLE_DMA == 1)
	{
		.dma_lite_offset = OFFSET_AXI_DMA_LITE_0,
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
#else//#if 1
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif//#if 1

		.receive_bulk_size = 128 * 4,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "tsp_dma",
	},
#endif//#if (ENABLE_DMA == 1)

#if ENABLE_DMA
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
#else//#if 1
		.dma_thread = NULL,
		.dma_thread_count = 0, 
		.is_auto_receive = false,
#endif//#if 1

		.receive_bulk_size = 128 * 4,
		.send_bulk_size = PCIe_MAP_BAR_SIZE,
		.devname = "asi_dma",
	},
#endif//#if ENABLE_DMA
#endif//#if (DEFINED_TSP == 0)

};

#define DMA_MAX (sizeof(dma_info) / sizeof(dma_static_config_info_t))

#endif//#ifndef _KC705_TSP_DMA_CONFIG_H
