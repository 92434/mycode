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


	wire oe;
	wire oe1;
	wire [15 : 0] re;
	wire [15 : 0] im;
	adi_pattern_gen adi_pattern_gen_inst(
			.rst_n(rst_n),
			.sys_clk(clk),
			
			.oe(oe),
			.oe1(oe1),
			.re(re),
			.im(im)
		);
endmodule
