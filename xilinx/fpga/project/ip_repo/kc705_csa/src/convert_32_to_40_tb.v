`timescale 1 ns / 1 ps

module convert_32_to_40_tb #()
	();
	
	wire clk;
	reg rst_n = 0;
	
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));

	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

	reg [32 - 1 : 0] i = 32'h12345678;
	integer count = 0;
	reg wen_32 = 0;
	reg [32 - 1 : 0] wdata_32 = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			i <= 32'h12345678;
			count <= 0;
			wen_32 <= 0;
			wdata_32 <= 0;
		end
		else begin
			wen_32 <= 0;

			if(count == 5) begin
				count <= 0;

				wdata_32 <= i;
				wen_32 <= 1;

				i <= i + 1;
			end
			else begin
				count <= count + 1;
			end

		end
	end

	wire ren_32;
	wire [32 - 1 : 0] rdata_32;
	wire r_ready;
	wire error_full;
	wire error_empty;
	my_fifo #(
			.DATA_WIDTH(32),
			.BULK_OF_DATA(5),
			.BULK_DEPTH(256)
		) my_fifo_inst (
			.rst_n(rst_n),
			.wclk(clk),
			.rclk(clk),
			.wdata(wdata_32),
			.rdata(rdata_32),
			.w_enable(wen_32),
			.r_enable(ren_32),
			.r_ready(r_ready),
			.error_full(error_full),
			.error_empty(error_empty)
		);

	wire wen;
	wire [40 - 1 : 0] wdata;
	convert_32_to_40 convert_32_to_40_inst(
			.clk(clk),
			.rst_n(rst_n),

			.r_ready(r_ready),
			.ren(ren_32),
			.rdata(rdata_32),

			.wen(wen),
			.wdata(wdata)
		);
endmodule
