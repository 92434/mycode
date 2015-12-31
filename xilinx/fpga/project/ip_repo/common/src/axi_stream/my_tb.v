`timescale 1ns / 1ns

module tb #(
		parameter integer NUMBER_OF_OUTPUT_WORDS = 5,
		parameter integer NUMBER_OF_INPUT_WORDS = 5,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1,
		parameter integer C_S00_AXIS_TDATA_WIDTH = 32
	)
	(
	);

	wire wclk;
	reg wen = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] wdata = 0;

	clkgen #(.clk_period(4)) xiaofeiclk1(.clk(wclk));

	//send ts
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] index = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] count = 2;

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
	wire r_ready_master;
	wire error_full_master;
	wire error_empty_master;

	reg m00_axis_aresetn = 1;
	wire m00_axis_aclk;
	wire m00_axis_tvalid;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] m00_axis_tdata;
	wire [(C_M00_AXIS_TDATA_WIDTH / 8) - 1 : 0] m00_axis_tstrb;
	wire m00_axis_tlast;
	wire m00_axis_tready;

	clkgen #(.clk_period(1)) xiaofeiclk2(.clk(m00_axis_aclk));

	axi4_stream_master_v1_0 # ( 
		.NUMBER_OF_OUTPUT_WORDS(NUMBER_OF_OUTPUT_WORDS),
		.C_M00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M00_AXIS_START_COUNT(C_M00_AXIS_START_COUNT)
	) axi4_stream_master_v1_0_inst (
		.wclk(wclk),
		.wen(wen),
		.wdata(wdata),

		.r_ready(r_ready_master),
		.error_full(error_full_master),
		.error_empty(error_empty_master),

		.m00_axis_aclk(m00_axis_aclk),
		.m00_axis_aresetn(m00_axis_aresetn),
		.m00_axis_tvalid(m00_axis_tvalid),
		.m00_axis_tdata(m00_axis_tdata),
		.m00_axis_tstrb(m00_axis_tstrb),
		.m00_axis_tlast(m00_axis_tlast),
		.m00_axis_tready(m00_axis_tready)
	);

	reg ren = 0;
	wire [C_M00_AXIS_TDATA_WIDTH - 1 : 0] rdata;

	wire r_ready_slave;
	wire error_full_slave;
	wire error_empty_slave;

	integer state = 0;
	integer rcount = 0;

	always @(posedge m00_axis_aclk) begin
		if(m00_axis_aresetn == 0) begin
			state <= 0;
			rcount <= 0;
		end
		else begin
			ren <= 0;

			case(state)
				0: begin
					if(r_ready_slave == 1) begin
						state <= 1;
						rcount <= 0;
					end
					else begin
					end
				end
				1: begin
					if((rcount >= 0) && (rcount <= 5 - 1)) begin
						ren <= 1;
						rcount <= rcount + 1;
					end
					else begin
						state <= 0;
					end
				end
				default: begin
				end
			endcase
		end
	end

	axi4_stream_slave_v1_0 #(
			.NUMBER_OF_INPUT_WORDS(NUMBER_OF_INPUT_WORDS),
			.C_S00_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH)
		) axi4_stream_slave_v1_0_inst (
			.rclk(m00_axis_aclk),
			.ren(ren),
			.rdata(rdata),
			.r_ready(r_ready_slave),
			.error_full(error_full_slave),
			.error_empty(error_empty_slave),

			.s00_axis_aclk(m00_axis_aclk),
			.s00_axis_aresetn(m00_axis_aresetn),
			.s00_axis_tready(m00_axis_tready),
			.s00_axis_tdata(m00_axis_tdata),
			.s00_axis_tstrb(m00_axis_tstrb),
			.s00_axis_tlast(m00_axis_tlast),
			.s00_axis_tvalid(m00_axis_tvalid)
		);

	initial begin
		#2;
		m00_axis_aresetn = 1;

		m00_axis_aresetn = 0;
		#2
		m00_axis_aresetn = 1;
	end
	
endmodule
