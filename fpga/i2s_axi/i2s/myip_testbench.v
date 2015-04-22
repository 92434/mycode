`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer C_M00_AXIS_TDATA_WIDTH	= 32,
		parameter integer C_M00_AXIS_START_COUNT	= 32
	)
	(
	);

	wire [C_M00_AXIS_START_COUNT - 1 : 0] rdata;
	wire axis_tvalid;
	wire output_ready;
	wire buffer_full_error;
	wire buffer_empty_error;
	wire [3:0] read_pointer;

	wire m00_axis_aclk;
	reg m00_axis_aresetn;
	wire  m00_axis_tvalid;
	wire [C_M00_AXIS_TDATA_WIDTH-1 : 0] m00_axis_tdata;
	wire [(C_M00_AXIS_TDATA_WIDTH/8)-1 : 0] m00_axis_tstrb;
	wire  m00_axis_tlast;
	reg m00_axis_tready;

	initial begin
		m00_axis_aresetn = 0;
		#10
		m00_axis_aresetn = 1;
		m00_axis_tready = 1;
	end

	// Instantiation of Axi Bus Interface M00_AXIS
	myip_i2s_receiver_v1_0 # ( 
		.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) myreceiver (
		.rdata(rdata),
		.output_ready(output_ready),
		.axis_tvalid(axis_tvalid),
		.buffer_full_error(buffer_full_error),
		.buffer_empty_error(buffer_empty_error),
		.read_pointer(read_pointer),

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

	// User logic ends

endmodule
