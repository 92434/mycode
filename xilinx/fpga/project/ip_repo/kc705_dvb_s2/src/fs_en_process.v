`timescale 1ns/1ns

module fs_en_process # (
	)
	(
		input wire sys_clk,
		input wire rst_n,

		input wire fs_en2,

		output reg fs_en_on_sys_clk = 0
	);

	//reg delay = 0;
	//always @(posedge fs_en2) begin
	//	if(rst_n == 0) begin
	//		delay <= 0;
	//	end
	//	else begin
	//		delay <= delay + 1;
	//	end
	//end

	//wire fs_en;
	//assign fs_en = ((delay == 1) && (fs_en2 == 1)) ? 1 : 0;

	reg fs_en = 0;
	always @(posedge fs_en2) begin
		if(rst_n == 0) begin
			fs_en <= 0;
		end
		else begin
			fs_en <= ~fs_en;
		end
	end

	wire sys_clk_using;
	assign sys_clk_using = sys_clk;

	wire rdata;
	wire r_enable;
	wire r_ready;
	wire error_full;
	wire error_empty;

	my_fifo # (
			.DATA_WIDTH(1),
			.BULK_OF_DATA(1),
			.BULK_DEPTH(32)
		) fs_en_fifo (
			.rst_n(rst_n),
			.wclk(fs_en2),
			.rclk(sys_clk_using),
			.wdata(0),
			.rdata(rdata),
			.w_enable(fs_en),
			.r_enable(r_enable),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	assign r_enable = r_ready;

	always @(posedge sys_clk_using) begin
		if(rst_n == 0) begin
			fs_en_on_sys_clk <= 0;
		end
		else begin
			if(r_enable == 1) begin
				fs_en_on_sys_clk <= 1;
			end
			else begin
				fs_en_on_sys_clk <= 0;
			end
		end
	end
endmodule
