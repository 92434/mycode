`timescale 1ns/1ns

module receive_data_from_i2s(
	rst,
	bclk,
	lrclk,
	i2s_dout,
//	i2s_din,
	s_data_out,
	s_data_valid
	);

parameter sdsize = 24;

input rst;
input bclk;
input lrclk;
input i2s_dout;
//output i2s_din;
output [sdsize:0] s_data_out;
output s_data_valid;

wire rst;
wire bclk;
wire lrclk;
wire i2s_dout;
//wire i2s_din;
reg [sdsize:0] s_data_out = 0;
reg s_data_valid = 0;

integer count = sdsize;
reg lrstate;

//lock s_data_out at posedge of bclk
always @(negedge bclk or lrclk) begin
	if(rst == 0'b0) begin
		count <= sdsize;
		lrstate <= ~lrclk;
		s_data_valid <= 0;
		s_data_out = 0;
	end
	else begin
		if((lrstate != lrclk) || (lrstate === 1'bX)) begin
			lrstate <= lrclk;
			s_data_out[sdsize] <= lrclk;
			s_data_valid <= 0;
			s_data_out = 0;
			count <= sdsize - 1;
		end
		else begin
			if(count < sdsize) begin
				s_data_out[count] <= i2s_dout;

				if(count == 0) begin
					s_data_valid <= 1;
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
