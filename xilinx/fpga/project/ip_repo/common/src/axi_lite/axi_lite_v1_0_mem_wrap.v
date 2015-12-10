`timescale 1 ns / 1 ps

module axi_lite_v1_0_mem_wrap #(
		// Width of S_AXI data bus
		parameter integer C_S_AXI_DATA_WIDTH = 32,
			// Width of S_AXI address bus index by word
		parameter integer OPT_MEM_ADDR_BITS = 31
	)
	(
		input wire clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [OPT_MEM_ADDR_BITS : 0] waddr,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,

		input wire ren,
		input wire [OPT_MEM_ADDR_BITS : 0] raddr,
		output reg [C_S_AXI_DATA_WIDTH - 1 : 0] rdata = 0
	);

	localparam integer MEMSIZE = (1 << (OPT_MEM_ADDR_BITS + 1));
	reg [C_S_AXI_DATA_WIDTH - 1 : 0] data[0 : MEMSIZE - 1];

	integer byte_index;
	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
		end 
		else begin
			if (wen == 1) begin
				for(byte_index = 0; byte_index < (C_S_AXI_DATA_WIDTH / 8); byte_index = byte_index + 1) begin
					if(wstrb[byte_index] == 1) begin
						data[waddr][(byte_index * 8) +: 8] <= wdata[(byte_index * 8) +: 8];
					end
				end
			end
			else begin
			end
		end
	end


	// Output register or memory read data
	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
			rdata <= 0;
		end
		else begin
			if (ren == 1) begin
				if((raddr >= 0) && (raddr < MEMSIZE)) begin
					rdata <= data[raddr];
				end
				else begin
					rdata <= 0;
				end
			end
			else begin
			end
		end
	end
endmodule

