`timescale 1ns / 1ps

module axi4_logic_ram_wrap #(
		parameter integer C_S_AXI_ADDR_WIDTH = 13,
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer ADDR_LSB = 2,
		parameter integer OPT_MEM_ADDR_BITS = 10,
		parameter integer USER_NUM_MEM = 1,
		parameter integer TSP_FILTER_NUM = 32
	)
	(
		input wire S_AXI_ACLK,
		input wire S_AXI_WVALID,
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire axi_rvalid,
		input wire axi_wready,
		input wire axi_arv_arr_flag,
		input wire axi_awv_awr_flag,
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] axi_araddr,
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] axi_awaddr,
		output reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata,
		input wire [7:0] mpeg_data,
		input wire mpeg_clk,
		input wire mpeg_valid,
		input wire mpeg_sync,
		input wire rst,
		output [7:0] ts_out,
		output ts_out_clk,
		output ts_out_valid
	);

	//----------------------------------------------
	//-- Signals for user logic memory space example
	//------------------------------------------------
	wire [OPT_MEM_ADDR_BITS:0] mem_address;
	wire [USER_NUM_MEM-1:0] mem_select;
	wire [C_S_AXI_DATA_WIDTH-1:0] mem_data_out[0 : USER_NUM_MEM-1];

	genvar i;
	generate
		if (USER_NUM_MEM >= 1) begin
			assign mem_select = 1;
			assign mem_address = (axi_arv_arr_flag ? axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB]:(axi_awv_awr_flag ? axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB]:0));
		end
	endgenerate

	// implement Block RAM(s)
	generate for(i=0; i<USER_NUM_MEM; i=i+1)
		begin:BRAM_GEN
			wire mem_rden;
			wire mem_wren;

			assign mem_wren = axi_wready && S_AXI_WVALID ;
			assign mem_rden = axi_arv_arr_flag ; //& ~axi_rvalid

			logic_ram #(
					.C_S_AXI_DATA_WIDTH(C_S_AXI_DATA_WIDTH),
					.OPT_MEM_ADDR_BITS(OPT_MEM_ADDR_BITS),
					.FILTER_MAX_NUM(TSP_FILTER_NUM)
				) logic_ram_inst(
					.S_AXI_ACLK(S_AXI_ACLK),
					.S_AXI_WSTRB(S_AXI_WSTRB),
					.S_AXI_WDATA(S_AXI_WDATA),
					.mem_rden(mem_rden),
					.mem_wren(mem_wren),
					.mem_address(mem_address),
					.axi_rdata(mem_data_out[i]),
					.mpeg_data(mpeg_data),
					.mpeg_clk(mpeg_clk),
					.mpeg_valid(mpeg_valid),
					.mpeg_sync(mpeg_sync),
					.rst(rst),
					.ts_out(ts_out),
					.ts_out_clk(ts_out_clk),
					.ts_out_valid(ts_out_valid)
				);

		end
	endgenerate

	//Output register or memory read data
	always @(mem_data_out, axi_rvalid) begin
		if (axi_rvalid) begin
			// Read address mux
			axi_rdata <= mem_data_out[0];
		end
		else begin
			axi_rdata <= 32'h00000000;
		end
	end

endmodule
