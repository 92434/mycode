`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2015/04/07 10:54:57
// Design Name: 
// Module Name: i2s_test
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


module i2s_test(
	rst,
	clk,
	rclk,
	rdata,
	read_enable,
	output_ready,
	buffer_full_error,
	buffer_empty_error
	);
	
parameter DATA_WIDTH = 32;

//for test
parameter testdata_size = 5;
parameter sdsize = 24;
integer test_data_index = 0;
reg [sdsize - 1 : 0] testdata[testdata_size - 1 : 0];

input rst;
input clk;
input rclk;
output [DATA_WIDTH - 1 : 0] rdata;
input read_enable;
output output_ready;
output buffer_full_error;
output buffer_empty_error;

//for sender
wire clk;
wire dacbclk;
wire read_sync;
wire read_testdata_en;
wire daclrc;
wire dacdat;
wire sender_data_valid;
wire data_flag;

//for receiver
reg dacrst;
wire [sdsize:0] s_data_out;
wire s_data_valid;

//for fifo
parameter NUMBER_OF_OUTPUT_WORDS = 3;                                               
wire [DATA_WIDTH - 1 : 0] dacdat_data;
wire read_enable;
wire output_ready;
wire rclk;
wire [DATA_WIDTH - 1 : 0] rdata;


initial begin
	test_data_index = 0;
	
	testdata[test_data_index] = 24'h123456;
	test_data_index = test_data_index + 1;

	testdata[test_data_index] = 24'h345678;
	test_data_index = test_data_index + 1;

	testdata[test_data_index] = 24'h567890;
	test_data_index = test_data_index + 1;

	testdata[test_data_index] = 24'h789012;
	test_data_index = test_data_index + 1;

	testdata[test_data_index] = 24'h901234;
	test_data_index = test_data_index + 1;

	test_data_index = 0;
	
	dacrst = 0;
	#10
	dacrst = 1;
end

assign dacdat_data = {s_data_out[sdsize], {(DATA_WIDTH - 1 - sdsize){1'b0}}, s_data_out[sdsize - 1 : 0]};


//clkgen #(.clk_period(1)) xiaofeiclk(clk);
SEND_DATA_TO_WM8731 sender(
	.CLK(clk),
	.RST(dacrst),
	.DATA_SOURCE(testdata[test_data_index][(sdsize - 1):0]),
	.BCLK(dacbclk),
	.READ_SYNC(read_sync),
	.READ_EN(read_testdata_en),
	.DACLRC(daclrc),
	.DACDAT(dacdat),
	.data_valid(sender_data_valid),
	.data_flag(data_flag)
	);

receive_data_from_i2s #(.sdsize(24)) receiver(
	.rst(dacrst),
	.bclk(dacbclk),
	.lrclk(daclrc),
	.i2s_dout(dacdat),
//	.i2s_din(i2s_din),
	.s_data_out(s_data_out),
	.s_data_valid(s_data_valid)
	);

my_fifo xiaofei_fifo(
	.wclk(s_data_valid),
	.rclk(rclk),
	.wdata(dacdat_data),
	.rdata(rdata),
	.read_enable(read_enable),//stay 3 cycles
	.output_ready(output_ready),
	.buffer_full_error(buffer_full_error),
	.buffer_empty_error(buffer_empty_error)
	);

always @(posedge read_testdata_en) begin
	if(dacrst == 0) begin
		test_data_index <= 0;
	end
	else begin
		if(test_data_index >= testdata_size - 1) begin
			test_data_index <= 0;
		end
		else begin
			test_data_index <= test_data_index + 1;
		end
	end
end

endmodule

module my_fifo(
	wclk,
	rclk,
	wdata,
	rdata,
	read_enable,
	output_ready,
	buffer_full_error,
	buffer_empty_error
	);

parameter DATA_WIDTH = 32;
parameter NUMBER_OF_OUTPUT_WORDS = 3;

input wclk;
input rclk;
input [DATA_WIDTH - 1 : 0] wdata;
output [DATA_WIDTH - 1 : 0] rdata;
input read_enable;
output output_ready;
output buffer_full_error;
output buffer_empty_error;

wire wclk;
wire rclk;
wire [DATA_WIDTH - 1 : 0] wdata;
reg [DATA_WIDTH - 1 : 0] rdata;
wire read_enable;
wire output_ready;
reg buffer_full_error = 0;
reg buffer_empty_error = 0;

integer buffer_index_read = 0;
integer buffer_index_write = 0;
integer BUFFER_SIZE = NUMBER_OF_OUTPUT_WORDS * 8;

reg [DATA_WIDTH - 1 : 0] buffer[(NUMBER_OF_OUTPUT_WORDS * 8) - 1 : 0];

initial begin
	buffer_index_read <= 0;
	buffer_index_write <= 0;
	buffer_full_error <= 0;
end

always @(posedge wclk) begin
	if((buffer_index_write == buffer_index_read) && output_ready == 1) begin
		buffer_full_error <= 1;
	end
	else begin
		buffer_full_error <= 0;
	end
	
	buffer[buffer_index_write] <= wdata;
	if((buffer_index_write + 1) >= BUFFER_SIZE) begin
		buffer_index_write <= 0;
	end
	else begin
		buffer_index_write <= buffer_index_write + 1;
	end
end

assign output_ready = (buffer_index_write >= buffer_index_read) ?
			((buffer_index_write >= buffer_index_read + NUMBER_OF_OUTPUT_WORDS) ? 1 : 0)
			: ((buffer_index_write + BUFFER_SIZE >= buffer_index_read + NUMBER_OF_OUTPUT_WORDS) ? 1 : 0);

always @(negedge rclk) begin
	if(read_enable == 1) begin
		if(buffer_index_write == buffer_index_read) begin
			buffer_empty_error <= 1;
		end
		else begin
			buffer_empty_error <= 0;
		end

		rdata <= buffer[buffer_index_read];
		if((buffer_index_read + 1) >= BUFFER_SIZE) begin
			buffer_index_read <= 0;
		end
		else begin
			buffer_index_read <= buffer_index_read + 1;
		end
	end
end

endmodule


module clkgen(clk);

parameter clk_period = 5;

output clk;
reg clk;

initial begin 
clk <= 0;
	forever begin
		#clk_period;
		clk = ~clk;
	end
end

endmodule
