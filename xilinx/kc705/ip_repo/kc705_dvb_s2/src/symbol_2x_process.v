`timescale 1ns/1ns

module symbol_2x_process # (
	)
	(
		input wire rst_n,
		input wire sys_clk,

		input wire fs_en2,

		input wire symbol_2x_oe_origin,
		input wire [15 : 0] symbol_2x_re_out_origin,
		input wire [15 : 0] symbol_2x_im_out_origin,

		output wire symbol_2x_oe,
		output wire [15 : 0] symbol_2x_re_out,
		output wire [15 : 0] symbol_2x_im_out
	);

	wire [31 : 0] symbol_2x_out_origin;
	wire [31 : 0] symbol_2x_out_origin_fifo;
	assign symbol_2x_out_origin = {symbol_2x_re_out_origin, symbol_2x_im_out_origin};

	wire r_enable;
	reg r_enable_reg = 0;
	wire r_ready;
	wire error_full;
	wire error_empty;

	my_fifo # (
			.DATA_WIDTH(32),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(32)
		) symbol_out_fifo (
			.rst_n(rst_n),
			.wclk(~sys_clk),
			.rclk(fs_en2),
			.wdata(symbol_2x_out_origin),
			.rdata(symbol_2x_out_origin_fifo),
			.w_enable(symbol_2x_oe_origin),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	assign r_enable = r_ready;

	always @(posedge fs_en2) begin
		if(rst_n == 0) begin
			r_enable_reg <= 0;
		end
		else begin
			r_enable_reg <= r_enable;
		end
	end

	reg [31 : 0] symbol_2x_out_origin_fifo_reg = 0;
	reg symbol_2x_oe_enable = 0;
	always @(posedge fs_en2) begin
		if(rst_n == 0) begin
			symbol_2x_out_origin_fifo_reg <= 0;
		end
		else begin
			symbol_2x_oe_enable <= 0;

			if(r_enable_reg == 1) begin
				symbol_2x_oe_enable <= 1;
				symbol_2x_out_origin_fifo_reg <= symbol_2x_out_origin_fifo;
			end
			else begin
			end
		end
	end

	assign symbol_2x_oe = (symbol_2x_oe_enable == 1) ? fs_en2 : 0;
	assign symbol_2x_re_out = symbol_2x_out_origin_fifo_reg[16 * 1 +: 16];
	assign symbol_2x_im_out = symbol_2x_out_origin_fifo_reg[16 * 0 +: 16];
endmodule
