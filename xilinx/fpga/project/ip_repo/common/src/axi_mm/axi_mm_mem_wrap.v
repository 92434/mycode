`timescale 1ns / 1ps

module axi_mm_mem_wrap #(
		parameter integer C_S_AXI_DATA_WIDTH = 32,

		parameter integer OPT_MEM_ADDR_BITS = 10
	)
	(
		input wire clk,
		input wire rst_n,

		input wire [(C_S_AXI_DATA_WIDTH / 8) - 1 : 0] wstrb,
		input wire wen,
		input wire [C_S_AXI_DATA_WIDTH - 1 : 0] wdata,

		input wire ren,
		output wire [C_S_AXI_DATA_WIDTH - 1 : 0] rdata,

		input wire [OPT_MEM_ADDR_BITS : 0] addr,
	);

	// implement Block RAM(s)
	// for write command
	//
	reg [C_S_AXI_DATA_WIDTH - 1:0] data [0 : 1 - 1];

	integer byte_index;
	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
			slv_reg0 <= 0;
		end 
		else begin
			if (wen == 1) begin
				case (waddr)
					0: begin
						for(byte_index = 0; byte_index < (C_S_AXI_DATA_WIDTH / 8); byte_index = byte_index + 1) begin
							if(wstrb[byte_index] == 1) begin
								data[(byte_index * 8) +: 8] <= wdata[(byte_index * 8) +: 8];
							end
						end
					end
					default: begin
					end
				endcase
			end
		end
	end

	always @(posedge clk) begin
		if (ren == 1) begin
			case(addr)
				0: begin
					rdata <= data[addr];
				end
				default: begin
					rdata <= 0;
				end
			endcase
		end
	end
endmodule
