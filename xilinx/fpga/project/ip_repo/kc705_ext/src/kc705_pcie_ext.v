`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/07/24 17:51:26
// Design Name: 
// Module Name: kc705_pcie_ext
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module kc705_pcie_ext #
	()
	(
		input wire ddr_clk_100MHz,
		input wire ddr_rst,
		input wire EXT_SYS_RST,
		
		input wire pcie_clk_125MHz,
		
		input wire ddr_rdy,
		input wire pcie_mmcm_locked,
		output wire mmcms_locked,


		input wire EXT_PCIE_REFCLK_P,
		input wire EXT_PCIE_REFCLK_N,
		output wire [0:0] pcie_refclk_100MHz,

		output wire [7:0] EXT_LEDS
	);

	reg [27:0] pcie_clk_counter;
	reg [27:0] ddr_clk_counter;

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

	IBUFDS_GTE2 pcie_refclk_buf (
			.I(EXT_PCIE_REFCLK_P),
			.CEB(1'b0),
			.IB(EXT_PCIE_REFCLK_N),
			.ODIV2(),
			.O(pcie_refclk_100MHz)
		);
endmodule
