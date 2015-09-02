
`timescale 1ns / 1ps

module logic_ram #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,
		parameter integer OPT_MEM_ADDR_BITS = 3
	)
	(
		input wire S_AXI_ACLK,
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		input wire mem_rden,
		input wire mem_wren,
		input wire [OPT_MEM_ADDR_BITS:0] mem_address,
		output reg [C_S_AXI_DATA_WIDTH-1 : 0] axi_rdata
	);

	// implement Block RAM(s)
	reg [8-1:0] byte_ram [0 : 1024 * 8];

	genvar mem_byte_index;
	generate for(mem_byte_index=0; mem_byte_index<= (C_S_AXI_DATA_WIDTH/8-1); mem_byte_index=mem_byte_index+1)
		begin : BYTE_BRAM_GEN
			wire [8-1:0] data_in;
			wire [8-1:0] data_out;

			//assigning 8 bit data
			assign data_in = S_AXI_WDATA[(mem_byte_index*8+7) -: 8];
			assign data_out = byte_ram[mem_address * (C_S_AXI_DATA_WIDTH/8) + mem_byte_index];

			always @(posedge S_AXI_ACLK) begin
				if (mem_wren && S_AXI_WSTRB[mem_byte_index]) begin
					byte_ram[mem_address * (C_S_AXI_DATA_WIDTH/8) + mem_byte_index] <= data_in;
				end
			end

			always @(posedge S_AXI_ACLK) begin
				if (mem_rden) begin
					axi_rdata[(mem_byte_index*8+7) -: 8] <= data_out;
				end
			end
		end
	endgenerate
endmodule
