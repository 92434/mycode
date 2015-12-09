
`timescale 1 ns / 1 ps

module tsp_dump_axi4_stream_v1_0 #
	(
		// Users to add parameters here
		parameter integer MPEG_DATA_WIDTH = 8,

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Master Bus Interface M00_AXIS
		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 32
	)
	(
		// Users to add ports here
		input wire ts_clk,
		input wire ts_valid,
		input wire ts_sync,
		input wire [MPEG_DATA_WIDTH - 1 : 0] ts_data,

		output wire r_ready,
		output wire error_full,
		output wire error_empty,

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
	tsp_dump_axi4_stream_v1_0_M00_AXIS # ( 
		.MPEG_DATA_WIDTH(MPEG_DATA_WIDTH),

		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M_START_COUNT(C_M00_AXIS_START_COUNT)
	) tsp_dump_axi4_stream_v1_0_M00_AXIS_inst (
		.ts_clk(ts_clk),
		.ts_valid(ts_valid),
		.ts_sync(ts_sync),
		.ts_data(ts_data),

		.r_ready(r_ready),
		.error_full(error_full),
		.error_empty(error_empty),

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
