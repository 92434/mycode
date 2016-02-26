`timescale 1ns/1ns

module tb #
	(
		parameter integer BYTE_WIDTH = 8,
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	();
	wire clk;
	reg rst_n = 0;
	initial begin
		#5;
		rst_n = 1;
	end

	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	always @(posedge clk) begin
		if(rst_n == 0) begin
		end
		else begin
		end
	end

	wire [BYTE_WIDTH - 1 : 0] lcm_data_origin;
	wire lcm_din;
	wire lcm_lp;
	wire xscl;
	wire [BYTE_WIDTH - 1 : 0] lcm_data;

	lcm_controler_eg9013f_nz_ram #(
			.BYTE_WIDTH(BYTE_WIDTH),
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) lcm_controler_eg9013f_nz_ram_inst (
			.clk(clk),
			.rst_n(rst_n),

			.lcm_data_origin(lcm_data_origin),
			.lcm_din(lcm_din),
			.lcm_lp(lcm_lp),
			.xscl(xscl),
			.lcm_data(lcm_data)
		);
endmodule
