
`timescale 1ns / 1ps
module bchencoder_data_align(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 			ldpc_mode,
input	          							init_vld,
input	          							frame_vld,
input   			 		[7:0]     		byte_in,
//////////////////////////////////////////////////////////////
output  reg  								oe,
output  reg	 			   	[191:0]     	byte_out
//////////////////////////////////////////////////////////////
);

reg											frame_vld_1dly_reg;
reg							[7:0]			byte_in_1dly_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_in_1dly_reg <= 8'b00000000;
		frame_vld_1dly_reg <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		byte_in_1dly_reg <= byte_in;
		frame_vld_1dly_reg <= frame_vld;
	end
	else begin
	end
end

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

reg							[4:0]			BCH_Q_Byte_Cnt_init;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Cnt_init <= 5'b00000;
	end
	else if(frame_mode == 0)begin
		case (ldpc_mode)
			4'h0:
				BCH_Q_Byte_Cnt_init <= 5'd16;
			4'h1:
				BCH_Q_Byte_Cnt_init <= 5'd13;
			4'h2:
				BCH_Q_Byte_Cnt_init <= 5'd1;
			4'h3:
				BCH_Q_Byte_Cnt_init <= 5'd7;
			4'h4:
				BCH_Q_Byte_Cnt_init <= 5'd13;
			4'h5:
				BCH_Q_Byte_Cnt_init <= 5'd1;
			4'h6:
				BCH_Q_Byte_Cnt_init <= 5'd22;
			4'h7:
				BCH_Q_Byte_Cnt_init <= 5'd1;
			4'h8:
				BCH_Q_Byte_Cnt_init <= 5'd11;
			4'h9:
				BCH_Q_Byte_Cnt_init <= 5'd1;
			4'ha:
				BCH_Q_Byte_Cnt_init <= 5'd7;
			default:
				BCH_Q_Byte_Cnt_init <= 5'd1;
		endcase
	end
	else begin
		case (ldpc_mode)
			4'h0:
				BCH_Q_Byte_Cnt_init <= 5'd16;
			4'h1:
				BCH_Q_Byte_Cnt_init <= 5'd19;
			4'h2:
				BCH_Q_Byte_Cnt_init <= 5'd10;
			4'h3:
				BCH_Q_Byte_Cnt_init <= 5'd4;
			4'h4:
				BCH_Q_Byte_Cnt_init <= 5'd4;
			4'h5:
				BCH_Q_Byte_Cnt_init <= 5'd16;
			4'h6:
				BCH_Q_Byte_Cnt_init <= 5'd7;
			4'h7:
				BCH_Q_Byte_Cnt_init <= 5'd1;
			4'h8:
				BCH_Q_Byte_Cnt_init <= 5'd16;
			4'h9:
				BCH_Q_Byte_Cnt_init <= 5'd7;
			default:
				BCH_Q_Byte_Cnt_init <= 5'd1;
		endcase
	end
end


reg							[4:0]			BCH_Q_Byte_Cnt_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		BCH_Q_Byte_Cnt_reg <= 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		if(init_vld == 1)begin
			BCH_Q_Byte_Cnt_reg <= BCH_Q_Byte_Cnt_init;
		end
		else if(frame_vld_1dly_reg == 1)begin
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


always @(posedge sys_clk)begin
	if(~rst_n)begin
		byte_out <= 192'h0;
	end
	else if(fs_en == 1'b1)begin
		if(init_vld == 1)begin
			byte_out <= 192'h0;
		end
		else if(frame_vld_1dly_reg == 1) begin
			byte_out <= {byte_out[183:0],byte_in_1dly_reg};
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld_1dly_reg == 1)&&(BCH_Q_Byte_Cnt_reg == 0))begin
			oe <= 1'b1;
		end
		else begin
			oe <= 1'b0;
		end
	end
	else begin
	end
end

endmodule



