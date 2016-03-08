
`timescale 1ns / 1ps
module bchencoder_data_ps(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 			ldpc_mode,
input	          							init_vld,
input	          							frame_vld,
input   			 		[191:0]     	byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe_head,
output  reg  								oe,
output  reg  			   	[7:0]     		byte_out
//////////////////////////////////////////////////////////////
);

reg							[4:0]			BCH_Q_Byte_Num;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Num <= 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		if(frame_mode == 0)begin // 0:normal frame; 1;short frame
			if((ldpc_mode == 5)||(ldpc_mode == 8))begin
				BCH_Q_Byte_Num <= 19;
			end
			else if((ldpc_mode == 9)||(ldpc_mode == 10))begin
				BCH_Q_Byte_Num <= 15;
			end
			else begin
				BCH_Q_Byte_Num <= 23;
			end
		end  
		else begin
			BCH_Q_Byte_Num <= 20;
		end
	end
	else begin
	end
end


reg							[4:0]			BCH_Q_Byte_Cnt_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Cnt_reg <= 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1)begin
			BCH_Q_Byte_Cnt_reg <= 5'b00000;
		end
		else begin
			if(BCH_Q_Byte_Cnt_reg == (BCH_Q_Byte_Num))begin
				BCH_Q_Byte_Cnt_reg <= 5'b00000;
			end
			else begin
				BCH_Q_Byte_Cnt_reg <= BCH_Q_Byte_Cnt_reg + 5'b00001;
			end
		end
	end
	else begin
	end
end


reg											frame_vld_1dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		frame_vld_1dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		frame_vld_1dly_reg <= frame_vld;
	end
	else begin
	end
end

reg		   			 		[191:0]     	byte_in_tmp;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_in_tmp <= 192'h0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld == 1)begin
			byte_in_tmp <= byte_in;
		end
		else begin
			byte_in_tmp <= {byte_in_tmp[183:0],8'b00000000};
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out <= 8'h0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_mode == 0)begin // 0:normal frame; 1;short frame
			if((ldpc_mode == 5)||(ldpc_mode == 8))begin
				byte_out <= byte_in_tmp[159:152];
			end
			else if((ldpc_mode == 9)||(ldpc_mode == 10))begin
				byte_out <= byte_in_tmp[127:120];
			end
			else begin
				byte_out <= byte_in_tmp[191:184];
			end
		end  
		else begin
			byte_out <= byte_in_tmp[167:160];
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= 1'b0;
		oe_head <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld_1dly_reg == 1)&&(BCH_Q_Byte_Cnt_reg == 0))begin
			oe <= 1'b1;
			oe_head <= 1'b1;
		end
		else if((frame_vld_1dly_reg == 0)&&(BCH_Q_Byte_Cnt_reg == 0))begin
			oe <= 1'b0;
			oe_head <= 1'b0;
		end
		else begin
			oe_head <= 1'b0;
		end
	end
	else begin
	end
end

endmodule



