`timescale 1ns/1ns

module lcm_controler_eg9013f_nz_wrap #(
		parameter integer BYTE_WIDTH = 8,

		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_ID_WIDTH = 1,
		parameter integer C_S00_AXI_DATA_WIDTH = 32,
		parameter integer C_S00_AXI_ADDR_WIDTH = 12,
		parameter integer C_S00_AXI_AWUSER_WIDTH = 1,
		parameter integer C_S00_AXI_ARUSER_WIDTH = 1,
		parameter integer C_S00_AXI_WUSER_WIDTH = 1,
		parameter integer C_S00_AXI_RUSER_WIDTH = 1,
		parameter integer C_S00_AXI_BUSER_WIDTH = 1
	)
	(
		output wire [BYTE_WIDTH - 1 : 0] lcm_data_origin,
		output wire [BYTE_WIDTH - 1 : 0] lcm_data_reverse,
		output wire lcm_din,
		output wire lcm_lp,
		output wire lcm_xscl,
		output wire [BYTE_WIDTH - 1 : 0] lcm_data,

		// Ports of Axi Slave Bus Interface S00_AXI
		input wire s00_axi_aclk,
		input wire s00_axi_aresetn,
		input wire [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_awid,
		input wire [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_awaddr,
		input wire [7 : 0] s00_axi_awlen,
		input wire [2 : 0] s00_axi_awsize,
		input wire [1 : 0] s00_axi_awburst,
		input wire s00_axi_awlock,
		input wire [3 : 0] s00_axi_awcache,
		input wire [2 : 0] s00_axi_awprot,
		input wire [3 : 0] s00_axi_awqos,
		input wire [3 : 0] s00_axi_awregion,
		input wire [C_S00_AXI_AWUSER_WIDTH - 1 : 0] s00_axi_awuser,
		input wire s00_axi_awvalid,
		output wire s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] s00_axi_wstrb,
		input wire s00_axi_wlast,
		input wire [C_S00_AXI_WUSER_WIDTH - 1 : 0] s00_axi_wuser,
		input wire s00_axi_wvalid,
		output wire s00_axi_wready,
		output wire [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_bid,
		output wire [1 : 0] s00_axi_bresp,
		output wire [C_S00_AXI_BUSER_WIDTH - 1 : 0] s00_axi_buser,
		output wire s00_axi_bvalid,
		input wire s00_axi_bready,
		input wire [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_arid,
		input wire [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_araddr,
		input wire [7 : 0] s00_axi_arlen,
		input wire [2 : 0] s00_axi_arsize,
		input wire [1 : 0] s00_axi_arburst,
		input wire s00_axi_arlock,
		input wire [3 : 0] s00_axi_arcache,
		input wire [2 : 0] s00_axi_arprot,
		input wire [3 : 0] s00_axi_arqos,
		input wire [3 : 0] s00_axi_arregion,
		input wire [C_S00_AXI_ARUSER_WIDTH - 1 : 0] s00_axi_aruser,
		input wire s00_axi_arvalid,
		output wire s00_axi_arready,
		output wire [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_rid,
		output wire [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire s00_axi_rlast,
		output wire [C_S00_AXI_RUSER_WIDTH - 1 : 0] s00_axi_ruser,
		output wire s00_axi_rvalid,
		input wire s00_axi_rready
	);

	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2=0; bit_depth>0; clogb2=clogb2+1)
				bit_depth = bit_depth >> 1;
		end
	endfunction

	localparam integer ADDR_LSB = clogb2((C_S00_AXI_DATA_WIDTH / 8) - 1);

	localparam integer OPT_MEM_ADDR_BITS = C_S00_AXI_ADDR_WIDTH - ADDR_LSB;

	wire clk;
	wire rst_n;

	wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;
	wire wen;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] wdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr;

	wire ren;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] rdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr;


	assign clk = s00_axi_aclk;
	assign rst_n = s00_axi_aresetn;

	assign wstrb = s00_axi_wstrb;

	// Instantiation of Axi Bus Interface S00_AXI
	axi4_mm_v1_0 # ( 
			.ADDR_LSB(ADDR_LSB),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.C_S00_AXI_ID_WIDTH(C_S00_AXI_ID_WIDTH),
			.C_S00_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.C_S00_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH),
			.C_S00_AXI_AWUSER_WIDTH(C_S00_AXI_AWUSER_WIDTH),
			.C_S00_AXI_ARUSER_WIDTH(C_S00_AXI_ARUSER_WIDTH),
			.C_S00_AXI_WUSER_WIDTH(C_S00_AXI_WUSER_WIDTH),
			.C_S00_AXI_RUSER_WIDTH(C_S00_AXI_RUSER_WIDTH),
			.C_S00_AXI_BUSER_WIDTH(C_S00_AXI_BUSER_WIDTH)
		) axi4_mm_v1_0_inst (
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.s00_axi_aclk(s00_axi_aclk),
			.s00_axi_aresetn(s00_axi_aresetn),
			.s00_axi_awid(s00_axi_awid),
			.s00_axi_awaddr(s00_axi_awaddr),
			.s00_axi_awlen(s00_axi_awlen),
			.s00_axi_awsize(s00_axi_awsize),
			.s00_axi_awburst(s00_axi_awburst),
			.s00_axi_awlock(s00_axi_awlock),
			.s00_axi_awcache(s00_axi_awcache),
			.s00_axi_awprot(s00_axi_awprot),
			.s00_axi_awqos(s00_axi_awqos),
			.s00_axi_awregion(s00_axi_awregion),
			.s00_axi_awuser(s00_axi_awuser),
			.s00_axi_awvalid(s00_axi_awvalid),
			.s00_axi_awready(s00_axi_awready),
			.s00_axi_wdata(s00_axi_wdata),
			.s00_axi_wstrb(s00_axi_wstrb),
			.s00_axi_wlast(s00_axi_wlast),
			.s00_axi_wuser(s00_axi_wuser),
			.s00_axi_wvalid(s00_axi_wvalid),
			.s00_axi_wready(s00_axi_wready),
			.s00_axi_bid(s00_axi_bid),
			.s00_axi_bresp(s00_axi_bresp),
			.s00_axi_buser(s00_axi_buser),
			.s00_axi_bvalid(s00_axi_bvalid),
			.s00_axi_bready(s00_axi_bready),
			.s00_axi_arid(s00_axi_arid),
			.s00_axi_araddr(s00_axi_araddr),
			.s00_axi_arlen(s00_axi_arlen),
			.s00_axi_arsize(s00_axi_arsize),
			.s00_axi_arburst(s00_axi_arburst),
			.s00_axi_arlock(s00_axi_arlock),
			.s00_axi_arcache(s00_axi_arcache),
			.s00_axi_arprot(s00_axi_arprot),
			.s00_axi_arqos(s00_axi_arqos),
			.s00_axi_arregion(s00_axi_arregion),
			.s00_axi_aruser(s00_axi_aruser),
			.s00_axi_arvalid(s00_axi_arvalid),
			.s00_axi_arready(s00_axi_arready),
			.s00_axi_rid(s00_axi_rid),
			.s00_axi_rdata(s00_axi_rdata),
			.s00_axi_rresp(s00_axi_rresp),
			.s00_axi_rlast(s00_axi_rlast),
			.s00_axi_ruser(s00_axi_ruser),
			.s00_axi_rvalid(s00_axi_rvalid),
			.s00_axi_rready(s00_axi_rready)
		);

	// Add user logic here
	lcm_controler_eg9013f_nz_ram #(
			.BYTE_WIDTH(BYTE_WIDTH),
			.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS)
		) tsp_ram_inst(
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),
			.waddr(waddr),

			.ren(ren),
			.rdata(rdata),
			.raddr(raddr),

			.lcm_data_origin(lcm_data_origin),
			.lcm_data_reverse(lcm_data_reverse),
			.lcm_din(lcm_din),
			.lcm_lp(lcm_lp),
			.lcm_xscl(lcm_xscl),
			.lcm_data(lcm_data)
		);

	// User logic ends

endmodule
