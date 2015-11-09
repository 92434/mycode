`timescale 1ns/1ns

module data_gen_counter #
	(
		parameter I2S_SENDER_TEST_DATA_WIDTH = 24
	)
	(
		input wire rst_n,
		input wire clk,
		output reg [I2S_SENDER_TEST_DATA_WIDTH - 1:0] data_source = 0
	);

	reg [I2S_SENDER_TEST_DATA_WIDTH - 1:0] index = 0;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			index <= 2;
			data_source <= 1;
		end
		else begin
			if((index >= 1) && (index < 20)) begin
				data_source <= index;
				index <= index + 1;
			end
			else begin
				index <= 1;
			end
		end
	end
endmodule
