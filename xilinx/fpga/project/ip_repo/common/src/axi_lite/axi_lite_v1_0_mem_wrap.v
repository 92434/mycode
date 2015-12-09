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

	reg [C_S_AXI_DATA_WIDTH - 1 : 0] slv_reg0 = 0;

	integer byte_index;
	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
			slv_reg0 <= 0;
		end 
		else begin
			if(wen == 1) begin
				case (waddr)
					0: begin
						for(byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH / 8) - 1; byte_index = byte_index + 1) begin
							if (wstrb[byte_index] == 1) begin
								// Respective byte enables are asserted as per write strobes 
								// Slave register 0
								slv_reg0[(byte_index * 8) +: 8] <= wdata[(byte_index * 8) +: 8];
							end 
						end
					end
					default : begin
						slv_reg0 <= slv_reg0;
					end
				endcase
			end
		end
	end 


	// Output register or memory read data
	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
			rdata <= 0;
		end 
		else begin 
			// When there is a valid read address (S_AXI_ARVALID) with 
			// acceptance of read address by the slave (axi_arready), 
			// output the read dada 
			if (ren == 1) begin
				// Address decoding for reading registers
				case (raddr)
					0 : rdata <= slv_reg0;
					default : rdata <= 0;
				endcase
			end 
		end
	end 
endmodule

