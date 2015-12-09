
`timescale 1ns / 1ps

module logic_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	(
		input wire S_AXI_ACLK,
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire mem_wren,

		input wire mem_rden,
		input wire [OPT_MEM_ADDR_BITS:0] mem_address,
		output reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata
	);

	// implement Block RAM(s)
	reg [C_S_AXI_DATA_WIDTH-1:0] byte_ram [0 : 1024 * 2];
	integer i;

	//assigning 8 bit data
	always @(posedge S_AXI_ACLK) begin
		if (mem_wren) begin
			for(i = 0; i < (C_S_AXI_DATA_WIDTH / 8); i = i + 1) begin
				if(S_AXI_WSTRB[i] == 1) begin
					byte_ram[mem_address][(8 * i + 7) -: 8] <= S_AXI_WDATA[(8 * i + 7) -: 8];
				end
			end
		end
	end

	always @(posedge S_AXI_ACLK) begin
		if (mem_rden) begin
			axi_rdata <= byte_ram[mem_address];
		end
	end

endmodule
