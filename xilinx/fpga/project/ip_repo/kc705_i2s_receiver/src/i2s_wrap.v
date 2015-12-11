
`timescale 1 ns / 1 ps

module i2s_wrap #
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
		input wire [I2S_RECEIVER_NUM - 1 : 0] bclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] lrclk,
		input wire [I2S_RECEIVER_NUM - 1 : 0] sdata,
		//output wire [I2S_RECEIVER_NUM - 1 : 0] i2s_din,

		output wire [I2S_RECEIVER_NUM - 1 : 0] local_r_ready,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_full,
		output wire [I2S_RECEIVER_NUM - 1 : 0] local_error_empty,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Master Bus Interface M00_AXIS
		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		input wire m00_axis_aclk,
		input wire m00_axis_aresetn,
		output wire m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata,
		output wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb,
		output wire m00_axis_tlast,
		input wire m00_axis_tready
	);

	wire wclk;
	wire wen;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] wdata;

	// Instantiation of Axi Bus Interface M00_AXIS
	axi4_stream_v1_0 # (
		.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) axi4_stream_v1_0_inst (
		.wclk(wclk),
		.wen(wen),
		.wdata(wdata),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

		.m00_axis_aclk(m00_axis_aclk),
		.m00_axis_aresetn(m00_axis_aresetn),
		.m00_axis_tvalid(m00_axis_tvalid),
		.m00_axis_tdata(m00_axis_tdata),
		.m00_axis_tstrb(m00_axis_tstrb),
		.m00_axis_tlast(m00_axis_tlast),
		.m00_axis_tready(m00_axis_tready)
	);

	localparam integer ID_WIDTH = 5;
	// Add user logic here
	i2s_receiver_wapper #(
		.ID_WIDTH(ID_WIDTH),
		.I2S_RECEIVER_NUM(I2S_RECEIVER_NUM),

		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH)
		) i2s_receiver_wapper_inst (
			.clk(m00_axis_aclk),
			.rst_n(m00_axis_aresetn),

			.bclk(bclk),
			.lrclk(lrclk),
			.sdata(sdata),
			//.i2s_din(i2s_din),

			.wclk(wclk),
			.wen(wen),
			.wdata(wdata),

			.local_r_ready(local_r_ready),
			.local_error_full(local_error_full),
			.local_error_empty(local_error_empty)
		);

	// User logic ends

endmodule
