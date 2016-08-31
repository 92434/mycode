`timescale 1ns/1ns

module clk_process # (
	)
	(
		input wire clk,
		input wire rst_n,

		input wire en,

		output wire clk_en
	);

	localparam integer DELAY = 1;

	integer delay = 0;
	always @(posedge en) begin
		if(rst_n == 0) begin
			delay <= 0;
		end
		else begin
			if(delay < DELAY) begin
				delay <= delay + 1;
			end
			else begin
				delay <= 0;
			end
		end
	end

	wire w_enable;
	assign w_enable = (delay == DELAY) ? 1 : 0;

	wire rdata;
	reg r_enable = 0;
	reg r_enable_reg = 0;
	wire r_ready;
	wire error_full;
	wire error_empty;

	my_fifo # (
			.DATA_WIDTH(1),
			.BULK_OF_DATA(4),
			.BULK_DEPTH(8)
		) fs_en_fifo (
			.rst_n(rst_n),
			.wclk(en),
			.rclk(clk),
			.wdata(0),
			.rdata(rdata),
			.w_enable(w_enable),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	always @(posedge clk) begin
		if(rst_n == 0) begin
			r_enable <= 0;
			r_enable_reg <= 0;
		end
		else begin
			r_enable <= 0;
			r_enable_reg <= r_enable;

			if(r_ready == 1) begin
				r_enable <= 1;
			end
			else begin
			end
		end
	end

	//assign clk_en = (r_enable == 1) ? clk : 0;
	assign clk_en = r_enable_reg;
endmodule
