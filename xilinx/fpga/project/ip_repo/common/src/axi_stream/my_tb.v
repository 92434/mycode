`timescale 1ns / 1ns

module tb #(
		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1
	)
	(
	);

	wire wclk;
	reg wen = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] wdata = 0;

	clkgen #(.clk_period(4)) xiaofeiclk1(.clk(wclk));

	//send ts
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] index = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] count = 1;

	always @(posedge wclk) begin
		if(m00_axis_aresetn == 0) begin
			index <= 0;
		end
		else begin
			wen <= 0;

			if((index % 2) == 0) begin
				wdata <= count;
				wen <= 1;

				count <= count + 1;
			end
			else begin
			end
			index <= index + 1;
		end
	end

	// Instantiation of Axi Bus Interface M00_AXIS
	wire r_ready;
	wire error_full;
	wire error_empty;

	reg m00_axis_aresetn = 1;
	wire m00_axis_aclk;
	wire m00_axis_tvalid;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata;
	wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb;
	wire m00_axis_tlast;
	reg m00_axis_tready = 1;

	clkgen #(.clk_period(1)) xiaofeiclk2(.clk(m00_axis_aclk));

	axi4_stream_v1_0_wrap # ( 
		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) axi4_stream_v1_0_wrap_inst (
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

	initial begin
		#2;
		m00_axis_aresetn = 1;

		m00_axis_aresetn = 0;
		#2
		m00_axis_aresetn = 1;
		m00_axis_tready = 1;
	end
	
endmodule
