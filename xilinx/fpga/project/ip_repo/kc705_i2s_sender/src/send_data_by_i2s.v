//=============================================================================
//	send_data_by_i2s.v
//	one time send 24 bits data;
//	data_source_clk and read_sync as a signal use to get data from RAM or FIFO.
//=============================================================================
//system clock = 50MHz
`timescale 1ns/1ns

module send_data_by_i2s #
	(
		parameter integer I2S_SENDER_TEST_DATA_WIDTH = 24
	)
	(
		input wire clk,
		input wire rst_n,
		input wire [I2S_SENDER_TEST_DATA_WIDTH - 1 : 0] data_source,
		output reg data_source_clk = 0,
		output reg read_sync = 1'b0,
		output reg bclk = 1'b0,
		output reg lrclk = 0,
		output reg sdata = 0
	);

	reg clk_div1 = 0;
	reg [5:0] bclk_count = 6'b0;
	reg [I2S_SENDER_TEST_DATA_WIDTH - 1 : 0] data_buffer;

	always @(posedge clk) begin
		if(rst_n == 0) begin
			clk_div1 <= 0;
		end
		else begin
			clk_div1 <= ~clk_div1;
		end
	end

	always @(posedge clk_div1) begin
		if(rst_n == 0) begin
			bclk <= 0;
			read_sync <= 0;
		end
		else begin
			bclk <= ~bclk;
			read_sync <= ~read_sync;
		end
	end

	always @(negedge bclk) begin
		if(rst_n == 0) begin
			bclk_count <= 0;
		end
		else begin
			if((bclk_count >= 0) && (bclk_count < 32 - 1)) begin
				bclk_count <= bclk_count + 1;
			end
			else begin
				bclk_count <= 0;
			end
		end
	end

	always @(negedge bclk) begin
		if(rst_n == 0) begin
			lrclk <= 1'b0; // left channel
		end
		else begin
			if(bclk_count == 0) begin
				lrclk <= ~lrclk;
			end
			else begin
			end
		end
	end

	always @(posedge bclk) begin
		if(rst_n == 0) begin
			data_source_clk <= 0;
		end
		else begin
			if(bclk_count == 0) begin
				data_source_clk <= 1;
			end
			else if(bclk_count == 1) begin
				data_source_clk <= 0;
			end
			else begin
			end
		end
	end

	always @(negedge bclk) begin
		if(rst_n == 0) begin
			sdata <= 0;
			data_buffer <= 0;
		end
		else begin
			if(bclk_count == 1) begin
				sdata <= data_source[I2S_SENDER_TEST_DATA_WIDTH - 1]; // shift output
				data_buffer <= {data_source[I2S_SENDER_TEST_DATA_WIDTH - 2 : 0], 1'b0};
			end
			else if((bclk_count >= 2) && (bclk_count <= I2S_SENDER_TEST_DATA_WIDTH)) begin
				sdata <= data_source[I2S_SENDER_TEST_DATA_WIDTH - 1]; // shift output
				data_buffer <= {data_buffer[I2S_SENDER_TEST_DATA_WIDTH - 2 : 0], 1'b0};
			end
			else begin
				sdata <= 0;
			end
		end
	end
endmodule
