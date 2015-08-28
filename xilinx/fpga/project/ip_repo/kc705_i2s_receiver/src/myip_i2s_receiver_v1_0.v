
`timescale 1 ns / 1 ps

module myip_i2s_receiver_v1_0 #
	(
		// Users to add parameters here
		parameter integer I2S_RECEIVER_NUM = 1,
		parameter integer I2S_DATA_BIT_WIDTH = 24,
		parameter integer NUMBER_OF_OUTPUT_WORDS = 8,

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
		output wire read_enable,
		output wire [I2S_RECEIVER_NUM - 1 : 0] output_ready,
		output wire [I2S_RECEIVER_NUM - 1 : 0] buffer_full_error,
		output wire [I2S_RECEIVER_NUM - 1 : 0] buffer_empty_error,
		output wire [I2S_RECEIVER_NUM - 1 : 0] chip_select,

		output wire s_data_valid,
		output wire [I2S_DATA_BIT_WIDTH:0] i2s_received_data,
		output wire [I2S_RECEIVER_NUM - 1:0] local_read_enable,

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
		.I2S_DATA_BIT_WIDTH(I2S_DATA_BIT_WIDTH),
		.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS),
		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M_START_COUNT(C_M00_AXIS_START_COUNT)
	) myip_i2s_receiver_v1_0_M00_AXIS_inst (
		.i2s_receiver_bclk(i2s_receiver_bclk),
 		.i2s_receiver_lrclk(i2s_receiver_lrclk),
 		.i2s_receiver_sdata(i2s_receiver_sdata),
		.output_ready(output_ready),
		.read_enable(read_enable),
		.buffer_full_error(buffer_full_error),
		.buffer_empty_error(buffer_empty_error),
		.chip_select(chip_select),

		.s_data_valid(s_data_valid),
		.i2s_received_data(i2s_received_data),
		.local_read_enable(local_read_enable),

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
