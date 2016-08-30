`timescale 1ns / 1ns

module tb #() ();

	wire clk;
	wire en;
	clkgen #(.clk_period(15)) xiaofeiclk1(.clk(clk));
	clkgen #(.clk_period(30)) xiaofeiclk2(.clk(en));

	reg rst_n = 1;

	initial begin
		#120;
		rst_n = 0;
		#120;
		rst_n = 1;
	end

	wire clk_en;
	clk_process #(
		) clk_process_inst(
			.clk(clk),
			.rst_n(rst_n),

			.en(en),

			.clk_en(clk_en)
		);


	integer en_count = 0;
	always @(posedge en) begin
		if(rst_n == 0) begin
			en_count <= 0;
		end
		else begin
			en_count <= en_count + 1;
		end
	end

	integer clk_count = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			clk_count <= 0;
		end
		else begin
			clk_count <= clk_count + 1;
		end
	end

	integer clk_en_count = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			clk_en_count <= 0;
		end
		else begin
			if(clk_en == 1) begin
				clk_en_count <= clk_en_count + 1;
			end
			else begin
			end
		end
	end
endmodule
