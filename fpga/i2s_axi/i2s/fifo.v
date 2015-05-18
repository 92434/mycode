`timescale 1ns / 1ps

module my_fifo #
	(
		parameter DATA_WIDTH = 32,
		parameter NUMBER_OF_OUTPUT_WORDS = 8
	)

	(
		input wire rst,
		input wire wclk,
		input wire rclk,
		input wire [DATA_WIDTH - 1 : 0] wdata,
		output reg [DATA_WIDTH - 1 : 0] rdata = 0,
		input wire read_enable,
		output wire output_ready,
		output reg buffer_full_error = 0,
		output reg buffer_empty_error = 0
	);

	integer buffer_index_read = 0;
	integer buffer_index_write = 0;
	integer BUFFER_SIZE = NUMBER_OF_OUTPUT_WORDS * 8;
	reg [DATA_WIDTH - 1 : 0] buffer[(NUMBER_OF_OUTPUT_WORDS * 8) - 1 : 0];

	assign output_ready = (buffer_index_write >= buffer_index_read) ?
				((buffer_index_write >= buffer_index_read + NUMBER_OF_OUTPUT_WORDS) ? 1 : 0)
				: ((buffer_index_write + BUFFER_SIZE >= buffer_index_read + NUMBER_OF_OUTPUT_WORDS) ? 1 : 0);

	always @(posedge wclk) begin
		if(rst == 0) begin
			buffer_index_write <= 0;
			buffer_full_error <= 0;
		end
		else begin
			if((buffer_index_write + 1 == buffer_index_read) || ((buffer_index_read == 0) && (buffer_index_write == BUFFER_SIZE - 1))) begin
				if(output_ready == 1) begin
					buffer_full_error <= 1;
				end
				else begin
					buffer_full_error <= 0;
				end
			end
			else begin
				buffer_full_error <= 0;
			end

			buffer[buffer_index_write] <= wdata;
			if((buffer_index_write + 1) >= BUFFER_SIZE) begin
				buffer_index_write <= 0;
			end
			else begin
				buffer_index_write <= buffer_index_write + 1;
			end
		end
	end

	always @(negedge rclk) begin
		if(rst == 0) begin
			buffer_index_read <= 0;
			buffer_empty_error <= 0;
		end
		else begin
			if(read_enable == 1) begin
				if((buffer_index_read + 1 == buffer_index_write) || ((buffer_index_write = 0) && (buffer_index_read == BUFFER_SIZE - 1))) begin
					buffer_empty_error <= 1;
				end
				else begin
					buffer_empty_error <= 0;
				end

				rdata <= buffer[buffer_index_read];
				if((buffer_index_read + 1) >= BUFFER_SIZE) begin
					buffer_index_read <= 0;
				end
				else begin
					buffer_index_read <= buffer_index_read + 1;
				end
			end
		end
	end

endmodule
