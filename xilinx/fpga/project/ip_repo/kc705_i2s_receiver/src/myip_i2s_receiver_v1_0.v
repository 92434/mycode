
`timescale 1 ns / 1 ps

module myip_i2s_receiver_v1_0 #
	(
		// Users to add parameters here
		parameter integer I2S_RECEIVER_NUM = 32,

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Master Bus Interface M00_AXIS
		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 32
	)
	(
		// Users to add ports here
		input wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_bclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_lrclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] i2s_receiver_sdata,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		output wire [I2S_RECEIVER_NUM - 1 : 0] local_r_ready,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_full,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_empty,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Master Bus Interface M00_AXIS
		input wire m00_axis_aclk,
		input wire m00_axis_aresetn,
		output wire m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH-1 : 0] m00_axis_tdata,
		output wire [(C_M00_AXIS_TDATA_WIDTH/8)-1 : 0] m00_axis_tstrb,
		output wire m00_axis_tlast,
		input wire m00_axis_tready
	);
	// Instantiation of Axi Bus Interface M00_AXIS
	myip_i2s_receiver_v1_0_M00_AXIS # ( 
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM),

		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M_START_COUNT(C_M00_AXIS_START_COUNT)
	) myip_i2s_receiver_v1_0_M00_AXIS_inst (
		.i2s_receiver_bclk(i2s_receiver_bclk),
 		.i2s_receiver_lrclk(i2s_receiver_lrclk),
 		.i2s_receiver_sdata(i2s_receiver_sdata),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

		.local_r_ready(local_r_ready),
		.local_error_full(local_error_full),
		.local_error_empty(local_error_empty),

		.M_AXIS_ACLK(m00_axis_aclk),
		.M_AXIS_ARESETN(m00_axis_aresetn),
		.M_AXIS_TVALID(m00_axis_tvalid),
		.M_AXIS_TDATA(m00_axis_tdata),
		.M_AXIS_TSTRB(m00_axis_tstrb),
		.M_AXIS_TLAST(m00_axis_tlast),
		.M_AXIS_TREADY(m00_axis_tready)
	);

	// Add user logic here

	// User logic ends

endmodule
