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

	localparam integer HEADER_BYTE_COUNT = 5;
	localparam integer DATA_WIDTH = 16;
	localparam integer HEADER_WIDTH = DATA_WIDTH * HEADER_BYTE_COUNT;

	reg [HEADER_WIDTH - 1 : 0] header = {16'h0B77, 16'hA1DD, 16'h4240, 16'h2F84, 16'h2B03};

	reg [I2S_SENDER_TEST_DATA_WIDTH - 1:0] index = 0;
	reg [I2S_SENDER_TEST_DATA_WIDTH - 1:0] count = 0;
	always @(posedge clk) begin
		if(rst_n == 0) begin
			index <= 1;
			count <= 0;
			data_source <= {header[HEADER_WIDTH - 1 -: DATA_WIDTH], 8'h00};
		end
		else begin
			if((index > 0) && (index < 5)) begin
				data_source <= {header[(HEADER_WIDTH - (index * DATA_WIDTH)) - 1 -: DATA_WIDTH], 8'h00};
				index <= index + 1;
			end
			else if((index >= 5) && (index < 20)) begin
				data_source <= index;
				index <= index + 1;
			end
			else if(index == 20) begin
				data_source <= count;
				index <= index + 1;
				count <= count + 1;
			end
			else begin
				data_source <= {header[HEADER_WIDTH - 1 -: DATA_WIDTH], 8'h00};
				index <= 1;
			end
		end
	end
endmodule
