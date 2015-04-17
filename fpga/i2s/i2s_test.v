//`timescale 1ns / 1ps
`timescale 1ns / 1ns
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


module i2s_test();
reg [23:0] testdata[3:0];
parameter testdata_size = 4;
parameter sdsize = 24;
integer index = 0;
integer data_index = 0;
reg rst;
//wire bclk;
//wire read_sync;
//wire ren;
//wire daclrc;
//wire dacdat;
wire [sdsize:0] data;
wire [23:0] result;

initial begin
	data_index <= 0;
	
	testdata[index] = 24'h123456;
	index = index + 1;

	testdata[index] = 24'h345678;
	index = index + 1;

	testdata[index] = 24'h567890;
	index = index + 1;

	testdata[index] = 24'h901234;
	index = index + 1;
	rst = 0;
	#10
	rst = 1;
end

initial begin
end

clkgen xiaofeiclk(clk);
SEND_DATA_TO_WM8731 sender(
	clk,
	rst,
	testdata[data_index][(sdsize - 1):0],
	bclk,
	read_sync,
	ren,
	daclrc,
	dacdat,
	sender_data_valid
	);

receive_data_from_i2s receiver(
	rst,
	bclk,
	daclrc,
	dacdat,
//	i2s_din,
	data,
	recever_data_valid
	);

assign result = data[23:0];

always @(posedge ren) begin
	if(rst == 0) begin
		data_index <= 0;
	end
	else begin
		if(data_index >= testdata_size - 1) begin
			data_index <= 0;
		end
		else begin
			data_index <= data_index + 1;
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
