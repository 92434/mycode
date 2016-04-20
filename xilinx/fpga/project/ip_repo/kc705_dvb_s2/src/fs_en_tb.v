`timescale 1ns / 1ns

module fs_en_tb #() ();

	wire sys_clk;
	wire fs_en2;
	clkgen #(.clk_period(15)) xiaofeiclk1(.clk(sys_clk));
	clkgen #(.clk_period(30)) xiaofeiclk2(.clk(fs_en2));

	reg rst_n = 1;

	initial begin
		#120;
		rst_n = 0;
		#120;
		rst_n = 1;
	end

	wire fs_en_on_sys_clk;
	wire fs_en;
	fs_en_process #(
		) fs_en_process_inst(
			.sys_clk(sys_clk),
			.rst_n(rst_n),

			.fs_en2(fs_en2),

			.fs_en_on_sys_clk(fs_en_on_sys_clk),
			.fs_en(fs_en)
		);
endmodule
