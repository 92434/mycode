`timescale 1ns / 1ns

module tb #(
		parameter integer NUMBER_OF_OUTPUT_WORDS = 14,
		parameter integer NUMBER_OF_INPUT_WORDS = 3,

		parameter integer C_M00_AXIS_TDATA_WIDTH = 32,
		parameter integer C_M00_AXIS_START_COUNT = 1,
		parameter integer C_S00_AXIS_TDATA_WIDTH = 32
	)
	(
	);

	wire wclk;
	clkgen #(.clk_period(4)) xiaofeiclk1(.clk(wclk));


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

	reg wen = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] wdata = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] count = 1;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] w_state = 0;

	always @(posedge wclk) begin
		if(m00_axis_aresetn == 0) begin
			count <= 1;

			w_state <= 0;
		end
		else begin
			wen <= 0;

			case(w_state)
				0: begin
					if(error_full_master == 0) begin
						wen <= 1;
						wdata <= count;

						count <= count + 1;

						w_state <= 1;
					end
					else begin
					end
				end
				1: begin
					w_state <= 2;
				end
				2: begin
					w_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

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

	integer r_state = 0;
	integer rcount = 0;
	reg [C_M00_AXIS_TDATA_WIDTH - 1 : 0] rdata_reg = 0;
	reg rdata_error = 0;

	always @(posedge m00_axis_aclk) begin
		if(m00_axis_aresetn == 0) begin
			r_state <= 0;
			rcount <= 0;
			rdata_reg <= 0;
			rdata_error <= 0;
		end
		else begin
			ren <= 0;
			rdata_error <= 0;

			case(r_state)
				0: begin
					if(r_ready_slave == 1) begin
						ren <= 1;
						r_state <= 1;
						rcount <= 0;
					end
					else begin
					end
				end
				1: begin
					if(ren == 1) begin
						r_state <= 2;
					end
					else begin//error
					end
				end
				2: begin
					if(rdata_reg + 1 == rdata) begin
					end
					else begin
						rdata_error <= 1;
					end
					rdata_reg <= rdata;

					r_state <= 3;
				end
				3: begin
					if((rcount >= 0) && (rcount < 100 - 1)) begin
						rcount <= rcount + 1;
					end
					else begin
						r_state <= 0;
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
