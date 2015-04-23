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


module i2s_test #
	(
		parameter integer DATA_WIDTH = 32,
		parameter integer TEST_DATA_SIZE = 5,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer NUMBER_OF_OUTPUT_WORDS = 8                                               
	)
	();


	//for common
	reg rst;
	wire clk;

	//for test
	integer test_data_index = 0;
	reg [I2S_DATA_BIT_WIDTH - 1 : 0] testdata[TEST_DATA_SIZE - 1 : 0];


	//for sender
	wire bclk;
	wire read_sync;
	wire read_testdata_en;
	wire daclrc;
	wire dacdat;
	wire sender_data_valid;
	wire data_flag;

	//for receiver
	wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data;
	wire s_data_valid;

	//for fifo
	integer output_count = 0;

	wire [DATA_WIDTH - 1 : 0] dacdat_data;

	wire rclk;
	wire [DATA_WIDTH - 1 : 0] rdata;
	reg read_enable;
	wire output_ready;
	wire buffer_full_error;
	wire buffer_empty_error;


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
		
		rst = 0;
		#10
		rst = 1;
	end

	assign dacdat_data = {i2s_received_data[I2S_DATA_BIT_WIDTH], {(DATA_WIDTH - 1 - I2S_DATA_BIT_WIDTH){1'b0}}, i2s_received_data[I2S_DATA_BIT_WIDTH - 1 : 0]};


	clkgen #
		(
			.clk_period(1)
		) 
		xiaofeiclk
		(
			.clk(clk)
		);

	SEND_DATA_TO_WM8731 sender(
		.CLK(clk),
		.RST(rst),
		.DATA_SOURCE(testdata[test_data_index][(I2S_DATA_BIT_WIDTH - 1):0]),
		.BCLK(bclk),
		.READ_SYNC(read_sync),
		.READ_EN(read_testdata_en),
		.DACLRC(daclrc),
		.DACDAT(dacdat),
		.data_valid(sender_data_valid),
		.data_flag(data_flag)
		);

	always @(posedge read_testdata_en) begin
		if(rst == 0) begin
			test_data_index <= 0;
		end
		else begin
			if(test_data_index >= TEST_DATA_SIZE - 1) begin
				test_data_index <= 0;
			end
			else begin
				test_data_index <= test_data_index + 1;
			end
		end
	end

	receive_data_from_i2s #
		(
			.I2S_DATA_BIT_WIDTH(24)
		) receiver
		(
			.rst(rst),
			.bclk(bclk),
			.lrclk(daclrc),
			.sd(dacdat),
			//.i2s_din(i2s_din),
			.i2s_received_data(i2s_received_data),
			.s_data_valid(s_data_valid)
		);

	assign rclk = bclk;

	my_fifo #
		(
			.DATA_WIDTH(DATA_WIDTH),
			.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS)
		)
		xiaofei_fifo
		(
			.wclk(s_data_valid),
			.rclk(rclk),
			.wdata(dacdat_data),
			.rdata(rdata),
			.read_enable(read_enable),//stay 3 cycles
			.output_ready(output_ready),
			.buffer_full_error(buffer_full_error),
			.buffer_empty_error(buffer_empty_error)
		);

	always @(posedge bclk) begin
		if(output_ready == 1) begin
			read_enable <= 1;
		end

		if(read_enable == 1) begin
			output_count <= output_count + 1;
			if(output_count == NUMBER_OF_OUTPUT_WORDS - 1) begin
				read_enable <= 0;
				output_count <= 0;
			end
		end
	end

endmodule
