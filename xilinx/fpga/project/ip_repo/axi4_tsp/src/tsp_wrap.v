`timescale 1 ns / 1 ps

module tsp_wrap #
	(
		// Users to add parameters here
		parameter integer MONITOR_FILTER_NUM = 2,
		parameter integer REPLACER_FILTER_NUM = 17,
		parameter integer REPLACE_MATCH_PID_COUNT = 1,
		parameter integer REPLACE_DATA_GROUPS = 1,
		parameter integer COMMON_REPLACER_FILTER_NUM = 1,
		parameter integer COMMON_REPLACE_MATCH_PID_COUNT = 16,
		parameter integer COMMON_REPLACE_DATA_GROUPS = 2,

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_ID_WIDTH = 1,
		parameter integer C_S00_AXI_DATA_WIDTH = 32,
		parameter integer C_S00_AXI_ADDR_WIDTH = 13,
		parameter integer C_S00_AXI_AWUSER_WIDTH = 1,
		parameter integer C_S00_AXI_ARUSER_WIDTH = 1,
		parameter integer C_S00_AXI_WUSER_WIDTH = 1,
		parameter integer C_S00_AXI_RUSER_WIDTH = 1,
		parameter integer C_S00_AXI_BUSER_WIDTH = 1
	)
	(
		// Users to add ports here
		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,

		output wire ts_out_clk,
		output wire ts_out_valid,
		output wire ts_out_sync,
		output wire [7:0] ts_out,
		// User ports ends
		// Do not modify the ports beyond this line


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

	localparam integer OPT_MEM_ADDR_BITS = C_S00_AXI_ADDR_WIDTH - ADDR_LSB - 1;

	wire clk;
	wire rst_n;

	wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;
	wire wen;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] wdata;

	wire ren;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] rdata;

	wire [OPT_MEM_ADDR_BITS:0] addr;


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
			.clk(clk),
			.rst_n(rst_n),

			.wen(wen),
			.wdata(wdata),

			.ren(ren),
			.rdata(rdata),

			.addr(addr),

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
	tsp_ram #(
			.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.MONITOR_FILTER_NUM(MONITOR_FILTER_NUM),
			.REPLACER_FILTER_NUM(REPLACER_FILTER_NUM),
			.REPLACE_MATCH_PID_COUNT(REPLACE_MATCH_PID_COUNT),
			.REPLACE_DATA_GROUPS(REPLACE_DATA_GROUPS),
			.COMMON_REPLACER_FILTER_NUM(COMMON_REPLACER_FILTER_NUM),
			.COMMON_REPLACE_MATCH_PID_COUNT(COMMON_REPLACE_MATCH_PID_COUNT),
			.COMMON_REPLACE_DATA_GROUPS(COMMON_REPLACE_DATA_GROUPS)
		) tsp_ram_inst(
			.clk(clk),
			.rst_n(rst_n),

			.wstrb(wstrb),
			.wen(wen),
			.wdata(wdata),

			.ren(ren),
			.rdata(rdata),

			.addr(addr),

			.mpeg_data(mpeg_data),
			.mpeg_clk(mpeg_clk),
			.mpeg_valid(mpeg_valid),
			.mpeg_sync(mpeg_sync),

			.ts_out_clk(ts_out_clk),
			.ts_out_valid(ts_out_valid),
			.ts_out_sync(ts_out_sync),
			.ts_out(ts_out)
		);

	// User logic ends

	endmodule
