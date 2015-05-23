`timescale 1ns / 1ps

module clkgen #
	(
		parameter integer clk_period = 5
	)
	
	(
		output reg clk
	);


	initial begin 
		clk <= 0;
		forever begin
			#clk_period;
			clk = ~clk;
		end
	end

endmodule
