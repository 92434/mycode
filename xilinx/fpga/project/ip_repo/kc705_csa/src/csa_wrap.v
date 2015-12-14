`timescale 1 ns / 1 ps

module csa_wrap #
	(
		// Users to add parameters here
		parameter integer MAX_CAL_TIMES = 10,
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
		input wire s00_axi_rready,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		input wire m00_axis_aclk,
		input wire m00_axis_aresetn,
		output wire m00_axis_tvalid,
		output wire [C_S00_AXI_DATA_WIDTH - 1 : 0] m00_axis_tdata,
		output wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb,
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

	localparam integer OPT_MEM_ADDR_BITS = C_S00_AXI_ADDR_WIDTH - ADDR_LSB - 1;

	wire clk;
	wire rst_n;

	wire [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb;
	wire wen;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] wdata;
	wire [OPT_MEM_ADDR_BITS:0] waddr;

	wire ren;
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] rdata;
	wire [OPT_MEM_ADDR_BITS:0] raddr;


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




	wire wclk_48;
	wire wen_48;
	wire [48 - 1 : 0] wdata_48;

	// Add user logic here
	csa_ram #(
			.MAX_CAL_TIMES(MAX_CAL_TIMES),

			.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),
		) csa_ram_inst(
			.clk(clk),
			.rst_n(rst_n),

			.S_AXI_WSTRB(wstrb),
			.wen(wen),
			.S_AXI_WDATA(wdata),
			.waddr(waddr),

			.fpga_clk(wclk_48),
			.ready(wen_48),
			.byte_ram_out(wdata_48)
		);

	wire r_enable;
	wire [48 - 1 : 0] rdata_48 = 0;
	my_fifo #(
			.DATA_WIDTH(48),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(s00_axi_aresetn),
			.wclk(wclk_48),
			.rclk(s00_axi_aclk),
			.wdata(wdata_48),
			.rdata(rdata_48),
			.w_enable(wen_48),
			.r_enable(r_enable),

			.r_ready(r_enable),
			.error_full(),
			.error_empty()
		);

	reg ren_48 = 0;
	always @(negedge s00_axi_aclk) begin
		if(s00_axi_aresetn == 0) begin
			ren_48 <= 0;
		end
		else begin
			ren_48 <= r_enable;
		end
	end

	reg [C_S00_AXI_DATA_WIDTH - 1 : 0] wdata_32_0 = 0;
	reg [C_S00_AXI_DATA_WIDTH - 1 : 0] wdata_32_1 = 0;
	reg [C_S00_AXI_DATA_WIDTH - 1 : 0] wdata_32_2 = 0;
	reg start_stream_data48 = 0;
	reg end_stream_data48 = 1;

	integer get_data48_state = 0;
	always @(posedge s00_axi_aclk) begin
		if(s00_axi_aresetn == 0) begin
			get_data48_state <= 0;
			wdata_32_0 <= 0;
			wdata_32_1 <= 0;
			wdata_32_2 <= 0;
			start_stream_data48 <= 0;
		end
		else begin
			case(get_data48_state)
				0: begin
					if(end_stream_data48 == 1) begin
						if(ren_48 == 1) begin
							wdata_32_0 <= rdata_48[16 * 1 - 1 : 16 * 0];
							wdata_32_1 <= rdata_48[16 * 2 - 1 : 16 * 1];
							wdata_32_0 <= rdata_48[16 * 3 - 1 : 16 * 2];
							start_stream_data48 <= 1;
							get_data48_state <= 1;
						end
						else begin
							get_data48_state <= 0;
						end
					end
					else begin
						get_data48_state <= 0;
					end
				end
				1: begin
					start_stream_data48 <= 0;
					get_data48_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end


	wire stream_wclk;
	reg stream_wen = 0;
	reg [C_S00_AXI_DATA_WIDTH - 1 : 0] stream_wdata = 0;

	assign stream_wclk = s00_axi_aclk;

	integer stream_data48_state = 0;
	always @(posedge s00_axi_aclk) begin
		if(s00_axi_aresetn == 0) begin
			stream_wen = 0;
			stream_data48_state <= 0;
			end_stream_data48 = 1;
		end
		else begin
			case(stream_data48_state)
				0: begin
					if(start_stream_data48 == 1) begin
						end_stream_data48 <= 0;
						stream_data48_state <= 1;
					end
					else begin
						stream_data48_state <= 0;
					end
				end
				1: begin
					stream_wen <= 1;
					stream_wdata <= wdata_32_0;
					stream_data48_state <= 2;
				end
				2: begin
					stream_wen <= 1;
					stream_wdata <= wdata_32_1;
					stream_data48_state <= 3;
				end
				3: begin
					stream_wen <= 1;
					stream_wdata <= wdata_32_2;
					stream_data48_state <= 4;
				end
				4: begin
					stream_wen <= 0;
					end_stream_data48 <= 1;
					stream_data48_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	// Instantiation of Axi Bus Interface M00_AXIS
	axi4_stream_v1_0 # (
		.C_M00_AXIS_TDATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		//.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) axi4_stream_v1_0_inst (
		.wclk(stream_wclk),
		.wen(stream_wen),
		.wdata(stream_wdata),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

		.m00_axis_aclk(m00_axis_aclk),
		.m00_axis_aresetn(m00_axis_aresetn),
		.m00_axis_tvalid(m00_axis_tvalid),
		.m00_axis_tdata(m00_axis_tdata),
		.m00_axis_tstrb(m00_axis_tstrb),
		.m00_axis_tlast(m00_axis_tlast),
		.m00_axis_tready(m00_axis_tready)
	);
	// User logic ends

endmodule

