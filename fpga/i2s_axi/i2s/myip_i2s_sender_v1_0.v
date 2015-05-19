
`timescale 1 ns / 1 ps

module myip_i2s_sender_v1_0 #
	(
		parameter integer I2S_SENDER_TEST_DATA_WIDTH = 24
	)
	(
		input wire i2s_sender_rst,
		input wire  i2s_sender_clk,
		output wire i2s_sender_bclk,
		output wire i2s_sender_lrclk,
		output wire i2s_sender_sdata
	);

// Add user logic here

	wire read_testdata_en;

 	wire read_sync;
	wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] data_source;

 	data_gen_counter #
		(
			.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
		)
		testdata
		(
			.clk(read_testdata_en),
			.rst(i2s_sender_rst),
			.data_source(data_source)
 		);

 	send_data_by_i2s #
		(
			.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
		)
		sender
		(
			.clk(i2s_sender_clk),
			.rst(i2s_sender_rst),
			.read_sync(read_sync),
			.data_source(data_source),
			.read_data_source_en(read_testdata_en),
			.bclk(i2s_sender_bclk),
			.lrclk(i2s_sender_lrclk),
			.sdata(i2s_sender_sdata)
 		);
// User logic ends

endmodule
