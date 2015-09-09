
#include "pcie.h"
#include "utils/xiaofei_debug.h"

int put_pcie_tr(pcie_dma_t *dma,
		uint64_t tx_dest_axi_addr,
		uint64_t rx_src_axi_addr,
		int tx_size,
		int rx_size);
void test_tsp(pcie_dma_t *dma) {
	put_pcie_tr(dma, BASE_AXI_TSP_LITE, BASE_AXI_TSP_LITE, 7, 7);
}
