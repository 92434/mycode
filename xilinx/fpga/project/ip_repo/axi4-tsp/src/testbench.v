`timescale 1 ns / 1 ps

module myip_testbench #
	(
		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_ID_WIDTH	= 1,
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		//parameter integer C_S00_AXI_ADDR_WIDTH	= 6,
		parameter integer C_S00_AXI_AWUSER_WIDTH	= 1,
		parameter integer C_S00_AXI_ARUSER_WIDTH	= 1,
		parameter integer C_S00_AXI_WUSER_WIDTH	= 1,
		parameter integer C_S00_AXI_RUSER_WIDTH	= 1,
		parameter integer C_S00_AXI_BUSER_WIDTH	= 1
	)
	(
	);

	// function called clogb2 that returns an integer which has the
	// value of the ceiling of the log base 2.
	function integer clogb2(input integer bit_depth);
		begin
			for(clogb2=0; bit_depth>0; clogb2=clogb2+1)
				bit_depth = bit_depth >> 1;
		end
	endfunction
	localparam integer C_S00_AXI_ADDR_WIDTH = clogb2(1024 * 8 - 1); //(C_S_AXI_DATA_WIDTH/32) + 1;

	// Ports of Axi Slave Bus Interface S00_AXI
	wire s00_axi_aclk; //input
	reg s00_axi_aresetn; //input
	reg [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_awid; //input
	reg [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr; //input
	reg [7 : 0] s00_axi_awlen; //input
	reg [2 : 0] s00_axi_awsize; //input
	reg [1 : 0] s00_axi_awburst; //input
	reg s00_axi_awlock; //input
	reg [3 : 0] s00_axi_awcache; //input
	reg [2 : 0] s00_axi_awprot; //input
	reg [3 : 0] s00_axi_awqos; //input
	reg [3 : 0] s00_axi_awregion; //input
	reg [C_S00_AXI_AWUSER_WIDTH-1 : 0] s00_axi_awuser; //input
	reg s00_axi_awvalid; //input
	wire s00_axi_awready; //output
	reg [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata; //input
	reg [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb; //input
	reg s00_axi_wlast; //input
	reg [C_S00_AXI_WUSER_WIDTH-1 : 0] s00_axi_wuser; //input
	reg s00_axi_wvalid; //input
	wire s00_axi_wready; //output
	wire [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_bid; //output
	wire [1 : 0] s00_axi_bresp; //output
	wire [C_S00_AXI_BUSER_WIDTH-1 : 0] s00_axi_buser; //output
	wire s00_axi_bvalid; //output
	reg s00_axi_bready; //input
	reg [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_arid; //input
	reg [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr; //input
	reg [7 : 0] s00_axi_arlen; //input
	reg [2 : 0] s00_axi_arsize; //input
	reg [1 : 0] s00_axi_arburst; //input
	reg s00_axi_arlock; //input
	reg [3 : 0] s00_axi_arcache; //input
	reg [2 : 0] s00_axi_arprot; //input
	reg [3 : 0] s00_axi_arqos; //input
	reg [3 : 0] s00_axi_arregion; //input
	reg [C_S00_AXI_ARUSER_WIDTH-1 : 0] s00_axi_aruser; //input
	reg s00_axi_arvalid; //input
	wire s00_axi_arready; //output
	wire [C_S00_AXI_ID_WIDTH-1 : 0] s00_axi_rid; //output
	wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata; //output
	wire [1 : 0] s00_axi_rresp; //output
	wire s00_axi_rlast; //output
	wire [C_S00_AXI_RUSER_WIDTH-1 : 0] s00_axi_ruser; //output
	wire s00_axi_rvalid; //output
	reg s00_axi_rready; //input

	initial begin
		s00_axi_aresetn = 0;
		#2
		s00_axi_aresetn = 1;

		s00_axi_awid = 0;
		s00_axi_awaddr = 3;//change
		s00_axi_awlen = 7;
		s00_axi_awsize = 0;
		s00_axi_awburst = 1;
		s00_axi_awlock = 0;
		s00_axi_awcache = 0;
		s00_axi_awprot = 0;
		s00_axi_awqos = 0;
		s00_axi_awregion = 0;
		s00_axi_awuser = 0;
		s00_axi_awvalid = 1;//change
		//s00_axi_awready;
		s00_axi_wdata = 32'h12345678;//change
		s00_axi_wstrb = 4'b1111;
		s00_axi_wlast = 0;//change
		s00_axi_wuser = 0;
		s00_axi_wvalid = 0;//change
		//s00_axi_wready;
		//s00_axi_bid;
		//s00_axi_bresp;
		//s00_axi_buser;
		//s00_axi_bvalid;
		s00_axi_bready = 0;//change

		#2
		s00_axi_awvalid = 0;
		s00_axi_wvalid = 1;



		//s00_axi_arid = 0;
		//s00_axi_araddr = 3;
		//s00_axi_arlen = 8;
		//s00_axi_arsize = 0;
		//s00_axi_arburst = 1;
		//s00_axi_arlock = 0;
		//s00_axi_arcache = 0;
		//s00_axi_arprot = 0;
		//s00_axi_arqos = 0;
		//s00_axi_arregion = 0;
		//s00_axi_aruser = 0;
		//s00_axi_arvalid = 1;//change
		////s00_axi_arready
		////s00_axi_rid;
		////s00_axi_rdata;
		////s00_axi_rresp;
		////s00_axi_rlast;
		////s00_axi_ruser;
		////s00_axi_rvalid;
		//s00_axi_rready = 0;//change
		
	end


	// Instantiation of Axi Bus Interface M00_AXIS
	axi4_wapper_v1_0 # ( 
		.C_S00_AXI_ID_WIDTH(C_S00_AXI_ID_WIDTH),
		.C_S00_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S00_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH),
		.C_S00_AXI_AWUSER_WIDTH(C_S00_AXI_AWUSER_WIDTH),
		.C_S00_AXI_ARUSER_WIDTH(C_S00_AXI_ARUSER_WIDTH),
		.C_S00_AXI_WUSER_WIDTH(C_S00_AXI_WUSER_WIDTH),
		.C_S00_AXI_RUSER_WIDTH(C_S00_AXI_RUSER_WIDTH),
		.C_S00_AXI_BUSER_WIDTH(C_S00_AXI_BUSER_WIDTH)
	) axi4_wapper_inst (
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
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(s00_axi_aclk));

	// User logic ends

endmodule
