`timescale 1 ns / 1 ps

module convert_32_to_40 #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer BYTE_WIDTH = 8,
		parameter integer CSA_IN_DATA_WIDTH_BY_BYTE = 5,
		parameter integer CSA_IN_OUT_DATA_WIDTH = BYTE_WIDTH * CSA_IN_DATA_WIDTH_BY_BYTE
	)(
		input wire clk,
		input wire rst_n,

		input wire axis_s_r_ready,

		output wire axis_s_rclk,
		output reg axis_s_ren = 0,
		input wire [AXI_DATA_WIDTH - 1 : 0] axis_s_rdata,

		input wire csa_in_rclk,
		input wire csa_in_ren,
		output wire [CSA_IN_OUT_DATA_WIDTH - 1 : 0] csa_in_rdata,

		output wire csa_in_r_ready,
		output wire csa_in_error_full,
		output wire csa_in_error_empty
	);

	localparam BUFFER_BIT_WIDTH = BYTE_WIDTH * CSA_IN_DATA_WIDTH_BY_BYTE * 4;

	reg [BUFFER_BIT_WIDTH - 1 : 0] buffer = 0;

	integer state = 0;
	integer rindex = 0;
	wire csa_in_wclk;

	reg [CSA_IN_OUT_DATA_WIDTH - 1 : 0] wdata_0 = 0;
	reg [CSA_IN_OUT_DATA_WIDTH - 1 : 0] wdata_1 = 0;
	reg [CSA_IN_OUT_DATA_WIDTH - 1 : 0] wdata_2 = 0;
	reg [CSA_IN_OUT_DATA_WIDTH - 1 : 0] wdata_3 = 0;

	reg csa_in_wen = 0;
	reg [CSA_IN_OUT_DATA_WIDTH - 1 : 0] csa_in_wdata = 0;

	assign axis_s_rclk = clk;
	assign csa_in_wclk = clk;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			buffer <= 0;
			rindex <= 0;

			axis_s_ren <= 0;

			wdata_0 <= 0;
			wdata_1 <= 0;
			wdata_2 <= 0;
			wdata_3 <= 0;

			csa_in_wen <= 0;
			csa_in_wdata <= 0;

			state <= 0;
		end
		else begin
			axis_s_ren <= 0;
			csa_in_wen <= 0;

			case(state)
				0: begin
					if(axis_s_r_ready == 1) begin
						axis_s_ren <= 1;

						state <= 1;
					end
					else begin
					end

				end
				1: begin
					buffer[(rindex * AXI_DATA_WIDTH) +: AXI_DATA_WIDTH] <= axis_s_rdata;

					if((rindex >= 0) && (rindex < CSA_IN_DATA_WIDTH_BY_BYTE - 1)) begin
						rindex <= rindex + 1;
						axis_s_ren <= 1;

						state <= 1;
					end
					else begin
						rindex <= 0;

						state <= 2;
					end
				end
				2: begin
					wdata_0 <= buffer[(0 * CSA_IN_OUT_DATA_WIDTH) +: CSA_IN_OUT_DATA_WIDTH];
					wdata_1 <= buffer[(1 * CSA_IN_OUT_DATA_WIDTH) +: CSA_IN_OUT_DATA_WIDTH];
					wdata_2 <= buffer[(2 * CSA_IN_OUT_DATA_WIDTH) +: CSA_IN_OUT_DATA_WIDTH];
					wdata_3 <= buffer[(3 * CSA_IN_OUT_DATA_WIDTH) +: CSA_IN_OUT_DATA_WIDTH];

					state <= 3;
				end
				3: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_0;
						
						state <= 4;
					end
					else begin
					end
				end
				4: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_1;
						
						state <= 5;
					end
					else begin
					end
				end
				5: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_2;
						
						state <= 6;
					end
					else begin
					end
				end
				6: begin
					if(csa_in_error_full == 0) begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_3;
						
						state <= 0;
					end
					else begin
					end
				end
				default: begin
				end
			endcase
		end
	end

	my_fifo #(
			.DATA_WIDTH(8 * CSA_IN_DATA_WIDTH_BY_BYTE),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(csa_in_wclk),
			.rclk(csa_in_rclk),
			.wdata(csa_in_wdata),
			.rdata(csa_in_rdata),
			.w_enable(csa_in_wen),
			.r_enable(csa_in_ren),
			.r_ready(csa_in_r_ready),
			.error_full(csa_in_error_full),
			.error_empty(csa_in_error_empty)
		);
endmodule
