`timescale 1ns / 1ps
module Parallel_Accum(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							Accum_en,
input	          							reg_clr_vld,
input	          			[7:0] 		Parallel_Accum_in,
//////////////////////////////////////////////////////////////
output  		reg  							Accum_oe,
output      reg   		[7:0] 		Parallel_Accum_out
);

always @(posedge sys_clk)begin
	if(~rst_n)begin
		Accum_oe <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		Accum_oe <= Accum_en;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		Parallel_Accum_out <= 8'b00000000;
	end
	else if(fs_en == 1'b1)begin
		if(reg_clr_vld)begin
			Parallel_Accum_out <= 8'b00000000;
		end
		else if(Accum_en)begin
			Parallel_Accum_out <= {Parallel_Accum_out[0]^Parallel_Accum_in[7],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5]^Parallel_Accum_in[4],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5]^Parallel_Accum_in[4]^Parallel_Accum_in[3],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5]^Parallel_Accum_in[4]^Parallel_Accum_in[3]^Parallel_Accum_in[2],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5]^Parallel_Accum_in[4]^Parallel_Accum_in[3]^Parallel_Accum_in[2]^Parallel_Accum_in[1],Parallel_Accum_out[0]^Parallel_Accum_in[7]^Parallel_Accum_in[6]^Parallel_Accum_in[5]^Parallel_Accum_in[4]^Parallel_Accum_in[3]^Parallel_Accum_in[2]^Parallel_Accum_in[1]^Parallel_Accum_in[0]};
		end
	end
	else begin
	end
end

endmodule
