
`timescale 1ns / 1ps
module bchencoder_mux(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							infor_frame_vld,
input   			 		[7:0]     		infor_byte_in,
input	          							check_frame_vld,
input   			 		[7:0]     		check_byte_in,
input   			 		[1:0]				BCH_Q_Byte_Num_Flag,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg  			   	[7:0]     		byte_out
//////////////////////////////////////////////////////////////
);



reg											infor_frame_vld_1dly_reg;
reg											infor_frame_vld_2dly_reg;
reg											infor_frame_vld_3dly_reg;
reg											infor_frame_vld_4dly_reg;
reg											infor_frame_vld_5dly_reg;
reg											infor_frame_vld_6dly_reg;
reg											infor_frame_vld_7dly_reg;
reg											infor_frame_vld_8dly_reg;
reg											infor_frame_vld_9dly_reg;
reg											infor_frame_vld_10dly_reg;
reg											infor_frame_vld_11dly_reg;
reg											infor_frame_vld_12dly_reg;
reg											infor_frame_vld_13dly_reg;
reg											infor_frame_vld_14dly_reg;
reg											infor_frame_vld_15dly_reg;
reg											infor_frame_vld_16dly_reg;
reg											infor_frame_vld_17dly_reg;
reg											infor_frame_vld_18dly_reg;
reg											infor_frame_vld_19dly_reg;
reg											infor_frame_vld_20dly_reg;
reg											infor_frame_vld_21dly_reg;
reg											infor_frame_vld_22dly_reg;
reg											infor_frame_vld_23dly_reg;
reg											infor_frame_vld_24dly_reg;
reg											infor_frame_vld_25dly_reg;
reg											infor_frame_vld_26dly_reg;
reg											infor_frame_vld_27dly_reg;
reg											infor_frame_vld_28dly_reg;
reg											infor_frame_vld_29dly_reg;
reg											infor_frame_vld_30dly_reg;
reg											infor_frame_vld_31dly_reg;       

reg							[7:0]			infor_byte_in_1dly_reg;
reg							[7:0]			infor_byte_in_2dly_reg;
reg							[7:0]			infor_byte_in_3dly_reg;
reg							[7:0]			infor_byte_in_4dly_reg;
reg							[7:0]			infor_byte_in_5dly_reg;
reg							[7:0]			infor_byte_in_6dly_reg;
reg							[7:0]			infor_byte_in_7dly_reg;
reg							[7:0]			infor_byte_in_8dly_reg;
reg							[7:0]			infor_byte_in_9dly_reg;
reg							[7:0]			infor_byte_in_10dly_reg;
reg							[7:0]			infor_byte_in_11dly_reg;
reg							[7:0]			infor_byte_in_12dly_reg;
reg							[7:0]			infor_byte_in_13dly_reg;
reg							[7:0]			infor_byte_in_14dly_reg;
reg							[7:0]			infor_byte_in_15dly_reg;
reg							[7:0]			infor_byte_in_16dly_reg;
reg							[7:0]			infor_byte_in_17dly_reg;
reg							[7:0]			infor_byte_in_18dly_reg;
reg							[7:0]			infor_byte_in_19dly_reg;
reg							[7:0]			infor_byte_in_20dly_reg;
reg							[7:0]			infor_byte_in_21dly_reg;
reg							[7:0]			infor_byte_in_22dly_reg;
reg							[7:0]			infor_byte_in_23dly_reg;
reg							[7:0]			infor_byte_in_24dly_reg;
reg							[7:0]			infor_byte_in_25dly_reg;
reg							[7:0]			infor_byte_in_26dly_reg;
reg							[7:0]			infor_byte_in_27dly_reg;
reg							[7:0]			infor_byte_in_28dly_reg;
reg							[7:0]			infor_byte_in_29dly_reg;
reg							[7:0]			infor_byte_in_30dly_reg;
reg							[7:0]			infor_byte_in_31dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		infor_byte_in_1dly_reg <= 8'b00000000;
		infor_byte_in_2dly_reg <= 8'b00000000;
		infor_byte_in_3dly_reg <= 8'b00000000;
		infor_byte_in_4dly_reg <= 8'b00000000;
		infor_byte_in_5dly_reg <= 8'b00000000;
		infor_byte_in_6dly_reg <= 8'b00000000;
		infor_byte_in_7dly_reg <= 8'b00000000;
		infor_byte_in_8dly_reg <= 8'b00000000;
		infor_byte_in_9dly_reg <= 8'b00000000;
		infor_byte_in_10dly_reg <= 8'b00000000;
		infor_byte_in_11dly_reg <= 8'b00000000;
		infor_byte_in_12dly_reg <= 8'b00000000;
		infor_byte_in_13dly_reg <= 8'b00000000;
		infor_byte_in_14dly_reg <= 8'b00000000;
		infor_byte_in_15dly_reg <= 8'b00000000;
		infor_byte_in_16dly_reg <= 8'b00000000;
		infor_byte_in_17dly_reg <= 8'b00000000;
		infor_byte_in_18dly_reg <= 8'b00000000;
		infor_byte_in_19dly_reg <= 8'b00000000;
		infor_byte_in_20dly_reg <= 8'b00000000;
		infor_byte_in_21dly_reg <= 8'b00000000;
		infor_byte_in_22dly_reg <= 8'b00000000;
		infor_byte_in_23dly_reg <= 8'b00000000;
		infor_byte_in_24dly_reg <= 8'b00000000;
		infor_byte_in_25dly_reg <= 8'b00000000;
		infor_byte_in_26dly_reg <= 8'b00000000;
		infor_byte_in_27dly_reg <= 8'b00000000;
		infor_byte_in_28dly_reg <= 8'b00000000;
		infor_byte_in_29dly_reg <= 8'b00000000;
		infor_byte_in_30dly_reg <= 8'b00000000;
		infor_byte_in_31dly_reg <= 8'b00000000;
			
		infor_frame_vld_1dly_reg <= 1'b0;
		infor_frame_vld_2dly_reg <= 1'b0;
		infor_frame_vld_3dly_reg <= 1'b0;
		infor_frame_vld_4dly_reg <= 1'b0;
		infor_frame_vld_5dly_reg <= 1'b0;
		infor_frame_vld_6dly_reg <= 1'b0;
		infor_frame_vld_7dly_reg <= 1'b0;
		infor_frame_vld_8dly_reg <= 1'b0;
		infor_frame_vld_9dly_reg <= 1'b0;
		infor_frame_vld_10dly_reg <= 1'b0;
		infor_frame_vld_11dly_reg <= 1'b0;
		infor_frame_vld_12dly_reg <= 1'b0;
		infor_frame_vld_13dly_reg <= 1'b0;
		infor_frame_vld_14dly_reg <= 1'b0;
		infor_frame_vld_15dly_reg <= 1'b0;
		infor_frame_vld_16dly_reg <= 1'b0;
		infor_frame_vld_17dly_reg <= 1'b0;
		infor_frame_vld_18dly_reg <= 1'b0;
		infor_frame_vld_19dly_reg <= 1'b0;
		infor_frame_vld_20dly_reg <= 1'b0;
		infor_frame_vld_21dly_reg <= 1'b0;
		infor_frame_vld_22dly_reg <= 1'b0;
		infor_frame_vld_23dly_reg <= 1'b0;
		infor_frame_vld_24dly_reg <= 1'b0;
		infor_frame_vld_25dly_reg <= 1'b0;
		infor_frame_vld_26dly_reg <= 1'b0;
		infor_frame_vld_27dly_reg <= 1'b0;
		infor_frame_vld_28dly_reg <= 1'b0;
		infor_frame_vld_29dly_reg <= 1'b0;
		infor_frame_vld_30dly_reg <= 1'b0;
		infor_frame_vld_31dly_reg <= 1'b0;	
	end
	else if(fs_en == 1'b1)begin
		infor_byte_in_1dly_reg  <= infor_byte_in;
		infor_byte_in_2dly_reg  <= infor_byte_in_1dly_reg ;
		infor_byte_in_3dly_reg  <= infor_byte_in_2dly_reg ;
		infor_byte_in_4dly_reg  <= infor_byte_in_3dly_reg ;
		infor_byte_in_5dly_reg  <= infor_byte_in_4dly_reg ;
		infor_byte_in_6dly_reg  <= infor_byte_in_5dly_reg ;
		infor_byte_in_7dly_reg  <= infor_byte_in_6dly_reg ;
		infor_byte_in_8dly_reg  <= infor_byte_in_7dly_reg ;
		infor_byte_in_9dly_reg  <= infor_byte_in_8dly_reg ;
		infor_byte_in_10dly_reg <= infor_byte_in_9dly_reg ;
		infor_byte_in_11dly_reg <= infor_byte_in_10dly_reg;
		infor_byte_in_12dly_reg <= infor_byte_in_11dly_reg;
		infor_byte_in_13dly_reg <= infor_byte_in_12dly_reg;
		infor_byte_in_14dly_reg <= infor_byte_in_13dly_reg;
		infor_byte_in_15dly_reg <= infor_byte_in_14dly_reg;
		infor_byte_in_16dly_reg <= infor_byte_in_15dly_reg;
		infor_byte_in_17dly_reg <= infor_byte_in_16dly_reg;
		infor_byte_in_18dly_reg <= infor_byte_in_17dly_reg;
		infor_byte_in_19dly_reg <= infor_byte_in_18dly_reg;
		infor_byte_in_20dly_reg <= infor_byte_in_19dly_reg;
		infor_byte_in_21dly_reg <= infor_byte_in_20dly_reg;
		infor_byte_in_22dly_reg <= infor_byte_in_21dly_reg;
		infor_byte_in_23dly_reg <= infor_byte_in_22dly_reg;
		infor_byte_in_24dly_reg <= infor_byte_in_23dly_reg;
		infor_byte_in_25dly_reg <= infor_byte_in_24dly_reg;
		infor_byte_in_26dly_reg <= infor_byte_in_25dly_reg;
		infor_byte_in_27dly_reg <= infor_byte_in_26dly_reg;
		infor_byte_in_28dly_reg <= infor_byte_in_27dly_reg;
		infor_byte_in_29dly_reg <= infor_byte_in_28dly_reg;
		infor_byte_in_30dly_reg <= infor_byte_in_29dly_reg;
		infor_byte_in_31dly_reg <= infor_byte_in_30dly_reg;
	                                       
		infor_frame_vld_1dly_reg <= infor_frame_vld ;
		infor_frame_vld_2dly_reg <= infor_frame_vld_1dly_reg ;
		infor_frame_vld_3dly_reg <= infor_frame_vld_2dly_reg ;
		infor_frame_vld_4dly_reg <= infor_frame_vld_3dly_reg ;
		infor_frame_vld_5dly_reg <= infor_frame_vld_4dly_reg ;
		infor_frame_vld_6dly_reg <= infor_frame_vld_5dly_reg ;
		infor_frame_vld_7dly_reg <= infor_frame_vld_6dly_reg ;
		infor_frame_vld_8dly_reg <= infor_frame_vld_7dly_reg ;
		infor_frame_vld_9dly_reg <= infor_frame_vld_8dly_reg ;
		infor_frame_vld_10dly_reg <=infor_frame_vld_9dly_reg ;
		infor_frame_vld_11dly_reg <=infor_frame_vld_10dly_reg;
		infor_frame_vld_12dly_reg <=infor_frame_vld_11dly_reg;
		infor_frame_vld_13dly_reg <=infor_frame_vld_12dly_reg;
		infor_frame_vld_14dly_reg <=infor_frame_vld_13dly_reg;
		infor_frame_vld_15dly_reg <=infor_frame_vld_14dly_reg;
		infor_frame_vld_16dly_reg <=infor_frame_vld_15dly_reg;
		infor_frame_vld_17dly_reg <=infor_frame_vld_16dly_reg;
		infor_frame_vld_18dly_reg <=infor_frame_vld_17dly_reg;
		infor_frame_vld_19dly_reg <=infor_frame_vld_18dly_reg;
		infor_frame_vld_20dly_reg <=infor_frame_vld_19dly_reg;
		infor_frame_vld_21dly_reg <=infor_frame_vld_20dly_reg;
		infor_frame_vld_22dly_reg <=infor_frame_vld_21dly_reg;
		infor_frame_vld_23dly_reg <=infor_frame_vld_22dly_reg;
		infor_frame_vld_24dly_reg <=infor_frame_vld_23dly_reg;
		infor_frame_vld_25dly_reg <=infor_frame_vld_24dly_reg;
		infor_frame_vld_26dly_reg <=infor_frame_vld_25dly_reg;
		infor_frame_vld_27dly_reg <=infor_frame_vld_26dly_reg;
		infor_frame_vld_28dly_reg <=infor_frame_vld_27dly_reg;
		infor_frame_vld_29dly_reg <=infor_frame_vld_28dly_reg;
		infor_frame_vld_30dly_reg <=infor_frame_vld_29dly_reg;
		infor_frame_vld_31dly_reg <=infor_frame_vld_30dly_reg;	
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
		if((infor_frame_vld_28dly_reg == 1)&&(BCH_Q_Byte_Num_Flag == 2'b00))begin///////////////////// 21+1+1+5 delaysalign
			oe <= 1'b1;
			byte_out <= infor_byte_in_28dly_reg;
		end
		else if((infor_frame_vld_27dly_reg == 1)&&(BCH_Q_Byte_Num_Flag == 2'b01))begin///////////////////// 20+1+1+5 delaysalign
			oe <= 1'b1;
			byte_out <= infor_byte_in_27dly_reg;
		end
		else if((infor_frame_vld_23dly_reg == 1)&&(BCH_Q_Byte_Num_Flag == 2'b10))begin///////////////////// 16+1+1+5 delaysalign
			oe <= 1'b1;
			byte_out <= infor_byte_in_23dly_reg;
		end
		else if((infor_frame_vld_31dly_reg == 1)&&(BCH_Q_Byte_Num_Flag == 2'b11))begin///////////////////// 24+1+1+5 delaysalign
			oe <= 1'b1;
			byte_out <= infor_byte_in_31dly_reg;
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



