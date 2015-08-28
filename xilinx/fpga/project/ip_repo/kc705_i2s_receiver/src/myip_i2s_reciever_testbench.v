`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer I2S_RECEIVER_NUM = 2,
		parameter I2S_DATA_BIT_WIDTH = 24,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 32
	)
	(
	);

	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_bclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_lrclk;
	wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_sdata;
	wire read_enable;
	wire [I2S_RECEIVER_NUM - 1 : 0] output_ready;
	wire [I2S_RECEIVER_NUM - 1 : 0] buffer_full_error;
	wire [I2S_RECEIVER_NUM - 1 : 0] buffer_empty_error;
	wire [I2S_RECEIVER_NUM - 1 : 0] chip_select;

	wire s_data_valid;
	wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data;
	wire [I2S_RECEIVER_NUM - 1:0] local_read_enable;


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
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM),
		.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) myreceiver (
		.i2s_receiver_bclk(i2s_receiver_bclk),
		.i2s_receiver_lrclk(i2s_receiver_lrclk),
		.i2s_receiver_sdata(i2s_receiver_sdata),
		.read_enable(read_enable),
		.output_ready(output_ready),
		.buffer_full_error(buffer_full_error),
		.buffer_empty_error(buffer_empty_error),
		.s_data_valid(s_data_valid),
		.i2s_received_data(i2s_received_data),
		.chip_select(chip_select),
		.local_read_enable(local_read_enable),

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
			.i2s_sender_rst(m00_axis_aresetn),
			.i2s_sender_clk(m00_axis_aclk),
			
			.i2s_sender_bclk(i2s_receiver_bclk),
			.i2s_sender_lrclk(i2s_receiver_lrclk),
			.i2s_sender_sdata(i2s_receiver_sdata),
			.data_source(data_source)
		);

	// User logic ends

endmodule
