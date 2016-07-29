`timescale 1ns/1ns

module tb #
	()
	();

	reg rst_n = 0;
	initial begin
		rst_n = 0;
		#9
		rst_n = 1;
	end

	wire clk;
	clkgen #(.clk_period(1)) xiaofeiclk_wr(.clk(clk));

	wire ts_clk;
	wire ts_valid;
	wire ts_sync;
	wire [8 - 1 : 0] ts_data;

	ts_generator ts_generator_inst(
			.rst_n(rst_n),
			.clk(clk),

			.ts_clk(ts_clk),
			.ts_valid(ts_valid),
			.ts_sync(ts_sync),
			.ts_data(ts_data)
		);
endmodule
