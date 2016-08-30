`timescale 1ns/1ns

module fs_en_process # (
	)
	(
		input wire sys_clk,
		input wire rst_n,

		input wire fs_en2,

		output wire fs_en_on_sys_clk
	);

	localparam integer DELAY = 1;

	integer delay = 0;
	always @(posedge fs_en2) begin
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
	wire r_ready;
	wire error_full;
	wire error_empty;

	my_fifo # (
			.DATA_WIDTH(1),
			.BULK_OF_DATA(4),
			.BULK_DEPTH(8)
		) fs_en_fifo (
			.rst_n(rst_n),
			.wclk(fs_en2),
			.rclk(sys_clk),
			.wdata(0),
			.rdata(rdata),
			.w_enable(w_enable),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	always @(posedge sys_clk) begin
		if(rst_n == 0) begin
			r_enable <= 0;
		end
		else begin
			r_enable <= 0;
			if(r_ready == 1) begin
				r_enable <= 1;
			end
			else begin
			end
		end
	end

	//assign fs_en_on_sys_clk = (r_enable == 1) ? sys_clk : 0;
	assign fs_en_on_sys_clk = (r_enable == 1) ? 1 : 0;
endmodule
