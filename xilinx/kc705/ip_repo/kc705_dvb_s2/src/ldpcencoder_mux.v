`timescale 1ns / 1ps
module ldpcencoder_mux(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							infor_frame_vld,
input   			 		[7:0]     		infor_byte_in,
input	          							check_frame_vld,
input   			 		[7:0]     		check_byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg  			   	[7:0]     		byte_out
//////////////////////////////////////////////////////////////
);



reg											infor_frame_vld_1dly_reg;
reg							[7:0]			infor_byte_in_1dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		infor_byte_in_1dly_reg <= 8'b00000000;
		infor_frame_vld_1dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		infor_byte_in_1dly_reg <= infor_byte_in;
		infor_frame_vld_1dly_reg <= infor_frame_vld;
	end
	else begin
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= 1'b0;
		byte_out <= 8'b00000000;
	end
	else if(fs_en == 1'b1)begin
		if((infor_frame_vld_1dly_reg == 1))begin///////////////////// delay not align
			oe <= 1'b1;
			byte_out <= infor_byte_in_1dly_reg;
		end
		else if(check_frame_vld == 1)begin
			oe <= 1'b1;
			byte_out <= check_byte_in;
		end
		else begin
			oe <= 1'b0;
			byte_out <= 8'b00000000;
		end
	end
	else begin
	end
end

endmodule



