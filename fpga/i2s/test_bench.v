`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/04/07 10:54:57
// Design Name: 
// Module Name: test_bench.v
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
module test();

parameter DATA_WIDTH	= 32;
parameter NUMBER_OF_OUTPUT_WORDS = 3;

integer output_count = 0;

reg rst;
wire clk;
wire rclk;
wire [DATA_WIDTH - 1 : 0] rdata;
reg read_enable = 0;
wire output_ready;
wire buffer_full_error;
wire buffer_empty_error;

reg [DATA_WIDTH - 1 : 0] result;


initial begin
	read_enable = 0;
	output_count = 0;
	rst = 0;
	#10
	rst = 1;
end

clkgen #(.clk_period(1)) xiaofeiclk(.clk(clk));

clkgen #(.clk_period(2)) xiaofeirclk(.clk(rclk));

i2s_test xiaofeii2c_test(
	.rst(rst),
	.clk(clk),
	.rclk(rclk),
	.rdata(rdata),
	.read_enable(read_enable),
	.output_ready(output_ready),
	.buffer_full_error(buffer_full_error),
	.buffer_empty_error(buffer_empty_error)
	);

always @(posedge rclk) begin
	if(output_ready == 1) begin
		read_enable <= 1;
	end

	if(read_enable == 1) begin
		result <= rdata;
		output_count <= output_count + 1;

		if(output_count >= NUMBER_OF_OUTPUT_WORDS - 1) begin
			read_enable <= 0;
			output_count <= 0;
		end
	end
end

endmodule
