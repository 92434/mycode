`timescale 1 ps / 1 ps
// lib IP_Integrator_Lib
module design_1_wrapper
	(
		output wire [13:0] EXT_DDR3_addr,
		output wire [2:0] EXT_DDR3_ba,
		output wire EXT_DDR3_cas_n,
		output wire [0:0] EXT_DDR3_ck_n,
		output wire [0:0] EXT_DDR3_ck_p,
		output wire [0:0] EXT_DDR3_cke,
		output wire [0:0] EXT_DDR3_cs_n,
		output wire [7:0] EXT_DDR3_dm,
		inout wire [63:0] EXT_DDR3_dq,
		inout wire [7:0] EXT_DDR3_dqs_n,
		inout wire [7:0] EXT_DDR3_dqs_p,
		output wire [0:0] EXT_DDR3_odt,
		output wire EXT_DDR3_ras_n,
		output wire EXT_DDR3_reset_n,
		output wire EXT_DDR3_we_n,
		input wire EXT_PCIE_REFCLK_P,
		input wire EXT_PCIE_REFCLK_N,
		input wire [3:0] EXT_PCIE_rxn,
		input wire [3:0] EXT_PCIE_rxp,
		output wire [3:0] EXT_PCIE_txn,
		output wire [3:0] EXT_PCIE_txp,
		input wire EXT_SYS_CLK_clk_n,
		input wire EXT_SYS_CLK_clk_p,
		input wire EXT_SYS_RST,
//		output wire ddr_clk_100MHz,//not need
//		output wire ddr_rdy,//not need
//		output wire ddr_rst,//not need
//		output wire pcie_clk_125MHz,//not need
		output wire [7:0] EXT_LEDS
	);

	wire ddr_clk_100MHz;//not need
	wire ddr_rdy;//not need
	wire ddr_rst;//not need
	wire pcie_clk_125MHz;//not need

	reg [27:0] pcie_clk_counter;
	reg [27:0] ddr_clk_counter;
	wire mmcms_locked;
	wire pcie_mmcm_locked;
	wire pcie_refclk_100MHz;

	always @(posedge pcie_clk_125MHz) begin
		  pcie_clk_counter = pcie_clk_counter + 1;
	end

	always @(posedge ddr_clk_100MHz) begin
		  ddr_clk_counter = ddr_clk_counter + 1;
	end

	assign EXT_LEDS = {
		ddr_clk_counter[27:26],
		pcie_clk_counter[27:26],
		pcie_mmcm_locked,
		ddr_rdy,
		~ddr_rst,
		~EXT_SYS_RST
		}; 

	assign mmcms_locked = pcie_mmcm_locked & ddr_rdy;

	IBUFDS_GTE2 pcie_refclk_buf
		(
			.I(EXT_PCIE_REFCLK_P),
			.CEB(1'b0),
			.IB(EXT_PCIE_REFCLK_N),
			.ODIV2(),
			.O(pcie_refclk_100MHz)
		);

design_1 design_1_i
	(
		.EXT_DDR3_addr(EXT_DDR3_addr),
		.EXT_DDR3_ba(EXT_DDR3_ba),
		.EXT_DDR3_cas_n(EXT_DDR3_cas_n),
		.EXT_DDR3_ck_n(EXT_DDR3_ck_n),
		.EXT_DDR3_ck_p(EXT_DDR3_ck_p),
		.EXT_DDR3_cke(EXT_DDR3_cke),
		.EXT_DDR3_cs_n(EXT_DDR3_cs_n),
		.EXT_DDR3_dm(EXT_DDR3_dm),
		.EXT_DDR3_dq(EXT_DDR3_dq),
		.EXT_DDR3_dqs_n(EXT_DDR3_dqs_n),
		.EXT_DDR3_dqs_p(EXT_DDR3_dqs_p),
		.EXT_DDR3_odt(EXT_DDR3_odt),
		.EXT_DDR3_ras_n(EXT_DDR3_ras_n),
		.EXT_DDR3_reset_n(EXT_DDR3_reset_n),
		.EXT_DDR3_we_n(EXT_DDR3_we_n),
		.EXT_PCIE_REFCLK_100MHz(pcie_refclk_100MHz),
		.EXT_PCIE_rxn(EXT_PCIE_rxn),
		.EXT_PCIE_rxp(EXT_PCIE_rxp),
		.EXT_PCIE_txn(EXT_PCIE_txn),
		.EXT_PCIE_txp(EXT_PCIE_txp),
		.EXT_SYS_CLK_clk_n(EXT_SYS_CLK_clk_n),
		.EXT_SYS_CLK_clk_p(EXT_SYS_CLK_clk_p),
		.EXT_SYS_RST(EXT_SYS_RST),
		.ddr_clk_100MHz(ddr_clk_100MHz),
		.ddr_rdy(ddr_rdy),
		.ddr_rst(ddr_rst),
		.pcie_clk_125MHz(pcie_clk_125MHz),
		.pcie_mmcm_locked(pcie_mmcm_locked),
		.reset_logic_mmcm_locked_in(mmcms_locked)
	);

endmodule
