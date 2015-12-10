
`timescale 1 ns / 1 ps

module axi4_stream_v1_0_wrap #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Master Bus Interface M00_AXIS
		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 32
	)
	(
		// Users to add ports here
		input wire wclk,
		input wire wen,
		input wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] wdata,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Master Bus Interface M00_AXIS
		input wire m00_axis_aclk,
		input wire m00_axis_aresetn,
		output wire m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata,
		output wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb,
		output wire m00_axis_tlast,
		input wire m00_axis_tready
	);
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

	// Add user logic here

	// User logic ends

endmodule
