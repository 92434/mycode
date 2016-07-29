`timescale 1 ns / 1 ps

module axi_lite_v1_0_wrap #(
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 3

	)
	(
		// Ports of Axi Slave Bus Interface S00_AXI
		input wire s00_axi_aclk,
		input wire s00_axi_aresetn,
		input wire [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_awaddr,
		input wire [2 : 0] s00_axi_awprot,
		input wire s00_axi_awvalid,
		output wire s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] s00_axi_wstrb,
		input wire s00_axi_wvalid,
		output wire s00_axi_wready,
		output wire [1 : 0] s00_axi_bresp,
		output wire s00_axi_bvalid,
		input wire s00_axi_bready,
		input wire [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_araddr,
		input wire [2 : 0] s00_axi_arprot,
		input wire s00_axi_arvalid,
		output wire s00_axi_arready,
		output wire [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire s00_axi_rvalid,
		input wire s00_axi_rready
	);

	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2 = 0; bit_depth > 0; clogb2 = clogb2 + 1)
				bit_depth = bit_depth >> 1;
		end
	endfunction

	localparam integer ADDR_LSB = clogb2((C_S00_AXI_DATA_WIDTH / 8) - 1);

	localparam integer OPT_MEM_ADDR_BITS = C_S00_AXI_ADDR_WIDTH - ADDR_LSB;

	wire clk;
	wire rst_n;

	wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;
	wire wen;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr;
	wire [C_S00_AXI_DATA_WIDTH - 1 : 0] wdata;

	wire ren;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr;
	wire [C_S00_AXI_DATA_WIDTH - 1 : 0] rdata;

	axi_lite_v1_0 #(
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),
			.ADDR_LSB(ADDR_LSB),

			.C_S00_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.C_S00_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
		) axi_lite_v1_0_inst (
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.waddr(waddr),
			.wdata(wdata),

			.ren(ren),
			.raddr(raddr),
			.rdata(rdata),

			.s00_axi_aclk(s00_axi_aclk),
			.s00_axi_aresetn(s00_axi_aresetn),
			.s00_axi_awaddr(s00_axi_awaddr),
			.s00_axi_awprot(s00_axi_awprot),
			.s00_axi_awvalid(s00_axi_awvalid),
			.s00_axi_awready(s00_axi_awready),
			.s00_axi_wdata(s00_axi_wdata),
			.s00_axi_wstrb(s00_axi_wstrb),
			.s00_axi_wvalid(s00_axi_wvalid),
			.s00_axi_wready(s00_axi_wready),
			.s00_axi_bresp(s00_axi_bresp),
			.s00_axi_bvalid(s00_axi_bvalid),
			.s00_axi_bready(s00_axi_bready),
			.s00_axi_araddr(s00_axi_araddr),
			.s00_axi_arprot(s00_axi_arprot),
			.s00_axi_arvalid(s00_axi_arvalid),
			.s00_axi_arready(s00_axi_arready),
			.s00_axi_rdata(s00_axi_rdata),
			.s00_axi_rresp(s00_axi_rresp),
			.s00_axi_rvalid(s00_axi_rvalid),
			.s00_axi_rready(s00_axi_rready)
		);

	axi_lite_v1_0_mem_wrap #(
			.C_S00_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) axi_lite_v1_0_mem_wrap_inst (
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.waddr(waddr),
			.wdata(wdata),

			.ren(ren),
			.raddr(raddr),
			.rdata(rdata)
		);
endmodule
