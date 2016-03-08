`timescale 1ns / 1ps
module ldpcencoder_data_align(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							init_vld,
input	          							frame_vld,
input   			 		[7:0]     	byte_in,
//////////////////////////////////////////////////////////////
output  reg  								  oe,
output  reg  		  [359:0]     byte_out
//////////////////////////////////////////////////////////////
);

reg							[5:0]			LDPC_360_Byte_Cnt_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		LDPC_360_Byte_Cnt_reg <= 6'b000000;
	end
	//else if(init_vld == 1)begin
	//	LDPC_360_Byte_Cnt_reg <= 6'b000000;
	//end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1)begin
			if(LDPC_360_Byte_Cnt_reg == 6'd44)begin
				LDPC_360_Byte_Cnt_reg <= 6'b000000;
			end
			else begin
				LDPC_360_Byte_Cnt_reg <= LDPC_360_Byte_Cnt_reg + 6'b000001;
			end
		end
	end
	else begin
	end
end


//reg											frame_vld_1dly_reg;
//reg							[7:0]			byte_in_1dly_reg;
//
//always @(posedge clk_1x)begin
//	if(~rst_n)begin
//		byte_in_1dly_reg <= 8'b00000000;
//		frame_vld_1dly_reg <= 1'b0;
//	end
//	else begin
//		byte_in_1dly_reg <= byte_in;
//		frame_vld_1dly_reg <= frame_vld;
//	end
//end

reg  		 				 [359:0]     byte_out_tmp;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out_tmp <= 360'h0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1) begin
			byte_out_tmp <= {byte_out_tmp[351:0],byte_in};
		end
	end
	else begin
	end
end

reg													oe_tmp;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe_tmp <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld == 1)&&(LDPC_360_Byte_Cnt_reg == 6'd44))begin
			oe_tmp <= 1'b1;
		end
		else begin
			oe_tmp <= 1'b0;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= 1'b0;
		byte_out <= 360'h0;
	end
	else if(fs_en == 1'b1)begin
		if(oe_tmp == 1)begin
			oe <= 1'b1;
			byte_out <= byte_out_tmp;
		end
		else begin
			oe <= 1'b0;
		end
	end
	else begin
	end
end


endmodule



