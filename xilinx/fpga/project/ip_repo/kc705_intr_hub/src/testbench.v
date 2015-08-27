`timescale 1 ns / 1 ps

module myip_testbench #
	(
		parameter integer INT_NUM = 3
	)
	(
	);

	wire clk;
	reg rst;
	reg [INT_NUM - 1 : 0] int_i = 0;
	wire int_o;

	initial begin
		rst = 0;
		#9
		rst = 1;
		
		#5
		int_i[0] = 1;
		#5
		int_i[1] = 1;
		#5
		int_i[2] = 1;
		#5
		int_i[0] = 0;
		#5
		int_i[0] = 1;
	end

	// Add user logic here
	clkgen #(.clk_period(1)) xiaofeiclk(.clk(clk));
	
	intr_hub #
	(
	   .INT_NUM(INT_NUM)
	)
	   hub_inst 
	   (
	       .clk(clk),
	       .rst(rst),
	       .int_i(int_i),
	       .int_o(int_o)
	   );

	// User logic ends

endmodule
