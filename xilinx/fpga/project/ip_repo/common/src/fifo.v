`timescale 1ns / 1ps

module my_fifo #
	(
		parameter integer DATA_WIDTH = 32,
		parameter integer BULK_OF_DATA = 8,
		parameter integer BULK_DEPTH = 64
	)

	(
		input wire rst_n,
		input wire wclk,
		input wire rclk,
		input wire [DATA_WIDTH - 1 : 0] wdata,
		output reg [DATA_WIDTH - 1 : 0] rdata = 0,
		input wire r_enable,
		output wire r_ready,
		output reg error_full = 0,
		output reg error_empty = 0
	);

	integer r_index = 0;
	integer w_index = 0;
	integer BUFFER_SIZE = BULK_OF_DATA * BULK_DEPTH;

	reg [DATA_WIDTH - 1 : 0] buffer[(BULK_OF_DATA * BULK_DEPTH) - 1 : 0];

	assign r_ready = (w_index >= r_index) ?
				((w_index >= r_index + BULK_OF_DATA) ? 1 : 0)
				: ((w_index + BUFFER_SIZE >= r_index + BULK_OF_DATA) ? 1 : 0);

	always @(posedge wclk) begin
		if(rst_n == 0) begin
			w_index <= 0;
			error_full <= 0;
		end
		else begin
			if((w_index + 1 == r_index) || ((r_index == 0) && (w_index == BUFFER_SIZE - 1))) begin
				error_full <= 1;
			end
			else begin
				error_full <= 0;
			end


			buffer[w_index] <= wdata;

			if((w_index >= 0) && ((w_index < BUFFER_SIZE - 1))) begin
				w_index <= w_index + 1;
			end
			else begin
				w_index <= 0;
			end
		end
	end

	always @(posedge rclk) begin
		if(rst_n == 0) begin
			r_index <= 0;
			error_empty <= 0;
		end
		else begin
			if(r_enable == 1) begin
				if((r_index + 1 == w_index) || ((w_index == 0) && (r_index == BUFFER_SIZE - 1))) begin
					error_empty <= 1;
				end
				else begin
					error_empty <= 0;
				end

				rdata <= buffer[r_index];

				if((r_index >= 0) && ((r_index < BUFFER_SIZE - 1))) begin
					r_index <= r_index + 1;
				end
				else begin
					r_index <= 0;
				end
			end
		end
	end

endmodule
