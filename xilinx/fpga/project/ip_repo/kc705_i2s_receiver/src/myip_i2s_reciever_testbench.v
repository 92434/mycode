`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer I2S_RECEIVER_NUM = 32,
		parameter I2S_DATA_BIT_WIDTH = 24,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1
	)
	(
	);

	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_bclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_lrclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_sdata;

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
	myip_i2s_receiver_v1_0 # ( 
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM)
	) myreceiver (
		.i2s_receiver_bclk(i2s_receiver_bclk),
 		.i2s_receiver_lrclk(i2s_receiver_lrclk),
 		.i2s_receiver_sdata(i2s_receiver_sdata),

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
			
			.i2s_sender_bclk(i2s_receiver_bclk),
			.i2s_sender_lrclk(i2s_receiver_lrclk),
			.i2s_sender_sdata(i2s_receiver_sdata),
			.data_source(data_source)
		);

	// User logic ends

endmodule
