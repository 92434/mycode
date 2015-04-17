`timescale 1ns/1ns

module receive_data_from_i2s(
	rst,
	bclk,
	lrclk,
	i2s_dout,
//	i2s_din,
	data,
	data_valid
	);

parameter countsize = 32;
parameter sdsize = 24;

input rst;
input bclk;
input lrclk;
input i2s_dout;
//output i2s_din;
output [sdsize:0] data;
output data_valid;

wire rst;
wire bclk;
wire lrclk;
wire i2s_dout;
//wire i2s_din;
reg [sdsize:0] data = 0;
reg data_valid;

integer count = sdsize;
reg lrstate;

//lock data at posedge of bclk
always @(negedge bclk or lrclk) begin
	if(rst == 0'b0) begin
		count <= sdsize;
		lrstate <= lrclk;
		data_valid <= 0;
		data = 0;
	end
	else begin
		if(lrstate != lrclk) begin
			lrstate <= lrclk;
			data[count] <= lrclk;
			data_valid <= 0;
			data = 0;
			count <= count - 1;
		end
		else begin
			if(count < sdsize) begin
				data[count] <= i2s_dout;

				if(count == 0) begin
					data_valid <= 1;
					count <= sdsize;
				end
				else begin
					count <= count - 1;
				end	
			end
		end

	end
end

endmodule
