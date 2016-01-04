`timescale 1 ns / 1 ps

module convert_32_to_40 #(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer BYTE_WIDTH = 8,
		parameter integer CSA_IN_DATA_WIDTH_BY_BYTE = 5,
		parameter integer OUT_DATA_WIDTH = BYTE_WIDTH * CSA_IN_DATA_WIDTH_BY_BYTE
	)(
		input wire clk,
		input wire rst_n,

		input wire axis_s_r_ready,

		output wire axis_s_rclk,
		output reg axis_s_ren = 0,
		input wire [AXI_DATA_WIDTH - 1 : 0] axis_s_rdata,

		input wire csa_in_rclk,
		input wire csa_in_ren,
		output wire [OUT_DATA_WIDTH - 1 : 0] csa_in_rdata,

		output wire csa_in_r_ready,
		output wire csa_in_error_full,
		output wire csa_in_error_empty
	);

	localparam BUFFER_BIT_WIDTH = BYTE_WIDTH * CSA_IN_DATA_WIDTH_BY_BYTE * 4;

	reg [BUFFER_BIT_WIDTH - 1 : 0] buffer = 0;

	integer r_state = 0;
	integer rindex = 0;
	reg buffer_filled = 0;
	reg csa_in_data_ready = 1;

	assign axis_s_rclk = clk;

	always @(posedge axis_s_rclk) begin
		if(rst_n == 0) begin
			r_state <= 0;
			buffer <= 0;
			rindex <= 0;
			buffer_filled <= 0;
			axis_s_ren <= 0;
		end
		else begin
			buffer_filled <= 0;
			axis_s_ren <= 0;

			if(csa_in_error_full == 0) begin
				case(r_state)
					0: begin
						if(axis_s_r_ready == 1) begin
							axis_s_ren <= 1;

							r_state <= 1;
						end
						else begin
						end

					end
					1: begin
						buffer[(rindex * AXI_DATA_WIDTH) +: AXI_DATA_WIDTH] <= axis_s_rdata;

						if((rindex >= 0) && (rindex < CSA_IN_DATA_WIDTH_BY_BYTE - 1)) begin
							rindex <= rindex + 1;

							r_state <= 0;
						end
						else begin
							rindex <= 0;

							r_state <= 2;
						end
					end
					2: begin
						if(csa_in_data_ready == 1) begin
							buffer_filled <= 1;

							r_state <= 0;
						end
						else begin
						end
					end
				endcase
			end
			else begin
			end
		end
	end

	reg [OUT_DATA_WIDTH - 1 : 0] wdata_0 = 0;
	reg [OUT_DATA_WIDTH - 1 : 0] wdata_1 = 0;
	reg [OUT_DATA_WIDTH - 1 : 0] wdata_2 = 0;
	reg [OUT_DATA_WIDTH - 1 : 0] wdata_3 = 0;

	wire csa_in_wclk;
	reg csa_in_wen = 0;
	reg [OUT_DATA_WIDTH - 1 : 0] csa_in_wdata = 0;

	assign csa_in_wclk = clk;

	integer w_state = 0;
	always @(posedge csa_in_wclk) begin
		if(rst_n == 0) begin
			wdata_0 <= 0;
			wdata_1 <= 0;
			wdata_2 <= 0;
			wdata_3 <= 0;
			w_state <= 0;
			csa_in_wen <= 0;
			csa_in_data_ready <= 1;
		end
		else begin
			csa_in_wen <= 0;
			if(csa_in_error_full == 0) begin
				case(w_state)
					0: begin
						if(buffer_filled == 1) begin
							csa_in_data_ready <= 0;
							wdata_0 <= buffer[(0 * OUT_DATA_WIDTH) +: OUT_DATA_WIDTH];
							wdata_1 <= buffer[(1 * OUT_DATA_WIDTH) +: OUT_DATA_WIDTH];
							wdata_2 <= buffer[(2 * OUT_DATA_WIDTH) +: OUT_DATA_WIDTH];
							wdata_3 <= buffer[(3 * OUT_DATA_WIDTH) +: OUT_DATA_WIDTH];

							w_state <= 1;
						end
						else begin
						end
					end
					1: begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_0;
						
						w_state <= 2;
					end
					2: begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_1;
						
						w_state <= 3;
					end
					3: begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_2;
						
						w_state <= 4;
					end
					4: begin
						csa_in_wen <= 1;
						csa_in_wdata <= wdata_3;
						csa_in_data_ready <= 1;
						
						w_state <= 0;
					end
					default: begin
					end
				endcase
			end
			else begin
			end
		end
	end

	my_fifo #(
			.DATA_WIDTH(8 * CSA_IN_DATA_WIDTH_BY_BYTE),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(8)
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
