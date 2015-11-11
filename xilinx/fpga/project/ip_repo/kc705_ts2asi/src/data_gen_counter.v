`timescale 1ns/1ns

module data_gen_counter #
	(
		parameter integer DATA_WIDTH = 24
	)
	(
		input wire rst_n,
		input wire clk,
		output reg [DATA_WIDTH - 1:0] data_source = 0
	);

	reg [DATA_WIDTH - 1:0] index = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			data_source <= 0;
		end
		else begin
			data_source <= index;
			index <= index + 1;
		end
	end
endmodule
