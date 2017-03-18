`timescale 1 ns / 1 ps

module ts_s2p #(
	)
	(
		input wire mpeg_clk_in,
		input wire mpeg_valid_in,
		input wire mpeg_sync_in,
		input wire mpeg_data_in,

		output wire mpeg_clk_out,
		output wire mpeg_valid_out,
		output wire mpeg_sync_out,
		output wire [7 : 0] mpeg_data_out
	);

	reg ts_valid = 0;
	reg [8 : 0] ts_data = 0;
	reg [2 : 0] index = 0;
	always @(negedge mpeg_clk_in) begin
		ts_valid <= 0;

		if(mpeg_valid_in == 1) begin
			if(mpeg_sync_in == 1) begin
				ts_data[8] <= 1;
				ts_data[0] <= mpeg_data_in;

				index <= 1;
			end
			else begin
				if(index == 0) begin
					ts_data[8] <= 0;
				end
				else begin
				end

				ts_data[index] <= mpeg_data_in;

				if(index == 3'b111) begin
					ts_valid <= 1;
				end
				else begin
				end

				index <= index + 1;
			end
		end
		else begin
		end
	end

	wire rst_n;
	assign rst_n = 1;

	reg mpeg_clk = 0;
	reg mpeg_valid = 0;
	wire [8 : 0] mpeg_data;

	reg [1 : 0] div_count = 0;
	always @(negedge mpeg_clk_in) begin
		if(div_count == 0) begin
			mpeg_clk <= ~mpeg_clk;
		end
		else begin
		end

		div_count <= div_count + 1;
	end

	assign mpeg_clk_out = mpeg_clk;

	localparam integer PACKAGE_BULK_DEPTH = 188 * 2;

	wire r_ready;
	wire r_enable;
	wire error_full;
	wire error_empty;

	assign r_enable = r_ready;
	my_fifo # (
			.DATA_WIDTH(9),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(PACKAGE_BULK_DEPTH)
		) ts_fifo (
			.rst_n(rst_n),
			.wclk(~mpeg_clk_in),
			.rclk(mpeg_clk_out),
			.wdata(ts_data),
			.rdata(mpeg_data),
			.w_enable(ts_valid),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);
	
	always @(posedge mpeg_clk_out) begin
		mpeg_valid <= 0;
		if(r_enable == 1) begin
			mpeg_valid <= 1;
		end
		else begin
		end
	end

	assign mpeg_valid_out = mpeg_valid;
	assign mpeg_sync_out = ((mpeg_data[8] == 1) && (mpeg_valid == 1)) ? 1 : 0;
	assign mpeg_data_out = mpeg_data[7 : 0];

endmodule
