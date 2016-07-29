
`timescale 1ns / 1ps
module plscrambler(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							init_vld,
input	          							scrambler_vld,
input	          							frame_vld,
input	    signed      [15:0]					re_in,
input	    signed      [15:0]					im_in,

output 	reg								oe,
output 	reg	signed [15:0]					symbol_re_out,
output 	reg	signed [15:0]					symbol_im_out
);

reg	          							scrambler_vld_1dly;
reg	          							frame_vld_1dly;
reg	     signed     [15:0]					re_in_1dly;
reg	     signed     [15:0]					im_in_1dly;

reg	          							scrambler_vld_2dly;
reg	          							frame_vld_2dly;
reg	     signed     [15:0]					re_in_2dly;
reg	     signed     [15:0]					im_in_2dly;

reg				 [17:0]					scramble_regx;
reg				 [17:0]					scramble_regy;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		scramble_regx <= 18'h00000;
		scramble_regy <= 18'h00000;
	end
	else if(fs_en)begin
		if(init_vld == 1'b1) begin
			scramble_regx <= 18'h00001;
			scramble_regy <= 18'h3ffff;
		end
		else if(scrambler_vld_1dly == 1'b1)begin
			scramble_regx <= {scramble_regx[0]^scramble_regx[7],scramble_regx[17:1]};
			scramble_regy <= {scramble_regy[0]^scramble_regy[5]^scramble_regy[7]^scramble_regy[10],scramble_regy[17:1]};
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		scrambler_vld_1dly <= 1'b0;
		frame_vld_1dly <= 1'b0;
		re_in_1dly <= 16'sh0000;
		im_in_1dly <= 16'sh0000;
		
		scrambler_vld_2dly <= 1'b0;
		frame_vld_2dly <= 1'b0;
		re_in_2dly <= 16'sh0000;
		im_in_2dly <= 16'sh0000;
		oe <= 1'b0;
	end
	else if(fs_en)begin
		scrambler_vld_1dly <= scrambler_vld;
		frame_vld_1dly <= frame_vld;
		re_in_1dly <= re_in;
		im_in_1dly <= im_in;
		
		scrambler_vld_2dly <= scrambler_vld_1dly;
		frame_vld_2dly <= frame_vld_1dly;
		re_in_2dly <= re_in_1dly;
		im_in_2dly <= im_in_1dly;
		
		oe <= frame_vld_2dly;
	end
	else begin
	end
end

reg	          							scrambler_tmpx;
reg	          							scrambler_tmpy;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		scrambler_tmpx <= 1'b0;
		scrambler_tmpy <= 1'b0;
	end
	else if(fs_en)begin
		if(scrambler_vld_1dly)begin
			scrambler_tmpx <= scramble_regx[0]^scramble_regy[0];
			scrambler_tmpy <= (scramble_regx[4]^scramble_regx[6]^scramble_regx[15])^(scramble_regy[5]^scramble_regy[6]^scramble_regy[8]^(^scramble_regy[15:9]));
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_re_out <= 16'sh0000;
		symbol_im_out <= 16'sh0000;
	end
	else if(fs_en)begin
		if(scrambler_vld_2dly)begin
			case({scrambler_tmpy,scrambler_tmpx})
			2'b00:begin
				symbol_re_out <= re_in_2dly;
				symbol_im_out <= im_in_2dly;
			end
			2'b01:begin
				symbol_re_out <= $signed(16'hffff^im_in_2dly - 1);
				symbol_im_out <= re_in_2dly;
			end
			2'b10:begin
				symbol_re_out <= $signed(16'hffff^re_in_2dly - 1);
				symbol_im_out <= $signed(16'hffff^im_in_2dly - 1);
			end
			2'b11:begin
				symbol_re_out <= im_in_2dly;
				symbol_im_out <= $signed(16'hffff^re_in_2dly - 1);
			end
			default:begin
				symbol_re_out <= re_in_2dly;
				symbol_im_out <= im_in_2dly;
			end
		endcase
		end
		else begin
			symbol_re_out <= re_in_2dly;
			symbol_im_out <= im_in_2dly;
		end
	end
	else begin
	end
end


endmodule
