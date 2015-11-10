`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer I2S_RECEIVER_NUM = 2,
		parameter I2S_SENDER_TEST_DATA_WIDTH = 24

	)
	(
	);

	reg rst_n;
	wire clk;

	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_bclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_lrclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_sdata;
	wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] data_source;

	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end


	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(clk));

	myip_i2s_sender_v1_0 # ( 
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM),
		.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
	) mysender (
		.rst_n(rst_n),
 		.clk(clk),

		.i2s_sender_bclk(i2s_sender_bclk),
		.i2s_sender_lrclk(i2s_sender_lrclk),
		.i2s_sender_sdata(i2s_sender_sdata),

		.data_source(data_source)
	);
	// User logic ends

endmodule
