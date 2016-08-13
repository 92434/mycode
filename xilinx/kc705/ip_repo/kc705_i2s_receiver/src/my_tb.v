`timescale 1 ns / 1 ps

module tb #
	(
		parameter integer I2S_RECEIVER_NUM = 16,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1
	)
	(
	);

	localparam integer I2S_DATA_BIT_WIDTH = 24;

	wire [I2S_RECEIVER_NUM - 1 : 0] bclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] lrclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] sdata;

	wire r_ready;
	wire error_full;
	wire error_empty;

	wire [I2S_RECEIVER_NUM - 1 : 0] local_r_ready;
	wire [I2S_RECEIVER_NUM - 1 : 0] local_error_full;
	wire [I2S_RECEIVER_NUM - 1 : 0] local_error_empty;


	reg m00_axis_aresetn = 1;
	wire m00_axis_aclk;
	wire m00_axis_tvalid;
	wire [C_M00_AXIS_TDATA_WIDTH-1 : 0] m00_axis_tdata;
	wire [(C_M00_AXIS_TDATA_WIDTH/8)-1 : 0] m00_axis_tstrb;
	wire [I2S_DATA_BIT_WIDTH-1 : 0] data_source;
	wire m00_axis_tlast;
	reg m00_axis_tready = 1;

	initial begin
		m00_axis_aresetn = 0;
		#2
		m00_axis_aresetn = 1;
		m00_axis_tready = 1;
	end

	// Instantiation of Axi Bus Interface M00_AXIS
	i2s_wrap # ( 
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM)
	) i2s_wrap_inst (
		.bclk(bclk),
 		.lrclk(lrclk),
 		.sdata(sdata),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

		.local_r_ready(local_r_ready),
		.local_error_full(local_error_full),
		.local_error_empty(local_error_empty),

		.m00_axis_aclk(m00_axis_aclk),
		.m00_axis_aresetn(m00_axis_aresetn),
		.m00_axis_tvalid(m00_axis_tvalid),
		.m00_axis_tdata(m00_axis_tdata),
		.m00_axis_tstrb(m00_axis_tstrb),
		.m00_axis_tlast(m00_axis_tlast),
		.m00_axis_tready(m00_axis_tready)
	);

	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(m00_axis_aclk));

	//sender
	myip_i2s_sender_v1_0 #(
			.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM),
			.I2S_SENDER_TEST_DATA_WIDTH(I2S_DATA_BIT_WIDTH)
		) mysender (
			.rst_n(m00_axis_aresetn),
			.clk(m00_axis_aclk),
			
			.i2s_sender_bclk(bclk),
			.i2s_sender_lrclk(lrclk),
			.i2s_sender_sdata(sdata),
			.data_source(data_source)
		);

	// User logic ends

endmodule
