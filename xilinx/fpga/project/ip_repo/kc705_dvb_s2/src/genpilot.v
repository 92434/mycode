`timescale 1ns / 1ps
module genpilot(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_vld,
output 				reg						oe,
output 				reg	signed	[15:0]			symbol_re_out,
output 				reg	signed	[15:0]			symbol_im_out
);

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_re_out <= 16'sh0000;
		symbol_im_out <= 16'sh0000;
		oe            <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1'b1)begin
			symbol_re_out <= 16'sh0b50;
			symbol_im_out <= 16'sh0b50;
			oe            <= 1'b1;
		end
		else begin
			symbol_re_out <= 16'sh0000;
			symbol_im_out <= 16'sh0000;
			oe            <= 1'b0;
		end
	end
	else begin
	end
end

endmodule
