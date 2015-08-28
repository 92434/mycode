`timescale 1ns/1ns

module receive_data_from_i2s #
	(
		parameter integer I2S_DATA_BIT_WIDTH = 24
	)
	(
		input wire rst,
		input wire bclk,
		input wire lrclk,
		input wire sdata,
		//output wire i2s_din,
		output reg [I2S_DATA_BIT_WIDTH:0] i2s_received_data = 0,
		output reg s_data_valid = 0
	);



	integer count = I2S_DATA_BIT_WIDTH;
	reg lrstate = 0;

	//lock i2s_received_data at negedge of bclk
	always @(posedge bclk) begin
		if(rst == 0'b0) begin
			lrstate <= lrclk;
			i2s_received_data[I2S_DATA_BIT_WIDTH] <= lrclk;
			s_data_valid <= 0;
			i2s_received_data = 0;
			count <= I2S_DATA_BIT_WIDTH - 1;
		end
		else begin
			if(lrstate != lrclk) begin
				lrstate <= lrclk;
				i2s_received_data[I2S_DATA_BIT_WIDTH] <= lrclk;
				s_data_valid <= 0;
				i2s_received_data = 0;
				count <= I2S_DATA_BIT_WIDTH - 1;
			end
			else begin
				if(count < I2S_DATA_BIT_WIDTH) begin
					i2s_received_data[count] <= sdata;

					if(count == 0) begin
						s_data_valid <= 1;
						count <= I2S_DATA_BIT_WIDTH;
					end
					else begin
						count <= count - 1;
					end	
				end
			end

		end
	end

endmodule
