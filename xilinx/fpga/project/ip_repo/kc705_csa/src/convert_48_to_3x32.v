`timescale 1 ns / 1 ps

module convert_48_to_3x32 #
	(
		parameter integer AXI_DATA_WIDTH = 32,
		parameter integer BYTE_WIDTH = 8,
		parameter integer CSA_OUT_DATA_WIDTH_BY_BYTE = 6,
		parameter integer CSA_OUT_OUT_DATA_WIDTH = BYTE_WIDTH * CSA_OUT_DATA_WIDTH_BY_BYTE
	)
	(
		input wire clk,
		input wire rst_n,

		output wire error_full_48,
		input wire wclk_48,
		input wire wen_48,
		input wire [CSA_OUT_OUT_DATA_WIDTH - 1 : 0] wdata_48,

		input wire error_full_32,
		output wire wclk_32,
		output reg wen_32 = 0,
		output reg [AXI_DATA_WIDTH - 1 : 0] wdata_32 = 0
	);

	wire rclk_48;
	reg ren_48 = 0;

	wire [CSA_OUT_OUT_DATA_WIDTH - 1 : 0] rdata_48;
	wire r_ready_48;
	//wire error_full_48;
	wire error_empty_48;

	assign rclk_48 = clk;
	assign wclk_32 = clk;


	my_fifo #(
			.DATA_WIDTH(CSA_OUT_OUT_DATA_WIDTH),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(32)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(wclk_48),
			.rclk(rclk_48),
			.wdata(wdata_48),
			.rdata(rdata_48),
			.w_enable(wen_48),
			.r_enable(ren_48),
			.r_ready(r_ready_48),
			.error_full(error_full_48),
			.error_empty(error_empty_48)
		);

	reg [AXI_DATA_WIDTH - 1 : 0] wdata_32_0 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] wdata_32_1 = 0;
	reg [AXI_DATA_WIDTH - 1 : 0] wdata_32_2 = 0;
	integer data_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			wdata_32_0 <= 0;
			wdata_32_1 <= 0;
			wdata_32_2 <= 0;

			wen_32 <= 0;
			wdata_32 <= 0;

			data_state <= 0;
		end
		else begin
			wen_32 <= 0;
			ren_48 <= 0;

			case(data_state)
				0: begin
					if(error_full_32 == 0) begin

						data_state <= 1;
					end
				end
				1: begin
					if(r_ready_48 == 1) begin
						ren_48 <= 1;

						data_state <= 2;
					end
					else begin
					end
				end
				2: begin
					wdata_32_0[16 - 1 : 0] <= rdata_48[16 * 0 +: 16];
					wdata_32_1[16 - 1 : 0] <= rdata_48[16 * 1 +: 16];
					wdata_32_2[16 - 1 : 0] <= rdata_48[16 * 2 +: 16];

					data_state <= 3;
				end
				3: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_0;

					data_state <= 4;
				end
				4: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_1;

					data_state <= 5;
				end
				5: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_2;

					data_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end
endmodule
