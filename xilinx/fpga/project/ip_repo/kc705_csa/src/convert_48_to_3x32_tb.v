`timescale 1 ns / 1 ps

module convert_48_3x32_tb #()
	();
	
	wire clk;
	reg rst_n = 0;
	
	clkgen #(.clk_period(1)) xiaofeiclk1(.clk(clk));


	initial begin
		rst_n = 0;
		#2
		rst_n = 1;
	end

	reg [48 - 1 : 0] i = 48'h123456789012;
	reg wen_48 = 0;
	reg [48 - 1 : 0] wdata_48 = 0;
	wire error_full_48;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			i <= 48'h123456789012;
			wdata_48 <= 0;
			wen_48 <= 0;
		end
		else begin
			wen_48 <= 0;

			if(error_full_48 == 0) begin
				wdata_48 <= i;
				wen_48 <= 1;

				i <= i + 1;
			end
			else begin
			end

		end
	end

	reg error_full_32 = 0;
	wire wclk_32;
	wire wen_32;
	wire [32 - 1 : 0] wdata_32;

	convert_48_to_3x32 convert_48_to_3x32_inst(
			.clk(clk),
			.rst_n(rst_n),

			.error_full_48(error_full_48),
			.wclk_48(clk),
			.wen_48(wen_48),
			.wdata_48(wdata_48),

			.error_full_32(error_full_32),
			.wclk_32(wclk_32),
			.wen_32(wen_32),
			.wdata_32(wdata_32)
		);

endmodule
