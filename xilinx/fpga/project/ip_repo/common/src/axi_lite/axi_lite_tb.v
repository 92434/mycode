`timescale 1 ns / 1 ps

module tb #(
		parameter integer C_S_AXI_DATA_WIDTH	= 32,
		parameter integer C_S_AXI_ADDR_WIDTH	= 4
	)
	();

	wire s00_axi_aclk;
	reg s00_axi_aresetn = 0;

	reg [C_S_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr = 0;
	reg [2 : 0] s00_axi_awprot = 0;//unused
	reg s00_axi_awvalid = 0;
	wire s00_axi_awready;


	reg [C_S_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata = 0;
	reg [(C_S_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb = 0;
	reg s00_axi_wvalid = 0;
	wire s00_axi_wready;

	wire s00_axi_bresp;//unused
	wire s00_axi_bvalid;// axi_awready && axi_wready
	reg s00_axi_bready = 0;//response for resetting s00_axi_bvalid;

	reg [C_S_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr = 0;
	reg [2 : 0] s00_axi_arprot = 0;//unused
	reg s00_axi_arvalid = 0;
	wire s00_axi_arready;

	reg s00_axi_rready = 0;
	wire [C_S_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata;
	wire [1 : 0] s00_axi_rresp;
	wire s00_axi_rvalid;

	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(s00_axi_aclk));

	axi_lite_v1_0_wrap #(
			.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
			.C_S_AXI_ADDR_WIDTH(C_S_AXI_ADDR_WIDTH)
		)
		axi_lite_v1_0_wrap_inst (
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

	initial begin
		s00_axi_aresetn <= 0;
		#2;
		s00_axi_aresetn <= 1;
	end

	integer state = 0;
	always @(posedge s00_axi_aclk) begin
		if(s00_axi_aresetn == 0) begin
			s00_axi_awaddr <= 0;
			s00_axi_awprot <= 0;
			s00_axi_awvalid <= 0;

			s00_axi_wdata <= 0;
			s00_axi_wstrb <= 0;
			s00_axi_wvalid <= 0;

			s00_axi_bready <= 0;//response for resetting s00_axi_bvalid;

			s00_axi_araddr <= 0;
			s00_axi_arprot <= 0;
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
					s00_axi_wdata <= {32'h12345678};
					state <= 3;
				end
				3: begin
					s00_axi_wstrb <= {4'b1111};
					s00_axi_wvalid <= 1;
					state <= 4;
				end
				4: begin
					state <= 5;
				end
				5: begin
					s00_axi_wvalid <= 0;
					s00_axi_wstrb <= 0;
					state <= 6;
				end
				6: begin
					s00_axi_wdata <= {32'h00000000};
					state <= 7;
				end
				7: begin
					s00_axi_wstrb <= {4'b1000};
					s00_axi_wvalid <= 1;
					state <= 8;
				end
				8: begin
					state <= 9;
				end
				9: begin
					s00_axi_wvalid <= 0;
					s00_axi_wstrb <= 0;
					state <= 10;
				end
				10: begin
					state <= 11;
				end
				11: begin
					state <= 12;
				end
				12: begin
					s00_axi_awvalid <= 0;
					state <= 13;
				end
				13: begin
					s00_axi_awaddr <= 0;
					state <= 14;
				end
				14: begin
					s00_axi_bready <= 1;
					state <= 15;
				end
				15: begin
					s00_axi_bready <= 0;
					state <= 16;
				end
				16: begin
					s00_axi_araddr <= 0;
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
					s00_axi_arvalid <= 0;
					s00_axi_rready <= 0;
					state <= 20;
				end
				20: begin
					state <= 0;
				end
				default: begin
				end
			endcase
		end
	end
	
endmodule
