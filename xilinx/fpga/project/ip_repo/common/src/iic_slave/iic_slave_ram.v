`timescale 1ns / 1ps

module iic_slave_ram #(
		parameter integer IIC_SLAVE_DATA_WIDTH = 8,

		parameter integer OPT_MEM_ADDR_BITS = 8
	)
	(
		input wire clk,
		input wire rst_n,

		input wire wen,
		input wire [IIC_SLAVE_DATA_WIDTH - 1 : 0] wdata,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] waddr,

		input wire ren,
		output reg [IIC_SLAVE_DATA_WIDTH - 1 : 0] rdata = 0,
		input wire [OPT_MEM_ADDR_BITS - 1 : 0] raddr
	);

	// implement Block RAM(s)
	// for write command
	//
	localparam integer MEMSIZE = (1 << (OPT_MEM_ADDR_BITS));

	reg [IIC_SLAVE_DATA_WIDTH - 1 : 0] data[0 : MEMSIZE - 1];

	always @(posedge clk) begin
		if (rst_n == 1'b0) begin
		end 
		else begin
			if (wen == 1) begin
				data[waddr] <= wdata;
			end
			else begin
			end
		end
	end

	always @(posedge clk) begin
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
endmodule
