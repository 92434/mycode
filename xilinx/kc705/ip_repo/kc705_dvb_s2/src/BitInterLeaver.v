
`timescale 1ps / 1ps
module BitInterLeaver(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          			[1:0]			mod_mode,
input	          							frame_mode,
input	          							except_order,
input	          							frame_vld,
input	          			[7:0]			byte_in,
input	          							bit_itl_read_head,
input	          							bit_itl_read_en,

output	       reg		          			oe,
output	       reg		    [4:0]			symbol_out
);

parameter Delay = 1;
reg							[11:0]  		Byte_Rows;
reg							[11:0]  		Byte_Rows_Out;
reg							[2:0]  			BIT_EACH_SYMBOL;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		Byte_Rows <= #Delay 12'h0;
		Byte_Rows_Out <= #Delay 12'h0;
		BIT_EACH_SYMBOL <= #Delay 3'b0;
	end
	else begin
		case(mod_mode)
		2'b00:begin
			if(frame_mode == 1'b0)begin
				Byte_Rows <= #Delay 2699;
				Byte_Rows_Out <= #Delay 2699;
			end
			else begin
				Byte_Rows <= #Delay 674;
				Byte_Rows_Out <= #Delay 674;
			end
			BIT_EACH_SYMBOL <= #Delay 2;
		end
		2'b01:begin
			if(frame_mode == 1'b0)begin
				Byte_Rows <= #Delay 2699;
				Byte_Rows_Out <= #Delay 2699;
			end
			else begin
				Byte_Rows <= #Delay 674;
				Byte_Rows_Out <= #Delay 674;
			end
			BIT_EACH_SYMBOL <= #Delay 2;
		end
		2'b10:begin
			if(frame_mode == 1'b0)begin
				Byte_Rows <= #Delay 2024;
				Byte_Rows_Out <= #Delay 2024;
			end
			else begin
				Byte_Rows <= #Delay 505;
				Byte_Rows_Out <= #Delay 506;
			end
			BIT_EACH_SYMBOL <= #Delay 3;
		end
		2'b11:begin
			if(frame_mode == 1'b0)begin
				Byte_Rows <= #Delay 1619;
				Byte_Rows_Out <= #Delay 1619;
			end
			else begin
				Byte_Rows <= #Delay 404;
				Byte_Rows_Out <= #Delay 404;
			end
			BIT_EACH_SYMBOL <= #Delay 4;
		end
		endcase
	end
end


reg									 		frame_vld_1dly_reg;
reg			[7:0]				 		byte_in_1dly_reg;
reg									 		frame_vld_2dly_reg;
reg			[7:0]				 		byte_in_2dly_reg;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		frame_vld_1dly_reg <= #Delay 1'b0;
		byte_in_1dly_reg <= #Delay 8'b0;
		frame_vld_2dly_reg <= #Delay 1'b0;
		byte_in_2dly_reg <= #Delay 8'b0;
	end
	else if(fs_en == 1'b1)begin
		frame_vld_1dly_reg <= #Delay frame_vld;
		byte_in_1dly_reg <= #Delay byte_in;
		frame_vld_2dly_reg <= #Delay frame_vld_1dly_reg;
		byte_in_2dly_reg <= #Delay byte_in_1dly_reg;
	end
	else begin
	end
end


reg							[11:0]  		ITL_Bytein_Cnt_reg; // align with frame_vld_1dly_reg


always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_Bytein_Cnt_reg <= #Delay 12'h0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld == 1'b1)&&(frame_vld_1dly_reg == 1'b0))begin
			ITL_Bytein_Cnt_reg <= #Delay 12'h0;
		end
		else begin
			if(frame_vld_1dly_reg == 1'b1)begin
				if((mod_mode == 2'b10)&&(frame_mode == 1'b1)&&(frame_vld == 1'b0))begin
				end
				else begin
					if(ITL_Bytein_Cnt_reg == Byte_Rows)begin
						ITL_Bytein_Cnt_reg <= #Delay 12'h0;
					end
					else begin
						ITL_Bytein_Cnt_reg <= #Delay ITL_Bytein_Cnt_reg + 12'h1;
					end
				end
			end
		end
	end
	else begin
	end
end

reg							[2:0]  			ITL_Bytein_Idx_reg;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_Bytein_Idx_reg <= #Delay 3'h0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld == 1'b1)&&(frame_vld_1dly_reg == 1'b0))begin
			ITL_Bytein_Idx_reg <= #Delay 3'h0;
		end
		else begin
			if((frame_vld_1dly_reg == 1)&&(ITL_Bytein_Cnt_reg == Byte_Rows))begin
				if(ITL_Bytein_Idx_reg == BIT_EACH_SYMBOL)begin
					ITL_Bytein_Idx_reg <= #Delay 3'h0;
				end
				else begin
					ITL_Bytein_Idx_reg <= #Delay ITL_Bytein_Idx_reg + 1;
				end
			end
		end
	end
	else begin
	end
end


reg								  			in_pingpong_flag_reg;// align with frame_vld_1dly_reg


always @(posedge sys_clk)begin
	if(~rst_n)begin
		in_pingpong_flag_reg <= #Delay 1'h0;
	end
	else if(fs_en == 1'b1)begin
		if((frame_vld_1dly_reg == 1)&&(ITL_Bytein_Idx_reg == BIT_EACH_SYMBOL)&&(ITL_Bytein_Cnt_reg == Byte_Rows))begin
			in_pingpong_flag_reg <= #Delay ~in_pingpong_flag_reg;
		end
	end
	else begin
	end
end


reg								  			in_pingpong_flag_1dly_reg;// align with frame_vld_2dly_reg


always @(posedge sys_clk)begin
	if(~rst_n)begin
		in_pingpong_flag_1dly_reg <= #Delay 1'h0;
	end
	else if(fs_en == 1'b1)begin
		in_pingpong_flag_1dly_reg <= #Delay in_pingpong_flag_reg;
	end
	else begin
	end
end

reg							[7:0]  			buff0_data;// align with frame_vld_2dly_reg
reg							[11:0]  		buff0_rdaddress;
reg							[11:0]  		buff0_wraddress;// align with frame_vld_2dly_reg

reg							[7:0]  			buff1_data;// align with frame_vld_2dly_reg
reg							[11:0]  		buff1_rdaddress;
reg							[11:0]  		buff1_wraddress;// align with frame_vld_2dly_reg

reg							[7:0]  			buff2_data;// align with frame_vld_2dly_reg
reg							[11:0]  		buff2_rdaddress;
reg							[11:0]  		buff2_wraddress;// align with frame_vld_2dly_reg

reg							[7:0]  			buff3_data;// align with frame_vld_2dly_reg
reg							[11:0]  		buff3_rdaddress;
reg							[11:0]  		buff3_wraddress;// align with frame_vld_2dly_reg

reg							[7:0]  			buff4_data;// align with frame_vld_2dly_reg
reg							[11:0]  		buff4_rdaddress;
reg							[11:0]  		buff4_wraddress;// align with frame_vld_2dly_reg

reg											buff0_wren;// align with frame_vld_2dly_reg
reg											buff1_wren;// align with frame_vld_2dly_reg
reg											buff2_wren;// align with frame_vld_2dly_reg
reg											buff3_wren;// align with frame_vld_2dly_reg
reg											buff4_wren;// align with frame_vld_2dly_reg


always @(posedge sys_clk)begin
	if(~rst_n)begin
		buff0_data <= #Delay 8'h0;
		buff0_wraddress <= #Delay 12'h0;
		buff0_wren <= #Delay 1'b0;
		
		buff1_data <= #Delay 8'h0;
		buff1_wraddress <= #Delay 12'h0;
		buff1_wren <= #Delay 1'b0;
		
		buff2_data <= #Delay 8'h0;
		buff2_wraddress <= #Delay 12'h0;
		buff2_wren <= #Delay 1'b0;
		
		buff3_data <= #Delay 8'h0;
		buff3_wraddress <= #Delay 12'h0;
		buff3_wren <= #Delay 1'b0;
		
		buff4_data <= #Delay 8'h0;
		buff4_wraddress <= #Delay 12'h0;
		buff4_wren <= #Delay 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if(frame_vld_1dly_reg == 1'b1) begin
			case(ITL_Bytein_Idx_reg)
			3'b000:begin
				buff0_data <= #Delay byte_in_1dly_reg;
				buff0_wraddress <= #Delay ITL_Bytein_Cnt_reg;
				buff0_wren <= #Delay frame_vld_1dly_reg;
				
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
				
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
				if((mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
					if(ITL_Bytein_Cnt_reg == 0)begin
						buff3_data <= #Delay {byte_in_1dly_reg[1:0],6'b000000};
						buff3_wraddress <= #Delay Byte_Rows + 1;
						buff3_wren <= #Delay frame_vld_1dly_reg;
					end
					else begin
						buff3_data <= #Delay 8'h0;
						buff3_wraddress <= #Delay 12'h0;
						buff3_wren <= #Delay 1'b0;
					end
				end
				
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end	
			3'b001:begin
				if((mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
					if(ITL_Bytein_Cnt_reg == 0)begin
						buff0_data <= #Delay {byte_in_1dly_reg[7:6],6'b000000};
						buff0_wraddress <= #Delay Byte_Rows + 1;
						buff0_wren <= #Delay frame_vld_1dly_reg;
					end
					else begin
						buff0_data <= #Delay 8'h0;
						buff0_wraddress <= #Delay 12'h0;
						buff0_wren <= #Delay 1'b0;
					end
					buff1_data <= #Delay {byte_in_1dly_reg[5:0],byte_in[7:6]};
					buff1_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff1_wren <= #Delay frame_vld_1dly_reg;
				end
				else begin
					buff0_data <= #Delay 8'h0;
					buff0_wraddress <= #Delay 12'h0;
					buff0_wren <= #Delay 1'b0;
					
					buff1_data <= #Delay {byte_in_1dly_reg};
					buff1_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff1_wren <= #Delay frame_vld_1dly_reg;
				end
				
					
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
					
				buff3_data <= #Delay 8'h0;
				buff3_wraddress <= #Delay 12'h0;
				buff3_wren <= #Delay 1'b0;
					
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
			3'b010:begin
				if((mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
					if(ITL_Bytein_Cnt_reg == 0)begin
						buff1_data <= #Delay {byte_in_1dly_reg[5:4],6'b000000};
						buff1_wraddress <= #Delay Byte_Rows + 1;
						buff1_wren <= #Delay frame_vld_1dly_reg;
					end
					else begin
						buff1_data <= #Delay 8'h0;
						buff1_wraddress <= #Delay 12'h0;
						buff1_wren <= #Delay 1'b0;
					end
					
					buff2_data <= #Delay {byte_in_1dly_reg[3:0],byte_in[7:4]};
					buff2_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff2_wren <= #Delay frame_vld_1dly_reg;
				end
				else begin
					buff1_data <= #Delay 8'h0;
					buff1_wraddress <= #Delay 12'h0;
					buff1_wren <= #Delay 1'b0;
					
					buff2_data <= #Delay {byte_in_1dly_reg};
					buff2_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff2_wren <= #Delay frame_vld_1dly_reg;
				end
				
					
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
					
				buff3_data <= #Delay 8'h0;
				buff3_wraddress <= #Delay 12'h0;
				buff3_wren <= #Delay 1'b0;
					
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
			3'b011:begin
				if((mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
					if(ITL_Bytein_Cnt_reg == 0)begin
						buff2_data <= #Delay {byte_in_1dly_reg[3:2],6'b000000};
						buff2_wraddress <= #Delay Byte_Rows + 1;
						buff2_wren <= #Delay frame_vld_1dly_reg;
					end
					else begin
						buff2_data <= #Delay 8'h0;
						buff2_wraddress <= #Delay 12'h0;
						buff2_wren <= #Delay 1'b0;
					end
					
					buff3_data <= #Delay {byte_in_1dly_reg[1:0],byte_in[7:2]};
					buff3_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff3_wren <= #Delay frame_vld_1dly_reg;
				end
				else begin
					buff2_data <= #Delay 8'h0;
					buff2_wraddress <= #Delay 12'h0;
					buff2_wren <= #Delay 1'b0;
					
					buff3_data <= #Delay {byte_in_1dly_reg};
					buff3_wraddress <= #Delay ITL_Bytein_Cnt_reg;
					buff3_wren <= #Delay frame_vld_1dly_reg;
				end
				
					
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
					
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
					
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
			3'b100:begin
				buff4_data <= #Delay byte_in_1dly_reg;
				buff4_wraddress <= #Delay ITL_Bytein_Cnt_reg;
				buff4_wren <= #Delay frame_vld_1dly_reg;
				
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
				
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
				
				buff3_data <= #Delay 8'h0;
				buff3_wraddress <= #Delay 12'h0;
				buff3_wren <= #Delay 1'b0;
				
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
			end	
			default:begin
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
				
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
				
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
				
				buff3_data <= #Delay 8'h0;
				buff3_wraddress <= #Delay 12'h0;
				buff3_wren <= #Delay 1'b0;
				
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
			endcase
		end
		else begin
			if((mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
				
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
				
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
				
				buff3_data <= #Delay {byte_in_2dly_reg[1:0],6'b000000};
				buff3_wraddress <= #Delay Byte_Rows + 1;//ITL_Bytein_Cnt_reg;
				buff3_wren <= #Delay frame_vld_2dly_reg;
				
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
			else begin
				buff0_data <= #Delay 8'h0;
				buff0_wraddress <= #Delay 12'h0;
				buff0_wren <= #Delay 1'b0;
				
				buff1_data <= #Delay 8'h0;
				buff1_wraddress <= #Delay 12'h0;
				buff1_wren <= #Delay 1'b0;
				
				buff2_data <= #Delay 8'h0;
				buff2_wraddress <= #Delay 12'h0;
				buff2_wren <= #Delay 1'b0;
				
				buff3_data <= #Delay 8'h0;
				buff3_wraddress <= #Delay 12'h0;
				buff3_wren <= #Delay 1'b0;
				
				buff4_data <= #Delay 8'h0;
				buff4_wraddress <= #Delay 12'h0;
				buff4_wren <= #Delay 1'b0;
			end
		end
	end
	else begin
	end
end

reg											out_pingpong_flag_reg;// align with bit_itl_read_head_1dly

always @(posedge sys_clk)begin
	if(~rst_n)begin
		out_pingpong_flag_reg <= #Delay 1'b0;
	end
	else if(fs_en == 1'b1)begin
		if(bit_itl_read_head == 1'b1) begin
			out_pingpong_flag_reg <= #Delay ~out_pingpong_flag_reg;
		end
	end
	else begin
	end
end

reg							[7:0]  			buff0_q;
reg							[7:0]  			buff1_q;
reg							[7:0]  			buff2_q;
reg							[7:0]  			buff3_q;
reg							[7:0]  			buff4_q;

reg											buff0_rden;
reg											buff1_rden;
reg											buff2_rden;
reg											buff3_rden;
reg											buff4_rden;


reg							[1:0]			ITL_QPSK_Bit_Cnt_reg;// align with bit_itl_read_en

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_QPSK_Bit_Cnt_reg <= #Delay 2'b00;
	end
	else if(fs_en == 1'b1)begin
		if(bit_itl_read_en == 1'b1)begin
			if(ITL_QPSK_Bit_Cnt_reg == 2'b11)begin
				ITL_QPSK_Bit_Cnt_reg <= #Delay 2'b00;
			end
			else begin
				ITL_QPSK_Bit_Cnt_reg <= #Delay ITL_QPSK_Bit_Cnt_reg + 2'b01;
			end
		end
	end
	else begin
	end
end

reg							[11:0]			ITL_QPSK_Byte_Cnt_reg;// align with bit_itl_read_en

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_QPSK_Byte_Cnt_reg <= #Delay 12'h000;
	end
	else if(fs_en == 1'b1)begin
		if((bit_itl_read_en == 1'b1)&&(ITL_QPSK_Bit_Cnt_reg == 2'b11))begin
			if(ITL_QPSK_Byte_Cnt_reg == Byte_Rows)begin
				ITL_QPSK_Byte_Cnt_reg <= #Delay 12'h000;
			end
			else begin
				ITL_QPSK_Byte_Cnt_reg <= #Delay ITL_QPSK_Byte_Cnt_reg + 12'h001;
			end
		end
	end
	else begin
	end
end


reg							[1:0]			ITL_QPSK_Buff_Idx_reg;// align with bit_itl_read_en

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_QPSK_Buff_Idx_reg <= #Delay 2'h0;
	end
	else if(fs_en == 1'b1)begin
		if((bit_itl_read_en == 1'b1)&&(ITL_QPSK_Bit_Cnt_reg == 2'b11)&&(ITL_QPSK_Byte_Cnt_reg == Byte_Rows))begin
			if(ITL_QPSK_Buff_Idx_reg == 2'b10)begin
				ITL_QPSK_Buff_Idx_reg <= #Delay 2'h0;
			end
			else begin
				ITL_QPSK_Buff_Idx_reg <= #Delay ITL_QPSK_Buff_Idx_reg + 2'h1;
			end
		end
	end
	else begin
	end
end

reg							[2:0]			ITL_8mux1_Cnt_reg;
reg							[11:0]			ITL_Symbolout_Cnt_reg;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_8mux1_Cnt_reg <= #Delay 3'b000;
	end
	else if(fs_en == 1'b1)begin
		if(bit_itl_read_head == 1'b1)begin
			ITL_8mux1_Cnt_reg <= #Delay 3'b001;
		end
		else if(bit_itl_read_en == 1'b1)begin
			if(ITL_8mux1_Cnt_reg == 3'b111)begin
				ITL_8mux1_Cnt_reg <= #Delay 3'b000;
			end
			else begin
				ITL_8mux1_Cnt_reg <= #Delay ITL_8mux1_Cnt_reg + 3'b001;
			end
		end
		else if((ITL_Symbolout_Cnt_reg == Byte_Rows_Out)&&(mod_mode == 2'b10)&&(frame_mode == 1'b1))begin
			ITL_8mux1_Cnt_reg <= #Delay 3'b000;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ITL_Symbolout_Cnt_reg <= #Delay 12'h000;
	end
	else if(fs_en == 1'b1)begin
		if((bit_itl_read_en == 1)&&((ITL_8mux1_Cnt_reg == 3'b111)||((ITL_Symbolout_Cnt_reg == Byte_Rows_Out)&&(mod_mode == 2'b10)&&(frame_mode == 1'b1)&&(ITL_8mux1_Cnt_reg == 3'b001))))begin
			if(ITL_Symbolout_Cnt_reg == Byte_Rows_Out)begin
				ITL_Symbolout_Cnt_reg <= #Delay 12'h000;
			end
			else begin
				ITL_Symbolout_Cnt_reg <= #Delay ITL_Symbolout_Cnt_reg + 12'h001;
			end
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		buff0_rden <= #Delay 1'b0;
		buff1_rden <= #Delay 1'b0;
		buff2_rden <= #Delay 1'b0;
		buff3_rden <= #Delay 1'b0;
		buff4_rden <= #Delay 1'b0;
		
		buff0_rdaddress <= #Delay 12'h0;
		buff1_rdaddress <= #Delay 12'h0;
		buff2_rdaddress <= #Delay 12'h0;
		buff3_rdaddress <= #Delay 12'h0;
		buff4_rdaddress <= #Delay 12'h0;
	end
	else if(fs_en == 1'b1)begin
		if(bit_itl_read_en == 1'b1)begin
			case(mod_mode)
			2'b00:begin
				if(ITL_QPSK_Buff_Idx_reg == 0)begin
					buff0_rden <= #Delay bit_itl_read_en;// align with bit_itl_read_head_1dly
					buff1_rden <= #Delay 1'b0;
					buff2_rden <= #Delay 1'b0;
					buff0_rdaddress <= #Delay ITL_QPSK_Byte_Cnt_reg;// align with bit_itl_read_head_1dly
					buff1_rdaddress <= #Delay 12'h0;
					buff2_rdaddress <= #Delay 12'h0;
				end
				else if(ITL_QPSK_Buff_Idx_reg == 1)begin
					buff0_rden <= #Delay 1'b0;
					buff1_rden <= #Delay bit_itl_read_en;
					buff2_rden <= #Delay 1'b0;
					buff0_rdaddress <= #Delay 12'h0;
					buff1_rdaddress <= #Delay ITL_QPSK_Byte_Cnt_reg;
					buff2_rdaddress <= #Delay 12'h0;
				end
				else if(ITL_QPSK_Buff_Idx_reg == 2)begin
					buff0_rden <= #Delay 1'b0;
					buff1_rden <= #Delay 1'b0;
					buff2_rden <= #Delay bit_itl_read_en;
					buff0_rdaddress <= #Delay 12'h0;
					buff1_rdaddress <= #Delay 12'h0;
					buff2_rdaddress <= #Delay ITL_QPSK_Byte_Cnt_reg;
				end
			end
			2'b01:begin
				buff0_rden <= #Delay bit_itl_read_en;
				buff1_rden <= #Delay bit_itl_read_en;
				buff2_rden <= #Delay bit_itl_read_en;
				
				buff0_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff1_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff2_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
			end
			2'b10:begin
				buff0_rden <= #Delay bit_itl_read_en;
				buff1_rden <= #Delay bit_itl_read_en;
				buff2_rden <= #Delay bit_itl_read_en;
				buff3_rden <= #Delay bit_itl_read_en;
				
				buff0_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff1_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff2_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff3_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
			end
			2'b11:begin
				buff0_rden <= #Delay bit_itl_read_en;
				buff1_rden <= #Delay bit_itl_read_en;
				buff2_rden <= #Delay bit_itl_read_en;
				buff3_rden <= #Delay bit_itl_read_en;
				buff4_rden <= #Delay bit_itl_read_en;
				
				buff0_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff1_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff2_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff3_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
				buff4_rdaddress <= #Delay ITL_Symbolout_Cnt_reg;
			end
			default:begin
				buff0_rden <= #Delay 1'b0;
				buff1_rden <= #Delay 1'b0;
				buff2_rden <= #Delay 1'b0;
				buff3_rden <= #Delay 1'b0;
				buff4_rden <= #Delay 1'b0;
				
				buff0_rdaddress <= #Delay 12'h0;
				buff1_rdaddress <= #Delay 12'h0;
				buff2_rdaddress <= #Delay 12'h0;
				buff3_rdaddress <= #Delay 12'h0;
				buff4_rdaddress <= #Delay 12'h0;
			end
			endcase
		end
	end
	else begin
	end
end

reg											bit_itl_read_en_1dly;
reg											bit_itl_read_en_2dly;
reg											bit_itl_read_en_3dly;
reg											bit_itl_read_en_4dly;
reg											bit_itl_read_en_5dly;
reg											bit_itl_read_en_6dly;

reg							[1:0]			ITL_QPSK_Buff_Idx_reg_1dly;
reg							[1:0]			ITL_QPSK_Bit_Cnt_reg_1dly;
reg							[2:0]			ITL_8mux1_Cnt_reg_1dly;

reg							[1:0]			ITL_QPSK_Buff_Idx_reg_2dly;
reg							[1:0]			ITL_QPSK_Bit_Cnt_reg_2dly;
reg							[2:0]			ITL_8mux1_Cnt_reg_2dly;

reg							[1:0]			ITL_QPSK_Buff_Idx_reg_3dly;
reg							[1:0]			ITL_QPSK_Bit_Cnt_reg_3dly;
reg							[2:0]			ITL_8mux1_Cnt_reg_3dly;

reg							[1:0]			ITL_QPSK_Buff_Idx_reg_4dly;
reg							[1:0]			ITL_QPSK_Bit_Cnt_reg_4dly;
reg							[2:0]			ITL_8mux1_Cnt_reg_4dly;

reg							[1:0]			ITL_QPSK_Buff_Idx_reg_5dly;
reg							[1:0]			ITL_QPSK_Bit_Cnt_reg_5dly;
reg							[2:0]			ITL_8mux1_Cnt_reg_5dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bit_itl_read_en_1dly <= #Delay 1'b0;
		bit_itl_read_en_2dly <= #Delay 1'b0;
		bit_itl_read_en_3dly <= #Delay 1'b0;
		
		bit_itl_read_en_4dly <= #Delay 1'b0;
		bit_itl_read_en_5dly <= #Delay 1'b0;
		bit_itl_read_en_6dly <= #Delay 1'b0;
		
		ITL_QPSK_Buff_Idx_reg_1dly <= #Delay 2'b00;
		ITL_QPSK_Buff_Idx_reg_2dly <= #Delay 2'b00;
		
		ITL_QPSK_Bit_Cnt_reg_1dly <= #Delay 2'b00;
		ITL_QPSK_Bit_Cnt_reg_2dly <= #Delay 2'b00;
		
		ITL_8mux1_Cnt_reg_1dly <= #Delay 3'b000;
		ITL_8mux1_Cnt_reg_2dly <= #Delay 3'b000;
		
		ITL_QPSK_Buff_Idx_reg_3dly <= #Delay 2'b00;
		ITL_QPSK_Buff_Idx_reg_4dly <= #Delay 2'b00;
		
		ITL_QPSK_Bit_Cnt_reg_3dly <= #Delay 2'b00;
		ITL_QPSK_Bit_Cnt_reg_4dly <= #Delay 2'b00;
		
		ITL_8mux1_Cnt_reg_3dly <= #Delay 3'b000;
		ITL_8mux1_Cnt_reg_4dly <= #Delay 3'b000;
		
		ITL_QPSK_Buff_Idx_reg_5dly <= #Delay 2'b00;
		
		ITL_QPSK_Bit_Cnt_reg_5dly <= #Delay 2'b00;
		
		ITL_8mux1_Cnt_reg_5dly <= #Delay 3'b000;
	end
	else if(fs_en == 1'b1)begin
		bit_itl_read_en_1dly <= #Delay bit_itl_read_en;
		bit_itl_read_en_2dly <= #Delay bit_itl_read_en_1dly;
		bit_itl_read_en_3dly <= #Delay bit_itl_read_en_2dly;
		
		bit_itl_read_en_4dly <= #Delay bit_itl_read_en_3dly;
		bit_itl_read_en_5dly <= #Delay bit_itl_read_en_4dly;
		bit_itl_read_en_6dly <= #Delay bit_itl_read_en_5dly;
		
		ITL_QPSK_Buff_Idx_reg_1dly <= #Delay ITL_QPSK_Buff_Idx_reg;
		ITL_QPSK_Buff_Idx_reg_2dly <= #Delay ITL_QPSK_Buff_Idx_reg_1dly;
		ITL_QPSK_Buff_Idx_reg_3dly <= #Delay ITL_QPSK_Buff_Idx_reg_2dly;
		ITL_QPSK_Buff_Idx_reg_4dly <= #Delay ITL_QPSK_Buff_Idx_reg_3dly;
		ITL_QPSK_Buff_Idx_reg_5dly <= #Delay ITL_QPSK_Buff_Idx_reg_4dly;
		
		ITL_QPSK_Bit_Cnt_reg_1dly <= #Delay ITL_QPSK_Bit_Cnt_reg;
		ITL_QPSK_Bit_Cnt_reg_2dly <= #Delay ITL_QPSK_Bit_Cnt_reg_1dly;
		ITL_QPSK_Bit_Cnt_reg_3dly <= #Delay ITL_QPSK_Bit_Cnt_reg_2dly;
		ITL_QPSK_Bit_Cnt_reg_4dly <= #Delay ITL_QPSK_Bit_Cnt_reg_3dly;
		ITL_QPSK_Bit_Cnt_reg_5dly <= #Delay ITL_QPSK_Bit_Cnt_reg_4dly;
		
		ITL_8mux1_Cnt_reg_1dly <= #Delay ITL_8mux1_Cnt_reg;
		ITL_8mux1_Cnt_reg_2dly <= #Delay ITL_8mux1_Cnt_reg_1dly;
		ITL_8mux1_Cnt_reg_3dly <= #Delay ITL_8mux1_Cnt_reg_2dly;
		ITL_8mux1_Cnt_reg_4dly <= #Delay ITL_8mux1_Cnt_reg_3dly;
		ITL_8mux1_Cnt_reg_5dly <= #Delay ITL_8mux1_Cnt_reg_4dly;
	end
	else begin
	end
end


reg							[4:0]				symbol_out_wo_qpsk;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_out_wo_qpsk <= #Delay 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		case(ITL_8mux1_Cnt_reg_5dly)
		3'b000:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[7],buff1_q[7],buff2_q[7],buff3_q[7],buff4_q[7]};
		end
		3'b001:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[6],buff1_q[6],buff2_q[6],buff3_q[6],buff4_q[6]};
		end
		3'b010:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[5],buff1_q[5],buff2_q[5],buff3_q[5],buff4_q[5]};
		end
		3'b011:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[4],buff1_q[4],buff2_q[4],buff3_q[4],buff4_q[4]};
		end
		3'b100:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[3],buff1_q[3],buff2_q[3],buff3_q[3],buff4_q[3]};
		end
		3'b101:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[2],buff1_q[2],buff2_q[2],buff3_q[2],buff4_q[2]};
		end
		3'b110:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[1],buff1_q[1],buff2_q[1],buff3_q[1],buff4_q[1]};
		end
		3'b111:begin
			symbol_out_wo_qpsk <= #Delay {buff0_q[0],buff1_q[0],buff2_q[0],buff3_q[0],buff4_q[0]};
		end
		default:begin
			symbol_out_wo_qpsk <= #Delay 5'b00000;
		end
		endcase
	end
	else begin
	end
end


reg							[4:0]				symbol_out_qpsk;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_out_qpsk <= #Delay 5'b00000;
	end
	else if(fs_en == 1'b1)begin
		case({ITL_QPSK_Buff_Idx_reg_5dly,ITL_QPSK_Bit_Cnt_reg_5dly})
		4'b0000:begin
			symbol_out_qpsk <= #Delay {buff0_q[7],buff0_q[6],3'b000};
		end
		4'b0001:begin
			symbol_out_qpsk <= #Delay {buff0_q[5],buff0_q[4],3'b000};
		end
		4'b0010:begin
			symbol_out_qpsk <= #Delay {buff0_q[3],buff0_q[2],3'b000};
		end
		4'b0011:begin
			symbol_out_qpsk <= #Delay {buff0_q[1],buff0_q[0],3'b000};
		end
		4'b0100:begin
			symbol_out_qpsk <= #Delay {buff1_q[7],buff1_q[6],3'b000};
		end
		4'b0101:begin
			symbol_out_qpsk <= #Delay {buff1_q[5],buff1_q[4],3'b000};
		end
		4'b0110:begin
			symbol_out_qpsk <= #Delay {buff1_q[3],buff1_q[2],3'b000};
		end
		4'b0111:begin
			symbol_out_qpsk <= #Delay {buff1_q[1],buff1_q[0],3'b000};
		end
		4'b1000:begin
			symbol_out_qpsk <= #Delay {buff2_q[7],buff2_q[6],3'b000};
		end
		4'b1001:begin
			symbol_out_qpsk <= #Delay {buff2_q[5],buff2_q[4],3'b000};
		end
		4'b1010:begin
			symbol_out_qpsk <= #Delay {buff2_q[3],buff2_q[2],3'b000};
		end
		4'b1011:begin
			symbol_out_qpsk <= #Delay {buff2_q[1],buff2_q[0],3'b000};
		end
		default:begin
			symbol_out_qpsk <= #Delay 5'b00000;
		end
		endcase
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		oe <= #Delay 1'b0;
	end
	else if(fs_en == 1'b1)begin
		oe <= #Delay bit_itl_read_en_6dly;
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		symbol_out <= #Delay 8'h0;
	end
	else if(fs_en == 1'b1)begin
		if(bit_itl_read_en_6dly == 1'b1)begin
			case(mod_mode)
			2'b00:begin
				symbol_out <= #Delay symbol_out_qpsk;
			end
			2'b01:begin
				if(except_order == 1)begin
					symbol_out <= #Delay {symbol_out_wo_qpsk[2],symbol_out_wo_qpsk[3],symbol_out_wo_qpsk[4],symbol_out_wo_qpsk[1:0]};
				end
				else begin
					symbol_out <= #Delay symbol_out_wo_qpsk;
				end
			end
			default:begin
				symbol_out <= #Delay symbol_out_wo_qpsk;
			end
			endcase
		end
	end
	else begin
	end
end


reg							[7:0]  			buffA0_data;
reg							[11:0]  		buffA0_rdaddress;
reg							[11:0]  		buffA0_wraddress;

reg							[7:0]  			buffA1_data;
reg							[11:0]  		buffA1_rdaddress;
reg							[11:0]  		buffA1_wraddress;

reg							[7:0]  			buffA2_data;
reg							[11:0]  		buffA2_rdaddress;
reg							[11:0]  		buffA2_wraddress;

reg							[7:0]  			buffA3_data;
reg							[11:0]  		buffA3_rdaddress;
reg							[11:0]  		buffA3_wraddress;

reg							[7:0]  			buffA4_data;
reg							[11:0]  		buffA4_rdaddress;
reg							[11:0]  		buffA4_wraddress;

reg							[7:0]  			buffB0_data;
reg							[11:0]  		buffB0_rdaddress;
reg							[11:0]  		buffB0_wraddress;

reg							[7:0]  			buffB1_data;
reg							[11:0]  		buffB1_rdaddress;
reg							[11:0]  		buffB1_wraddress;

reg							[7:0]  			buffB2_data;
reg							[11:0]  		buffB2_rdaddress;
reg							[11:0]  		buffB2_wraddress;

reg							[7:0]  			buffB3_data;
reg							[11:0]  		buffB3_rdaddress;
reg							[11:0]  		buffB3_wraddress;

reg							[7:0]  			buffB4_data;
reg							[11:0]  		buffB4_rdaddress;
reg							[11:0]  		buffB4_wraddress;

wire						[7:0]  			buffA0_q_1pre;
wire						[7:0]  			buffA1_q_1pre;
wire						[7:0]  			buffA2_q_1pre;
wire						[7:0]  			buffA3_q_1pre;
wire						[7:0]  			buffA4_q_1pre;

reg						[7:0]  			buffA0_q;
reg						[7:0]  			buffA1_q;
reg						[7:0]  			buffA2_q;
reg						[7:0]  			buffA3_q;
reg						[7:0]  			buffA4_q;

reg											buffA0_rden;
reg											buffA1_rden;
reg											buffA2_rden;
reg											buffA3_rden;
reg											buffA4_rden;

reg											buffA0_wren;
reg											buffA1_wren;
reg											buffA2_wren;
reg											buffA3_wren;
reg											buffA4_wren;

wire						[7:0]  			buffB0_q_1pre;
wire						[7:0]  			buffB1_q_1pre;
wire						[7:0]  			buffB2_q_1pre;
wire						[7:0]  			buffB3_q_1pre;
wire						[7:0]  			buffB4_q_1pre;

reg						[7:0]  			buffB0_q;
reg						[7:0]  			buffB1_q;
reg						[7:0]  			buffB2_q;
reg						[7:0]  			buffB3_q;
reg						[7:0]  			buffB4_q;

reg											buffB0_rden;
reg											buffB1_rden;
reg											buffB2_rden;
reg											buffB3_rden;
reg											buffB4_rden;

reg											buffB0_wren;
reg											buffB1_wren;
reg											buffB2_wren;
reg											buffB3_wren;
reg											buffB4_wren;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		buffA0_q <= 8'h0;
		buffA1_q <= 8'h0;
		buffA2_q <= 8'h0;
		buffA3_q <= 8'h0;
		buffA4_q <= 8'h0;
		
		buffB0_q <= 8'h0;
		buffB1_q <= 8'h0;
		buffB2_q <= 8'h0;
		buffB3_q <= 8'h0;
		buffB4_q <= 8'h0;
	end
	else if(fs_en == 1'b1)begin
		buffA0_q <= buffA0_q_1pre;
		buffA1_q <= buffA1_q_1pre;
		buffA2_q <= buffA2_q_1pre;
		buffA3_q <= buffA3_q_1pre;
		buffA4_q <= buffA4_q_1pre;
		
		buffB0_q <= buffB0_q_1pre;
		buffB1_q <= buffB1_q_1pre;
		buffB2_q <= buffB2_q_1pre;
		buffB3_q <= buffB3_q_1pre;
		buffB4_q <= buffB4_q_1pre;
	end
	else begin
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		buff0_q <= #Delay 8'b0;
		buff1_q <= #Delay 8'b0;
		buff2_q <= #Delay 8'b0;
		buff3_q <= #Delay 8'b0;
		buff4_q <= #Delay 8'b0;
		
		buffA0_rden <= #Delay 1'b0;
		buffA1_rden <= #Delay 1'b0;
		buffA2_rden <= #Delay 1'b0;
		buffA3_rden <= #Delay 1'b0;
		buffA4_rden <= #Delay 1'b0;
		
		buffB0_rden <= #Delay 1'b0;
		buffB1_rden <= #Delay 1'b0;
		buffB2_rden <= #Delay 1'b0;
		buffB3_rden <= #Delay 1'b0;
		buffB4_rden <= #Delay 1'b0;
		
		buffA0_rdaddress <= #Delay 12'h0;
		buffA1_rdaddress <= #Delay 12'h0;
		buffA2_rdaddress <= #Delay 12'h0;
		buffA3_rdaddress <= #Delay 12'h0;
		buffA4_rdaddress <= #Delay 12'h0;
		
		buffB0_rdaddress <= #Delay 12'h0;
		buffB1_rdaddress <= #Delay 12'h0;
		buffB2_rdaddress <= #Delay 12'h0;
		buffB3_rdaddress <= #Delay 12'h0;
		buffB4_rdaddress <= #Delay 12'h0;
		
	end
	else if(fs_en == 1'b1)begin
		if(out_pingpong_flag_reg == 1'b1)begin// align with bit_itl_read_head_1dly
			buff0_q <= #Delay buffA0_q;
			buff1_q <= #Delay buffA1_q;
			buff2_q <= #Delay buffA2_q;
			buff3_q <= #Delay buffA3_q;
			buff4_q <= #Delay buffA4_q;
			
			buffA0_rden <= #Delay buff0_rden;// align with bit_itl_read_head_2dly
			buffA1_rden <= #Delay buff1_rden;
			buffA2_rden <= #Delay buff2_rden;
			buffA3_rden <= #Delay buff3_rden;
			buffA4_rden <= #Delay buff4_rden;
			
			buffB0_rden <= #Delay 1'b0;
			buffB1_rden <= #Delay 1'b0;
			buffB2_rden <= #Delay 1'b0;
			buffB3_rden <= #Delay 1'b0;
			buffB4_rden <= #Delay 1'b0;
			
			buffA0_rdaddress <= #Delay buff0_rdaddress;
			buffA1_rdaddress <= #Delay buff1_rdaddress;
			buffA2_rdaddress <= #Delay buff2_rdaddress;
			buffA3_rdaddress <= #Delay buff3_rdaddress;
			buffA4_rdaddress <= #Delay buff4_rdaddress;
			
			buffB0_rdaddress <= #Delay 12'h0;
			buffB1_rdaddress <= #Delay 12'h0;
			buffB2_rdaddress <= #Delay 12'h0;
			buffB3_rdaddress <= #Delay 12'h0;
			buffB4_rdaddress <= #Delay 12'h0;
		end
		else begin
			buff0_q <= #Delay buffB0_q;
			buff1_q <= #Delay buffB1_q;
			buff2_q <= #Delay buffB2_q;
			buff3_q <= #Delay buffB3_q;
			buff4_q <= #Delay buffB4_q;
			
			buffA0_rden <= #Delay 1'b0;
			buffA1_rden <= #Delay 1'b0;
			buffA2_rden <= #Delay 1'b0;
			buffA3_rden <= #Delay 1'b0;
			buffA4_rden <= #Delay 1'b0;
			
			buffB0_rden <= #Delay buff0_rden;
			buffB1_rden <= #Delay buff1_rden;
			buffB2_rden <= #Delay buff2_rden;
			buffB3_rden <= #Delay buff3_rden;
			buffB4_rden <= #Delay buff4_rden;
			
			buffA0_rdaddress <= #Delay 12'h0;
			buffA1_rdaddress <= #Delay 12'h0;
			buffA2_rdaddress <= #Delay 12'h0;
			buffA3_rdaddress <= #Delay 12'h0;
			buffA4_rdaddress <= #Delay 12'h0;
			
			buffB0_rdaddress <= #Delay buff0_rdaddress;
			buffB1_rdaddress <= #Delay buff1_rdaddress;
			buffB2_rdaddress <= #Delay buff2_rdaddress;
			buffB3_rdaddress <= #Delay buff3_rdaddress;
			buffB4_rdaddress <= #Delay buff4_rdaddress;
		end
	end
	else begin
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		buffA0_wren <= #Delay 1'h0;
		buffA1_wren <= #Delay 1'h0;
		buffA2_wren <= #Delay 1'h0;
		buffA3_wren <= #Delay 1'h0;
		buffA4_wren <= #Delay 1'h0;
		
		buffB0_wren <= #Delay 1'h0;
		buffB1_wren <= #Delay 1'h0;
		buffB2_wren <= #Delay 1'h0;
		buffB3_wren <= #Delay 1'h0;
		buffB4_wren <= #Delay 1'h0;
	end
	else if(fs_en == 1'b1)begin
		if(in_pingpong_flag_1dly_reg == 1'b0)begin
			buffA0_wren <= #Delay buff0_wren;
			buffA1_wren <= #Delay buff1_wren;
			buffA2_wren <= #Delay buff2_wren;
			buffA3_wren <= #Delay buff3_wren;
			buffA4_wren <= #Delay buff4_wren;
			buffB0_wren <= #Delay 1'h0;
			buffB1_wren <= #Delay 1'h0;
			buffB2_wren <= #Delay 1'h0;
			buffB3_wren <= #Delay 1'h0;
			buffB4_wren <= #Delay 1'h0;
		end
		else begin
			buffA0_wren <= #Delay 1'h0;
			buffA1_wren <= #Delay 1'h0;
			buffA2_wren <= #Delay 1'h0;
			buffA3_wren <= #Delay 1'h0;
			buffA4_wren <= #Delay 1'h0;
			buffB0_wren <= #Delay buff0_wren;
			buffB1_wren <= #Delay buff1_wren;
			buffB2_wren <= #Delay buff2_wren;
			buffB3_wren <= #Delay buff3_wren;
			buffB4_wren <= #Delay buff4_wren;
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		buffA0_wraddress <= #Delay 12'h0;
		buffA1_wraddress <= #Delay 12'h0;
		buffA2_wraddress <= #Delay 12'h0;
		buffA3_wraddress <= #Delay 12'h0;
		buffA4_wraddress <= #Delay 12'h0;
		
		buffB0_wraddress <= #Delay 12'h0;
		buffB1_wraddress <= #Delay 12'h0;
		buffB2_wraddress <= #Delay 12'h0;
		buffB3_wraddress <= #Delay 12'h0;
		buffB4_wraddress <= #Delay 12'h0;
	end
	else if(fs_en == 1'b1)begin
		buffA0_wraddress <= #Delay buff0_wraddress;
		buffA1_wraddress <= #Delay buff1_wraddress;
		buffA2_wraddress <= #Delay buff2_wraddress;
		buffA3_wraddress <= #Delay buff3_wraddress;
		buffA4_wraddress <= #Delay buff4_wraddress;
		
		buffB0_wraddress <= #Delay buff0_wraddress;
		buffB1_wraddress <= #Delay buff1_wraddress;
		buffB2_wraddress <= #Delay buff2_wraddress;
		buffB3_wraddress <= #Delay buff3_wraddress;
		buffB4_wraddress <= #Delay buff4_wraddress;
	end
	else begin
	end
end

/*
always @(posedge clk_1x)begin
	if(~rst_n)begin
		buffA0_rdaddress <= #Delay 12'h0;
		buffA1_rdaddress <= #Delay 12'h0;
		buffA2_rdaddress <= #Delay 12'h0;
		buffA3_rdaddress <= #Delay 12'h0;
		buffA4_rdaddress <= #Delay 12'h0;
		
		buffB0_rdaddress <= #Delay 12'h0;
		buffB1_rdaddress <= #Delay 12'h0;
		buffB2_rdaddress <= #Delay 12'h0;
		buffB3_rdaddress <= #Delay 12'h0;
		buffB4_rdaddress <= #Delay 12'h0;
	end
	else begin
		buffA0_rdaddress <= #Delay buff0_rdaddress;
		buffA1_rdaddress <= #Delay buff1_rdaddress;
		buffA2_rdaddress <= #Delay buff2_rdaddress;
		buffA3_rdaddress <= #Delay buff3_rdaddress;
		buffA4_rdaddress <= #Delay buff4_rdaddress;
		
		buffB0_rdaddress <= #Delay buff0_rdaddress;
		buffB1_rdaddress <= #Delay buff1_rdaddress;
		buffB2_rdaddress <= #Delay buff2_rdaddress;
		buffB3_rdaddress <= #Delay buff3_rdaddress;
		buffB4_rdaddress <= #Delay buff4_rdaddress;
	end
end
*/

always @(posedge sys_clk)begin
	if(~rst_n)begin
		buffA0_data <= #Delay 8'h0;
		buffA1_data <= #Delay 8'h0;
		buffA2_data <= #Delay 8'h0;
		buffA3_data <= #Delay 8'h0;
		buffA4_data <= #Delay 8'h0;
		
		buffB0_data <= #Delay 8'h0;
		buffB1_data <= #Delay 8'h0;
		buffB2_data <= #Delay 8'h0;
		buffB3_data <= #Delay 8'h0;
		buffB4_data <= #Delay 8'h0;
	end
	else if(fs_en == 1'b1)begin
		buffA0_data <= #Delay buff0_data;
		buffA1_data <= #Delay buff1_data;
		buffA2_data <= #Delay buff2_data;
		buffA3_data <= #Delay buff3_data;
		buffA4_data <= #Delay buff4_data;
		
		buffB0_data <= #Delay buff0_data;
		buffB1_data <= #Delay buff1_data;
		buffB2_data <= #Delay buff2_data;
		buffB3_data <= #Delay buff3_data;
		buffB4_data <= #Delay buff4_data;
	end
	else begin
	end
end

ITL_Byte_buff_Ram ITL_Byte_buffA0_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffA0_wren&&fs_en),      // input wire ena
  .wea(buffA0_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffA0_wraddress),  // input wire [11 : 0] addra
  .dina(buffA0_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffA0_rden&&fs_en),      // input wire enb
  .addrb(buffA0_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffA0_q_1pre)  // output wire [8 : 0] doutb
);


//ITL_Byte_buff_Ram ITL_Byte_buffA0_reg(
//.clock										(sys_clk),
//.data										(buffA0_data),
//.rdaddress									(buffA0_rdaddress),
//.rden										(buffA0_rden&&fs_en),
//.wraddress									(buffA0_wraddress),
//.wren										(buffA0_wren&&fs_en),
//.q											(buffA0_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffA1_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffA1_wren&&fs_en),      // input wire ena
  .wea(buffA1_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffA1_wraddress),  // input wire [11 : 0] addra
  .dina(buffA1_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffA1_rden&&fs_en),      // input wire enb
  .addrb(buffA1_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffA1_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffA1_reg(
//.clock										(sys_clk),
//.data										(buffA1_data),
//.rdaddress									(buffA1_rdaddress),
//.rden										(buffA1_rden&&fs_en),
//.wraddress									(buffA1_wraddress),
//.wren										(buffA1_wren&&fs_en),
//.q											(buffA1_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffA2_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffA2_wren&&fs_en),      // input wire ena
  .wea(buffA2_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffA2_wraddress),  // input wire [11 : 0] addra
  .dina(buffA2_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffA2_rden&&fs_en),      // input wire enb
  .addrb(buffA2_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffA2_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffA2_reg(
//.clock										(sys_clk),
//.data										(buffA2_data),
//.rdaddress									(buffA2_rdaddress),
//.rden										(buffA2_rden&&fs_en),
//.wraddress									(buffA2_wraddress),
//.wren										(buffA2_wren&&fs_en),
//.q											(buffA2_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffA3_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffA3_wren&&fs_en),      // input wire ena
  .wea(buffA3_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffA3_wraddress),  // input wire [11 : 0] addra
  .dina(buffA3_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffA3_rden&&fs_en),      // input wire enb
  .addrb(buffA3_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffA3_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffA3_reg(
//.clock										(sys_clk),
//.data										(buffA3_data),
//.rdaddress									(buffA3_rdaddress),
//.rden										(buffA3_rden&&fs_en),
//.wraddress									(buffA3_wraddress),
//.wren										(buffA3_wren&&fs_en),
//.q											(buffA3_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffA4_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffA4_wren&&fs_en),      // input wire ena
  .wea(buffA4_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffA4_wraddress),  // input wire [11 : 0] addra
  .dina(buffA4_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffA4_rden&&fs_en),      // input wire enb
  .addrb(buffA4_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffA4_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffA4_reg(
//.clock										(sys_clk),
//.data										(buffA4_data),
//.rdaddress									(buffA4_rdaddress),
//.rden										(buffA4_rden&&fs_en),
//.wraddress									(buffA4_wraddress),
//.wren										(buffA4_wren&&fs_en),
//.q											(buffA4_q_1pre)
//);


ITL_Byte_buff_Ram ITL_Byte_buffB0_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffB0_wren&&fs_en),      // input wire ena
  .wea(buffB0_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffB0_wraddress),  // input wire [11 : 0] addra
  .dina(buffB0_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffB0_rden&&fs_en),      // input wire enb
  .addrb(buffB0_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffB0_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffB0_reg(
//.clock										(sys_clk),
//.data										(buffB0_data),
//.rdaddress									(buffB0_rdaddress),
//.rden										(buffB0_rden&&fs_en),
//.wraddress									(buffB0_wraddress),
//.wren										(buffB0_wren&&fs_en),
//.q											(buffB0_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffB1_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffB1_wren&&fs_en),      // input wire ena
  .wea(buffB1_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffB1_wraddress),  // input wire [11 : 0] addra
  .dina(buffB1_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffB1_rden&&fs_en),      // input wire enb
  .addrb(buffB1_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffB1_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffB1_reg(
//.clock										(sys_clk),
//.data										(buffB1_data),
//.rdaddress									(buffB1_rdaddress),
//.rden										(buffB1_rden&&fs_en),
//.wraddress									(buffB1_wraddress),
//.wren										(buffB1_wren&&fs_en),
//.q											(buffB1_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffB2_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffB2_wren&&fs_en),      // input wire ena
  .wea(buffB2_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffB2_wraddress),  // input wire [11 : 0] addra
  .dina(buffB2_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffB2_rden&&fs_en),      // input wire enb
  .addrb(buffB2_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffB2_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffB2_reg(
//.clock										(sys_clk),
//.data										(buffB2_data),
//.rdaddress									(buffB2_rdaddress),
//.rden										(buffB2_rden&&fs_en),
//.wraddress									(buffB2_wraddress),
//.wren										(buffB2_wren&&fs_en),
//.q											(buffB2_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffB3_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffB3_wren&&fs_en),      // input wire ena
  .wea(buffB3_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffB3_wraddress),  // input wire [11 : 0] addra
  .dina(buffB3_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffB3_rden&&fs_en),      // input wire enb
  .addrb(buffB3_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffB3_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffB3_reg(
//.clock										(sys_clk),
//.data										(buffB3_data),
//.rdaddress									(buffB3_rdaddress),
//.rden										(buffB3_rden&&fs_en),
//.wraddress									(buffB3_wraddress),
//.wren										(buffB3_wren&&fs_en),
//.q											(buffB3_q_1pre)
//);

ITL_Byte_buff_Ram ITL_Byte_buffB4_reg (
  .clka(sys_clk),    // input wire clka
  .ena(buffB4_wren&&fs_en),      // input wire ena
  .wea(buffB4_wren&&fs_en),      // input wire [0 : 0] wea
  .addra(buffB4_wraddress),  // input wire [11 : 0] addra
  .dina(buffB4_data),    // input wire [8 : 0] dina
  .clkb(sys_clk),    // input wire clkb
  .enb(buffB4_rden&&fs_en),      // input wire enb
  .addrb(buffB4_rdaddress),  // input wire [11 : 0] addrb
  .doutb(buffB4_q_1pre)  // output wire [8 : 0] doutb
);

//ITL_Byte_buff_Ram ITL_Byte_buffB4_reg(
//.clock										(sys_clk),
//.data										(buffB4_data),
//.rdaddress									(buffB4_rdaddress),
//.rden										(buffB4_rden&&fs_en),
//.wraddress									(buffB4_wraddress),
//.wren										(buffB4_wren&&fs_en),
//.q											(buffB4_q_1pre)
//);

endmodule
