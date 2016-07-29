`timescale 1 ns / 1 ps

module csa_wrap #
	(
		parameter integer CSA_CALC_INST_NUM = 4,
	
		parameter integer C_S00_AXI_ID_WIDTH = 1,
		parameter integer C_S00_AXI_DATA_WIDTH = 32,
		parameter integer C_S00_AXI_ADDR_WIDTH = 12,
		parameter integer C_S00_AXI_AWUSER_WIDTH = 1,
		parameter integer C_S00_AXI_ARUSER_WIDTH = 1,
		parameter integer C_S00_AXI_WUSER_WIDTH = 1,
		parameter integer C_S00_AXI_RUSER_WIDTH = 1,
		parameter integer C_S00_AXI_BUSER_WIDTH = 1,

		parameter integer C_S00_AXIS_TDATA_WIDTH = 32,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1
	)
	(
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
		input wire s00_axi_rready,


		input wire s00_axis_aclk,
		input wire s00_axis_aresetn,
		output wire s00_axis_tready,
		input wire [C_S00_AXIS_TDATA_WIDTH - 1 : 0] s00_axis_tdata,
		input wire [(C_S00_AXIS_TDATA_WIDTH / 8) - 1 : 0] s00_axis_tstrb,
		input wire s00_axis_tlast,
		input wire s00_axis_tvalid,

		input wire m00_axis_aclk,
		input wire m00_axis_aresetn,
		output wire m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata,
		output wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb,
		output wire m00_axis_tlast,
		input wire m00_axis_tready
	);

	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2=0; bit_depth>0; clogb2=clogb2+1)
				bit_depth = bit_depth >> 1;
		end
	endfunction

	localparam integer ADDR_LSB = clogb2((C_S00_AXI_DATA_WIDTH / 8) - 1);

	localparam integer AXI_DATA_WIDTH = 32;
	localparam integer OPT_MEM_ADDR_BITS = C_S00_AXI_ADDR_WIDTH - ADDR_LSB;

	localparam integer CYPHER_DATA_WIDTH = 8 * 8;
	localparam integer CSA_CALC_IN_WIDTH = 8 * 8;
	localparam integer CSA_CALC_OUT_WIDTH = 8 * 8;

	localparam integer CSA_IN_PARAMETER_LENGTH = AXI_DATA_WIDTH * 5;
	localparam integer CSA_OUT_PARAMETER_LENGTH = AXI_DATA_WIDTH * 7;

	localparam integer NUMBER_OF_INPUT_WORDS = 50;
	localparam integer NUMBER_OF_OUTPUT_WORDS = 70;
	localparam integer BULK_OF_INPUT_DATA = 5;
	localparam integer BULK_OF_OUTPUT_DATA = 70;
	localparam integer FIFO_RAM_INPUT_DEPTH = 10 * 160;
	localparam integer FIFO_RAM_OUTPUT_DEPTH = 1 * 160;

	wire user_rst_n;
	wire rst_n;

	assign rst_n = (user_rst_n == 1 && s00_axi_aresetn == 1 && s00_axis_aresetn == 1 && m00_axis_aresetn == 1) ? 1 : 0;


	wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] axi_wstrb;
	wire axi_wen;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] axi_wdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] axi_waddr;

	wire axi_ren;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] axi_rdata;
	wire [OPT_MEM_ADDR_BITS - 1 : 0] axi_raddr;

	assign axi_wstrb = s00_axi_wstrb;

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
			.wen(axi_wen),
			.wdata(axi_wdata),
			.waddr(axi_waddr),

			.ren(axi_ren),
			.rdata(axi_rdata),
			.raddr(axi_raddr),

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


	wire axis_s_rclk;
	wire axis_s_ren;
	wire [C_S00_AXIS_TDATA_WIDTH - 1 : 0] axis_s_rdata;

	wire axis_s_r_ready;
	wire axis_s_error_full;
	wire axis_s_error_empty;

	axi4_stream_slave_v1_0 #(
			.NUMBER_OF_INPUT_WORDS(NUMBER_OF_INPUT_WORDS),

			.BULK_OF_DATA(BULK_OF_INPUT_DATA),
			.BULK_DEPTH(FIFO_RAM_INPUT_DEPTH),

			.C_S00_AXIS_TDATA_WIDTH(C_S00_AXIS_TDATA_WIDTH)
		) axi4_stream_slave_v1_0_inst (
			.rclk(axis_s_rclk),
			.ren(axis_s_ren),
			.rdata(axis_s_rdata),

			.r_ready(axis_s_r_ready),
			.error_full(axis_s_error_full),
			.error_empty(axis_s_error_empty),

			.s00_axis_aclk(s00_axis_aclk),
			.s00_axis_aresetn(rst_n),
			.s00_axis_tready(s00_axis_tready),
			.s00_axis_tdata(s00_axis_tdata),
			.s00_axis_tstrb(s00_axis_tstrb),
			.s00_axis_tlast(s00_axis_tlast),
			.s00_axis_tvalid(s00_axis_tvalid)
		);

	wire axis_m_wclk;
	wire axis_m_wen;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] axis_m_wdata;

	wire axis_m_r_ready;
	wire axis_m_error_full;
	wire axis_m_error_empty;

	axi4_stream_master_v1_0 # (
			.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS),

			.BULK_OF_DATA(BULK_OF_OUTPUT_DATA),
			.BULK_DEPTH(FIFO_RAM_OUTPUT_DEPTH),

			.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
			.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
		) axi4_stream_master_v1_0_inst (
			.wclk(axis_m_wclk),
			.wen(axis_m_wen),
			.wdata(axis_m_wdata),

			.r_ready(axis_m_r_ready),
			.error_full(axis_m_error_full),
			.error_empty(axis_m_error_empty),

			.m00_axis_aclk(m00_axis_aclk),
			.m00_axis_aresetn(rst_n),
			.m00_axis_tvalid(m00_axis_tvalid),
			.m00_axis_tdata(m00_axis_tdata),
			.m00_axis_tstrb(m00_axis_tstrb),
			.m00_axis_tlast(m00_axis_tlast),
			.m00_axis_tready(m00_axis_tready)
		);

	csa_ram #(
			.AXI_DATA_WIDTH(AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),

			.CSA_CALC_INST_NUM(CSA_CALC_INST_NUM),

			.CYPHER_DATA_WIDTH(CYPHER_DATA_WIDTH),
			.CSA_CALC_IN_WIDTH(CSA_CALC_IN_WIDTH),
			.CSA_CALC_OUT_WIDTH(CSA_CALC_OUT_WIDTH),

			.CSA_IN_PARAMETER_LENGTH(CSA_IN_PARAMETER_LENGTH),
			.CSA_OUT_PARAMETER_LENGTH(CSA_OUT_PARAMETER_LENGTH)
		) csa_ram_inst(
			.axi_mm_clk(s00_axi_aclk),
			.s00_axi_aresetn(s00_axi_aresetn),

			.wstrb(axi_wstrb),
			.wen(axi_wen),
			.wdata(axi_wdata),
			.waddr(axi_waddr),

			.ren(axi_ren),
			.rdata(axi_rdata),
			.raddr(axi_raddr),

			.csa_calc_clk(s00_axi_aclk),
			.user_rst_n(user_rst_n),
			.rst_n(rst_n),

			.csa_in_r_ready(axis_s_r_ready),
			.csa_in_rclk(axis_s_rclk),
			.csa_in_ren(axis_s_ren),
			.csa_in_rdata(axis_s_rdata),

			.csa_out_error_full(axis_m_error_full),
			.csa_out_wclk(axis_m_wclk),
			.csa_out_wen(axis_m_wen),
			.csa_out_wdata(axis_m_wdata)
		);
	// User logic ends

endmodule
