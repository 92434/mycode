`timescale 1 ns / 1 ps

module convert_48_to_3x32 #
	()
	(
		input wire clk,
		input wire rst_n,

		input wire wclk_48,
		input wire wen_48,
		input wire [48 - 1 : 0] wdata_48,

		output wire wclk_32,
		output reg wen_32 = 0,
		output reg [32 - 1 : 0] wdata_32 = 0
	);


	wire r_enable;
	wire [48 - 1 : 0] rdata_48;

	my_fifo #(
			.DATA_WIDTH(48),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(wclk_48),
			.rclk(clk),
			.wdata(wdata_48),
			.rdata(rdata_48),
			.w_enable(wen_48),
			.r_enable(r_enable),

			.r_ready(r_enable),
			.error_full(),
			.error_empty()
		);

	reg ren_48 = 0;
	always @(negedge clk) begin
		if(rst_n == 0) begin
			ren_48 <= 0;
		end
		else begin
			ren_48 <= r_enable;
		end
	end

	reg [32 - 1 : 0] wdata_32_0 = 0;
	reg [32 - 1 : 0] wdata_32_1 = 0;
	reg [32 - 1 : 0] wdata_32_2 = 0;
	reg start_data_48 = 0;
	reg end_data_48 = 1;

	integer data_48_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			data_48_state <= 0;
			wdata_32_0 <= 0;
			wdata_32_1 <= 0;
			wdata_32_2 <= 0;
			start_data_48 <= 0;
		end
		else begin
			case(data_48_state)
				0: begin
					if(end_data_48 == 1) begin
						if(ren_48 == 1) begin
							wdata_32_0[16 - 1 : 0] <= rdata_48[16 * 1 - 1 -: 16];
							wdata_32_1[16 - 1 : 0] <= rdata_48[16 * 2 - 1 -: 16];
							wdata_32_2[16 - 1 : 0] <= rdata_48[16 * 3 - 1 -: 16];
							start_data_48 <= 1;
							data_48_state <= 1;
						end
						else begin
							data_48_state <= 0;
						end
					end
					else begin
						data_48_state <= 0;
					end
				end
				1: begin
					start_data_48 <= 0;
					data_48_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

	assign wclk_32 = clk;

	integer data_32_state = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			wen_32 <= 0;
			wdata_32 <= 0;
			data_32_state <= 0;
			end_data_48 = 1;
		end
		else begin
			case(data_32_state)
				0: begin
					if(start_data_48 == 1) begin
						end_data_48 <= 0;
						data_32_state <= 1;
					end
					else begin
						data_32_state <= 0;
					end
				end
				1: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_0;
					data_32_state <= 2;
				end
				2: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_1;
					data_32_state <= 3;
				end
				3: begin
					wen_32 <= 1;
					wdata_32 <= wdata_32_2;
					data_32_state <= 4;
				end
				4: begin
					wen_32 <= 0;
					end_data_48 <= 1;
					data_32_state <= 0;
				end
				default: begin
				end
			endcase
		end
	end

endmodule
