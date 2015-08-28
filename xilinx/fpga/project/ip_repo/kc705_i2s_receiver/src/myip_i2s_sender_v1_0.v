
`timescale 1 ns / 1 ps

module myip_i2s_sender_v1_0 #
	(
		parameter integer I2S_RECEIVER_NUM = 1,
		parameter integer I2S_SENDER_TEST_DATA_WIDTH = 24
	)
	(
		input wire i2s_sender_rst,
		input wire i2s_sender_clk,
		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_bclk,
		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_lrclk,
		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_sdata,
		output wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] data_source
	);

// Add user logic here

	wire [I2S_RECEIVER_NUM - 1 : 0] read_testdata_en;
 	wire [I2S_RECEIVER_NUM - 1 : 0] read_sync;
	wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] local_data_source [I2S_RECEIVER_NUM - 1 : 0];

	assign data_source = local_data_source[0];

	genvar i;
	generate for (i=0; i<I2S_RECEIVER_NUM; i=i+1)
		begin : sender_inst
			data_gen_counter # (
					.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
				) testdata (
					.clk(read_testdata_en[i]),
					.rst(i2s_sender_rst),
					.data_source(local_data_source[i])
				);

			send_data_by_i2s # (
					.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
				) sender (
					.clk(i2s_sender_clk),
					.rst(i2s_sender_rst),
					.read_sync(read_sync[i]),
					.data_source(local_data_source[i]),
					.read_data_source_en(read_testdata_en[i]),
					.bclk(i2s_sender_bclk[i]),
					.lrclk(i2s_sender_lrclk[i]),
					.sdata(i2s_sender_sdata[i])
				);
		end
	endgenerate
// User logic ends

endmodule
