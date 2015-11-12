
`timescale 1 ns / 1 ps

module myip_i2s_sender_v1_0 #
	(
		parameter integer I2S_RECEIVER_NUM = 1,
		parameter integer I2S_SENDER_TEST_DATA_WIDTH = 24
	)
	(
		input wire rst_n,
		input wire clk,

		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_bclk,
		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_lrclk,
		output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_sender_sdata,
		output wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] data_source
	);

	reg clk1 = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			clk1 <= 0;
		end
		else begin
			clk1 <= ~clk1;
		end
	end

	reg clk2 = 0;
	always @(posedge clk1) begin
		if(rst_n == 0) begin
			clk2 <= 0;
		end
		else begin
			clk2 <= ~clk2;
		end
	end

	reg clk3 = 0;
	always @(posedge clk2) begin
		if(rst_n == 0) begin
			clk3 <= 0;
		end
		else begin
			clk3 <= ~clk3;
		end
	end

// Add user logic here

	wire [I2S_RECEIVER_NUM - 1 : 0] data_source_clk;
 	wire [I2S_RECEIVER_NUM - 1 : 0] read_sync;
	wire [I2S_SENDER_TEST_DATA_WIDTH - 1 :0] local_data_source [I2S_RECEIVER_NUM - 1 : 0];

	assign data_source = local_data_source[0];

	genvar i;
	generate for (i=0; i<I2S_RECEIVER_NUM; i=i+1)
		begin : sender_inst
			data_gen_counter # (
					.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
				) testdata (
					.clk(data_source_clk[i]),
					.rst_n(rst_n),
					.data_source(local_data_source[i])
				);

			send_data_by_i2s # (
					.I2S_SENDER_TEST_DATA_WIDTH(I2S_SENDER_TEST_DATA_WIDTH)
				) sender (
					.clk(clk3),
					.rst_n(rst_n),
					.read_sync(read_sync[i]),

					.data_source(local_data_source[i]),
					.data_source_clk(data_source_clk[i]),

					.bclk(i2s_sender_bclk[i]),
					.lrclk(i2s_sender_lrclk[i]),
					.sdata(i2s_sender_sdata[i])
				);
		end
	endgenerate
// User logic ends

endmodule
