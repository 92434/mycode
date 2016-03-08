
`timescale 1ns / 1ps
module plframer(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							xfec_frame_vld,
input	          							pl_header_vld,
input	          							pl_pilot_vld,
input	          							xfec_ready,
input	          							null_vld,
input	    signed      [15:0]					xfec_re_in,
input	    signed      [15:0]					xfec_im_in,
input	    signed      [15:0]					pl_header_re_in,
input	    signed      [15:0]					pl_header_im_in,
input	    signed      [15:0]					pl_pilot_re_in,
input	    signed      [15:0]					pl_pilot_im_in,

output 	reg								oe,
output 	reg	signed [15:0]					symbol_re_out,
output 	reg	signed [15:0]					symbol_im_out
);

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_re_out <= 16'sh0000;
		symbol_im_out <= 16'sh0000;
		oe            <= 1'b0;
	end
	else if(fs_en)begin
		if(xfec_ready == 1'b1)begin
			if(xfec_frame_vld == 1'b1)begin
				symbol_re_out <= xfec_re_in;
				symbol_im_out <= xfec_im_in;
				oe            <= 1'b1;
			end
			else if(pl_header_vld == 1'b1)begin
				symbol_re_out <= pl_header_re_in;
				symbol_im_out <= pl_header_im_in;
				oe            <= 1'b1;
			end
			else if(pl_pilot_vld == 1'b1)begin
				symbol_re_out <= pl_pilot_re_in;
				symbol_im_out <= pl_pilot_im_in;
				oe            <= 1'b1;
			end
			else begin
				oe            <= 1'b0;
			end
		end
		else begin
			symbol_re_out <= 16'sh0000;//pl_header_re_in; //modified by wanjin 2014.08.17
			symbol_im_out <= 16'sh0000;//pl_header_im_in; //modified by wanjin 2014.08.17
			oe            <= 1'b0;//1'b1; //modified by wanjin 2014.08.17
			
//			if(pl_header_vld == 1'b1)begin
//				symbol_re_out <= pl_header_re_in; //modified by wanjin 2014.08.17
//				symbol_im_out <= pl_header_im_in; //modified by wanjin 2014.08.17
//				oe            <= 1'b1; //modified by wanjin 2014.08.17
//			end
//			else if(null_vld == 1'b1)begin
//				symbol_re_out <= 16'sh0b50;
//				symbol_im_out <= 16'sh0b50;
//				oe            <= 1'b1;
//			end
//			else begin
//				oe            <= 1'b0;
//			end
		end
	end
	else begin
	end
end


endmodule
