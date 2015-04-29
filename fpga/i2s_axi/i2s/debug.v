`timescale 1ns/1ns

module data_gen #
	(
		parameter I2S_DATA_BIT_WIDTH = 24,
		parameter DATA_SIZE = 5
	)
	(
		input wire rst,
		input wire clk,
		output reg [I2S_DATA_BIT_WIDTH - 1:0] data_source = 24'h123456
	);
	integer count = 0;

	always @(posedge clk) begin
		if(rst == 0) begin
			count <= 0;
			data_source <= 0;
		end
		else begin
			case(count)
				0:
					data_source = 24'h123456;
				1:
					data_source = 24'h345678;
				2:
					data_source = 24'h567890;
				3:
					data_source = 24'h789012;
				4:
					data_source = 24'h901234;
				default:
					data_source = 0;
			endcase
					

			if(count >= DATA_SIZE - 1) begin
				count <= 0;
			end
			else begin
				count = count + 1;
			end
		end
	end
endmodule
