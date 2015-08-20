`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/08/19 15:09:50
// Design Name: 
// Module Name: intr_hub
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module intr_hub #
	(
		parameter integer INT_NUM = 32
	)
	(
		input wire [INT_NUM - 1:0] int_i,
		input wire clk,
		input wire rst,
		output wire int_o
	);

	wire [INT_NUM - 1:0] int_o_inst;

	//-------------intr Instantiation----------------------------------------------------------//
	genvar i;
	generate for (i=0; i<INT_NUM; i=i+1)
		begin : hub_instance
			intr_instance # 
			()
			intr_inst (
				.clk(clk),
				.rst(rst),
				.int_i(int_i[i]),
				.int_o(int_o_inst[i])
			);
		end
	endgenerate
	
	assign int_o = (int_o_inst == 0) ? 1'b0 : 1'b1;
endmodule
