`timescale 1ns / 1ns

module tp #(
		parameter integer C_S00_AXI_ID_WIDTH = 1,
		parameter integer C_S00_AXI_DATA_WIDTH = 32,
		parameter integer C_S00_AXI_ADDR_WIDTH = 13,
		parameter integer C_S00_AXI_AWUSER_WIDTH = 0,
		parameter integer C_S00_AXI_ARUSER_WIDTH = 0,
		parameter integer C_S00_AXI_WUSER_WIDTH = 0,
		parameter integer C_S00_AXI_RUSER_WIDTH = 0,
		parameter integer C_S00_AXI_BUSER_WIDTH = 0
	)
	(
	);

	wire s00_axi_aclk;//gen input
	reg s00_axi_aresetn = 0;//input
	reg [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_awid = 0;//const
	reg [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_awaddr = 0;//input
	reg [7 : 0] s00_axi_awlen = 1;//const
	reg [2 : 0] s00_axi_awsize = 4;//const
	reg [1 : 0] s00_axi_awburst = 0;//const
	reg s00_axi_awlock = 0;//const
	reg [3 : 0] s00_axi_awcache = 0;//const
	reg [2 : 0] s00_axi_awprot = 0;//const
	reg [3 : 0] s00_axi_awqos = 0;//const
	reg [3 : 0] s00_axi_awregion = 0;//const
	reg [C_S00_AXI_AWUSER_WIDTH - 1 : 0] s00_axi_awuser = 0;//const
	reg s00_axi_awvalid = 0;//input
	wire s00_axi_awready;//output
	reg [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_wdata = 0;//input
	reg [(C_S00_AXI_DATA_WIDTH / 8) - 1 : 0] s00_axi_wstrb = 0;//input
	reg s00_axi_wlast = 0;//input
	reg [C_S00_AXI_WUSER_WIDTH - 1 : 0] s00_axi_wuser = 0;//const
	reg s00_axi_wvalid = 0;//input
	wire s00_axi_wready;//output
	wire [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_bid;//output equal to s00_axi_awid
	wire [1 : 0] s00_axi_bresp;//output
	wire [C_S00_AXI_BUSER_WIDTH - 1 : 0] s00_axi_buser;//output Z
	wire s00_axi_bvalid;//output
	reg s00_axi_bready = 0;//input for reset axi_bvalid
	reg [C_S00_AXI_ID_WIDTH - 1 : 0] s00_axi_arid = 0;//const
	reg [C_S00_AXI_ADDR_WIDTH - 1 : 0] s00_axi_araddr = 0;//input
	reg [7 : 0] s00_axi_arlen = 1;//const
	reg [2 : 0] s00_axi_arsize = 4;//const
	reg [1 : 0] s00_axi_arburst = 0;//const
	reg s00_axi_arlock = 0;//const
	reg [3 : 0] s00_axi_arcache = 0;//const
	reg [2 : 0] s00_axi_arprot = 0;//const
	reg [3 : 0] s00_axi_arqos = 0;//const
	reg [3 : 0] s00_axi_arregion = 0;//const
	reg [C_S00_AXI_ARUSER_WIDTH - 1 : 0] s00_axi_aruser = 0;//const
	reg s00_axi_arvalid = 0;//input
	wire s00_axi_arready;//output
	wire [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_rid;//output equal to s00_axi_arid
	wire [C_S00_AXI_DATA_WIDTH - 1 : 0] s00_axi_rdata;//output
	wire [1 : 0] s00_axi_rresp;//output
	wire s00_axi_rlast;//output
	wire [C_S00_AXI_RUSER_WIDTH - 1 : 0] s00_axi_ruser;//output equal to s00_axi_aruser
	wire s00_axi_rvalid;//output
	reg s00_axi_rready = 0;//input for reset axi_arv_arr_flag

	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(s00_axi_aclk));

	axi4_mm_v1_0_wrap #(
			.C_S00_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
			.C_S00_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
		)
		axi4_mm_v1_0_wrap_inst (
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

	initial begin
		s00_axi_aresetn <= 0;
		#2;
		s00_axi_aresetn <= 1;
	end

	integer state = 0;
	always @(posedge s00_axi_aclk) begin
		if(s00_axi_aresetn == 0) begin
			s00_axi_awaddr <= 0;
			s00_axi_awvalid <= 0;
			s00_axi_wdata <= 0;
			s00_axi_wstrb <= 0;
			s00_axi_wvalid <= 0;
			s00_axi_wlast <= 0;

			s00_axi_bready <= 0;

			s00_axi_araddr <= 0;
			s00_axi_arvalid <= 0;
			s00_axi_rready <= 0;

			state <= 0;
		end
		else begin
			case(state)
				0: begin
					s00_axi_awaddr <= 0;
					state <= 1;
				end
				1: begin
					s00_axi_awvalid <= 1;
					state <= 2;
				end
				2: begin
					s00_axi_awvalid <= 0;
					state <= 3;
				end
				3: begin
					s00_axi_wdata <= {32'h12345678};
					state <= 4;
				end
				4: begin
					s00_axi_wvalid <= 1;
					state <= 5;
				end
				5: begin
					s00_axi_wvalid <= 0;
					state <= 6;
				end
				6: begin
					s00_axi_wstrb <= {4'b1111};
					s00_axi_wlast <= 1;
					state <= 7;
				end
				7: begin
					s00_axi_wstrb <= 0;
					s00_axi_wlast <= 0;
					state <= 8;
				end
				8: begin
					s00_axi_awvalid <= 1;
					state <= 9;
				end
				9: begin
					s00_axi_awvalid <= 0;
					state <= 10;
				end
				10: begin
					s00_axi_wdata <= {32'h00000000};
					state <= 11;
				end
				11: begin
					s00_axi_wvalid <= 1;
					state <= 12;
				end
				12: begin
					s00_axi_wvalid <= 0;
					state <= 13;
				end
				13: begin
					s00_axi_wstrb <= {4'b1000};
					s00_axi_wlast <= 1;
					state <= 14;
				end
				14: begin
					s00_axi_wstrb <= 0;
					s00_axi_wlast <= 0;
					state <= 15;
				end
				15: begin
					s00_axi_bready <= 1;
					state <= 16;
				end
				16: begin
					s00_axi_bready <= 0;
					state <= 17;
				end
				17: begin
					s00_axi_arvalid <= 1;
					state <= 18;
				end
				18: begin
					s00_axi_rready <= 1;
					state <= 19;
				end
				19: begin
					s00_axi_rready <= 0;
					state <= 20;
				end
				20: begin
					s00_axi_arvalid <= 0;
					state <= 0;
				end
				default: begin
				end
			endcase
		end
	end
endmodule
