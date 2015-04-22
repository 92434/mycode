`timescale 1ns/1ns

module receive_data_from_i2s #
	(
		parameter I2S_DATA_BIT_WIDTH = 24
	)
	(
		input wire rst,
		input wire bclk,
		input wire lrclk,
		input wire sd,
		//output wire i2s_din,
		output reg [I2S_DATA_BIT_WIDTH:0] i2s_received_data,
		output reg s_data_valid
	);



	integer count = I2S_DATA_BIT_WIDTH;
	reg lrstate;

	//lock i2s_received_data at posedge of bclk
	always @(negedge bclk or posedge lrclk or negedge lrclk) begin
		if(rst == 0'b0) begin
			count <= I2S_DATA_BIT_WIDTH;
			lrstate <= ~lrclk;
			s_data_valid <= 0;
			i2s_received_data = 0;
		end
		else begin
			if((lrstate != lrclk) || (lrstate === 1'bX)) begin
				lrstate <= lrclk;
				i2s_received_data[I2S_DATA_BIT_WIDTH] <= lrclk;
				s_data_valid <= 0;
				i2s_received_data = 0;
				count <= I2S_DATA_BIT_WIDTH - 1;
			end
			else begin
				if(count < I2S_DATA_BIT_WIDTH) begin
					i2s_received_data[count] <= sd;

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
