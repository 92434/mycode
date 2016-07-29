
`timescale 1ps / 1ps
module ldpc_ctrl(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							frame_mode,
input	          			[3:0] 		ldpc_mode,
//////////////////////////////////////////////////////////////
input	          							ram_clr_vld,// before byte_in Q_360 cycles but after ldpc_check output
input	          							frame_head,// before byte_in 3cycles
input	          							frame_vld,// before byte_in 3cycles
//////////////////////////////////////////////////////////////
input	          			[8:0] 		int_Q_360,
input	          			[7:0] 		yu_Q_360,
//////////////////////////////////////////////////////////////
output  reg 								shift_vld,
output  		     			[8:0]			shift_index,// cyclic shift derication is up

output  		     			[2:0] 		order_idx,
output  		 								switch_vld,
output  reg     			[5:0] 		switch_array,//0~44
output  reg     			[2:0] 		order_array,//0~7

output  reg     							reg_clr_vld,

output  						[7:0]  		check_ram0_rdaddress,
output  						[7:0]  		check_ram1_rdaddress,
output  						[7:0]  		check_ram2_rdaddress,
output  						[7:0]  		check_ram3_rdaddress,
output  						[7:0]  		check_ram4_rdaddress,
output  						[7:0]  		check_ram5_rdaddress,
output  						[7:0]  		check_ram6_rdaddress,
output  						[7:0]  		check_ram7_rdaddress,

output  reg									equal_flag,
output  reg									ram_rden,
output  		 				[7:0]  		ram_wraddress,
output  										ram_wren,

output  reg 						 		rd_h_matrix_en,
output  reg 				[12:0]  		rd_h_matrix_addr
//////////////////////////////////////////////////////////////
);

parameter Delay = 0;

reg							[7:0]			K_360_NUM;
reg							[7:0]			Q_360_NUM;
reg							[12:0]		h_matrix_offset_addr;

reg							[7:0]			not91_ram_rd_addr_tmp_0;
reg							[7:0]			not91_ram_rd_addr_tmp_1;
reg							[7:0]			not91_ram_rd_addr_tmp_2;
reg							[7:0]			not91_ram_rd_addr_tmp_3;
reg							[7:0]			not91_ram_rd_addr_tmp_4;
reg							[7:0]			not91_ram_rd_addr_tmp_5;
reg							[7:0]			not91_ram_rd_addr_tmp_6;
reg							[7:0]			not91_ram_rd_addr_tmp_7;

reg							[7:0]			is91_ram_rd_addr_tmp_0;
reg							[7:0]			is91_ram_rd_addr_tmp_1;
reg							[7:0]			is91_ram_rd_addr_tmp_2;
reg							[7:0]			is91_ram_rd_addr_tmp_3;
reg							[7:0]			is91_ram_rd_addr_tmp_4;


reg							[7:0]			ram_rd_addr_tmp_0;
reg							[7:0]			ram_rd_addr_tmp_1;
reg							[7:0]			ram_rd_addr_tmp_2;
reg							[7:0]			ram_rd_addr_tmp_3;
reg							[7:0]			ram_rd_addr_tmp_4;
reg							[7:0]			ram_rd_addr_tmp_5;
reg							[7:0]			ram_rd_addr_tmp_6;
reg							[7:0]			ram_rd_addr_tmp_7;


reg							[7:0]			ram_rd_addr_tmp_0_1pre;
reg							[7:0]			ram_rd_addr_tmp_1_1pre;
reg							[7:0]			ram_rd_addr_tmp_2_1pre;
reg							[7:0]			ram_rd_addr_tmp_3_1pre;
reg							[7:0]			ram_rd_addr_tmp_4_1pre;
reg							[7:0]			ram_rd_addr_tmp_5_1pre;
reg							[7:0]			ram_rd_addr_tmp_6_1pre;
reg							[7:0]			ram_rd_addr_tmp_7_1pre;

reg											ram_rd_addr_gen_en;
reg											ram_rd_out_flag;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_addr_tmp_0_1pre <= #Delay 0;
		ram_rd_addr_tmp_1_1pre <= #Delay 0;
		ram_rd_addr_tmp_2_1pre <= #Delay 0;
		ram_rd_addr_tmp_3_1pre <= #Delay 0;
		ram_rd_addr_tmp_4_1pre <= #Delay 0;
		ram_rd_addr_tmp_5_1pre <= #Delay 0;
		ram_rd_addr_tmp_6_1pre <= #Delay 0;
		ram_rd_addr_tmp_7_1pre <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if((ldpc_mode == 4'b1001)&&(frame_mode == 1'b1))begin
		ram_rd_addr_tmp_0_1pre <= #Delay is91_ram_rd_addr_tmp_0;
		ram_rd_addr_tmp_1_1pre <= #Delay is91_ram_rd_addr_tmp_1;
		ram_rd_addr_tmp_2_1pre <= #Delay is91_ram_rd_addr_tmp_2;
		ram_rd_addr_tmp_3_1pre <= #Delay is91_ram_rd_addr_tmp_3;
		ram_rd_addr_tmp_4_1pre <= #Delay is91_ram_rd_addr_tmp_4;
		ram_rd_addr_tmp_5_1pre <= #Delay is91_ram_rd_addr_tmp_0;
		ram_rd_addr_tmp_6_1pre <= #Delay is91_ram_rd_addr_tmp_1;
		ram_rd_addr_tmp_7_1pre <= #Delay is91_ram_rd_addr_tmp_2;
		end
		else begin
		ram_rd_addr_tmp_0_1pre <= #Delay not91_ram_rd_addr_tmp_0;
		ram_rd_addr_tmp_1_1pre <= #Delay not91_ram_rd_addr_tmp_1;
		ram_rd_addr_tmp_2_1pre <= #Delay not91_ram_rd_addr_tmp_2;
		ram_rd_addr_tmp_3_1pre <= #Delay not91_ram_rd_addr_tmp_3;
		ram_rd_addr_tmp_4_1pre <= #Delay not91_ram_rd_addr_tmp_4;
		ram_rd_addr_tmp_5_1pre <= #Delay not91_ram_rd_addr_tmp_5;
		ram_rd_addr_tmp_6_1pre <= #Delay not91_ram_rd_addr_tmp_6;
		ram_rd_addr_tmp_7_1pre <= #Delay not91_ram_rd_addr_tmp_7;
		end
	end
	else begin
	end
end


reg 							[12:0]		order_period_cnt_for_rdaddr;
reg 							[2:0]			order_array_for_rdaddr;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_addr_tmp_0 <= #Delay 0;
		ram_rd_addr_tmp_1 <= #Delay 0;
		ram_rd_addr_tmp_2 <= #Delay 0;
		ram_rd_addr_tmp_3 <= #Delay 0;
		ram_rd_addr_tmp_4 <= #Delay 0;
		ram_rd_addr_tmp_5 <= #Delay 0;
		ram_rd_addr_tmp_6 <= #Delay 0;
		ram_rd_addr_tmp_7 <= #Delay 0;
	end
	//else if(frame_mode == 1'b0)begin
	//	ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_0_1pre;
	//	ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_1_1pre;
	//	ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_2_1pre;
	//	ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_3_1pre;
	//	ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_4_1pre;
	//	ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_5_1pre;
	//	ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_6_1pre;
	//	ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_7_1pre;
	//end
	else if(fs_en == 1'b1)begin   
		case(order_array_for_rdaddr)  
		3'b000:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_7_1pre;
		end
		3'b001:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_6_1pre;
		end
		3'b010:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_5_1pre;
		end
		3'b011:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_4_1pre;
		end
		3'b100:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_3_1pre;
		end
		3'b101:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_2_1pre;
		end
		3'b110:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_0_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_1_1pre;
		end
		3'b111:begin
			ram_rd_addr_tmp_0 <= #Delay ram_rd_addr_tmp_1_1pre;
			ram_rd_addr_tmp_1 <= #Delay ram_rd_addr_tmp_2_1pre;
			ram_rd_addr_tmp_2 <= #Delay ram_rd_addr_tmp_3_1pre;
			ram_rd_addr_tmp_3 <= #Delay ram_rd_addr_tmp_4_1pre;
			ram_rd_addr_tmp_4 <= #Delay ram_rd_addr_tmp_5_1pre;
			ram_rd_addr_tmp_5 <= #Delay ram_rd_addr_tmp_6_1pre;
			ram_rd_addr_tmp_6 <= #Delay ram_rd_addr_tmp_7_1pre;
			ram_rd_addr_tmp_7 <= #Delay ram_rd_addr_tmp_0_1pre;
		end
		endcase
		end
	else begin
	end
end 
    
    
always @(posedge sys_clk)begin
	if(~rst_n)begin
		not91_ram_rd_addr_tmp_0 <= #Delay 0;
		not91_ram_rd_addr_tmp_1 <= #Delay 0;
		not91_ram_rd_addr_tmp_2 <= #Delay 0;
		not91_ram_rd_addr_tmp_3 <= #Delay 0;
		not91_ram_rd_addr_tmp_4 <= #Delay 0;
		not91_ram_rd_addr_tmp_5 <= #Delay 0;
		not91_ram_rd_addr_tmp_6 <= #Delay 0;
		not91_ram_rd_addr_tmp_7 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			if((not91_ram_rd_addr_tmp_0 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_0 <= #Delay (not91_ram_rd_addr_tmp_0 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_0 <= #Delay not91_ram_rd_addr_tmp_0 + 8;
			end
			
			if((not91_ram_rd_addr_tmp_1 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_1 <= #Delay (not91_ram_rd_addr_tmp_1 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_1 <= #Delay not91_ram_rd_addr_tmp_1 + 8;
			end
			if((not91_ram_rd_addr_tmp_2 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_2 <= #Delay (not91_ram_rd_addr_tmp_2 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_2 <= #Delay not91_ram_rd_addr_tmp_2 + 8;
			end
			
			if((not91_ram_rd_addr_tmp_3 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_3 <= #Delay (not91_ram_rd_addr_tmp_3 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_3 <= #Delay not91_ram_rd_addr_tmp_3 + 8;
			end
			
			if((not91_ram_rd_addr_tmp_4 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_4 <= #Delay (not91_ram_rd_addr_tmp_4 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_4 <= #Delay not91_ram_rd_addr_tmp_4 + 8;
			end
			
			if((not91_ram_rd_addr_tmp_5 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_5 <= #Delay (not91_ram_rd_addr_tmp_5 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_5 <= #Delay not91_ram_rd_addr_tmp_5 + 8;
			end
			if((not91_ram_rd_addr_tmp_6 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_6 <= #Delay (not91_ram_rd_addr_tmp_6 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_6 <= #Delay not91_ram_rd_addr_tmp_6 + 8;
			end
			
			if((not91_ram_rd_addr_tmp_7 + 8)>Q_360_NUM)begin
				not91_ram_rd_addr_tmp_7 <= #Delay (not91_ram_rd_addr_tmp_7 + 7) - Q_360_NUM;
			end
			else begin
				not91_ram_rd_addr_tmp_7 <= #Delay not91_ram_rd_addr_tmp_7 + 8;
			end
		end
		else begin
			not91_ram_rd_addr_tmp_0 <= #Delay 0;
			not91_ram_rd_addr_tmp_1 <= #Delay 1;
			not91_ram_rd_addr_tmp_2 <= #Delay 2;
			not91_ram_rd_addr_tmp_3 <= #Delay 3;
			not91_ram_rd_addr_tmp_4 <= #Delay 4;
			not91_ram_rd_addr_tmp_5 <= #Delay 5;
			not91_ram_rd_addr_tmp_6 <= #Delay 6;
			not91_ram_rd_addr_tmp_7 <= #Delay 7;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		is91_ram_rd_addr_tmp_0 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			case(is91_ram_rd_addr_tmp_0)
			3'b000:is91_ram_rd_addr_tmp_0 <= #Delay 3;  
			3'b001:is91_ram_rd_addr_tmp_0 <= #Delay 4;
			3'b010:is91_ram_rd_addr_tmp_0 <= #Delay 0;
			3'b011:is91_ram_rd_addr_tmp_0 <= #Delay 1;
			3'b100:is91_ram_rd_addr_tmp_0 <= #Delay 2;
			default:is91_ram_rd_addr_tmp_0 <= #Delay 0;
			endcase
		end
		else begin
			is91_ram_rd_addr_tmp_0 <= #Delay 0;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		is91_ram_rd_addr_tmp_1 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			case(is91_ram_rd_addr_tmp_1)
			3'b000:is91_ram_rd_addr_tmp_1 <= #Delay 3;  
			3'b001:is91_ram_rd_addr_tmp_1 <= #Delay 4;
			3'b010:is91_ram_rd_addr_tmp_1 <= #Delay 0;
			3'b011:is91_ram_rd_addr_tmp_1 <= #Delay 1;
			3'b100:is91_ram_rd_addr_tmp_1 <= #Delay 2;
			default:is91_ram_rd_addr_tmp_1 <= #Delay 0;
			endcase
		end
		else begin
			is91_ram_rd_addr_tmp_1 <= #Delay 1;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		is91_ram_rd_addr_tmp_2 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			case(is91_ram_rd_addr_tmp_2)
			3'b000:is91_ram_rd_addr_tmp_2 <= #Delay 3;  
			3'b001:is91_ram_rd_addr_tmp_2 <= #Delay 4;
			3'b010:is91_ram_rd_addr_tmp_2 <= #Delay 0;
			3'b011:is91_ram_rd_addr_tmp_2 <= #Delay 1;
			3'b100:is91_ram_rd_addr_tmp_2 <= #Delay 2;
			default:is91_ram_rd_addr_tmp_2 <= #Delay 0;
			endcase
		end
		else begin
			is91_ram_rd_addr_tmp_2 <= #Delay 2;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		is91_ram_rd_addr_tmp_3 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			case(is91_ram_rd_addr_tmp_3)
			3'b000:is91_ram_rd_addr_tmp_3 <= #Delay 3;  
			3'b001:is91_ram_rd_addr_tmp_3 <= #Delay 4;
			3'b010:is91_ram_rd_addr_tmp_3 <= #Delay 0;
			3'b011:is91_ram_rd_addr_tmp_3 <= #Delay 1;
			3'b100:is91_ram_rd_addr_tmp_3 <= #Delay 2;
			default:is91_ram_rd_addr_tmp_3 <= #Delay 0;
			endcase
		end
		else begin
			is91_ram_rd_addr_tmp_3 <= #Delay 3;
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		is91_ram_rd_addr_tmp_4 <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en)begin
			case(is91_ram_rd_addr_tmp_4)
			3'b000:is91_ram_rd_addr_tmp_4 <= #Delay 3;  
			3'b001:is91_ram_rd_addr_tmp_4 <= #Delay 4;
			3'b010:is91_ram_rd_addr_tmp_4 <= #Delay 0;
			3'b011:is91_ram_rd_addr_tmp_4 <= #Delay 1;
			3'b100:is91_ram_rd_addr_tmp_4 <= #Delay 2;
			default:is91_ram_rd_addr_tmp_4 <= #Delay 0;
			endcase
		end
		else begin
			is91_ram_rd_addr_tmp_4 <= #Delay 4;
		end
	end
	else begin
	end
end


reg							ram_rd_addr_gen_en_1dly;
//reg							ram_rd_addr_gen_en_2dly;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ram_rd_addr_gen_en_1dly <= #Delay 0;
		//ram_rd_addr_gen_en_2dly <= #Delay 0;
	end
	else if(fs_en == 1'b1)begin
		ram_rd_addr_gen_en_1dly <= #Delay ram_rd_addr_gen_en;
		//ram_rd_addr_gen_en_2dly <= #Delay ram_rd_addr_gen_en_1dly;
	end
	else begin
	end
end


assign check_ram0_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_0;
assign check_ram1_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_1;
assign check_ram2_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_2;
assign check_ram3_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_3;
assign check_ram4_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_4;
assign check_ram5_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_5;
assign check_ram6_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_6;
assign check_ram7_rdaddress =  (ram_rd_out_flag == 1'b0)?yu_Q_360:ram_rd_addr_tmp_7;

parameter					IDLE_STATE    				= 10'b0000000001;
parameter					RAM_CLR_STATE 				= 10'b0000000010;
parameter					LDPC_START_STATE 			= 10'b0000000100;
parameter					LDPC_CODING_ODD_STATE	= 10'b0000001000;
parameter					LDPC_STOP_STATE 			= 10'b0000010000;
parameter					LDPC_FINISH_STATE 		= 10'b0000100000;
parameter					OUT_START_STATE 			= 10'b0001000000;
parameter					OUT_PUTING_STATE 			= 10'b0010000000;
parameter					OUT_FINISH_STATE 			= 10'b0100000000;
parameter					LDPC_CODING_EVEN_STATE	= 10'b1000000000;

reg							[9:0]			ldpc_ctrl_state;


reg 							[7:0]  		check_ram_wraddress;
reg											check_ram_wren;

reg											shift_vld_1dly;

reg 							[7:0]  		yu_Q_360_1dly;
reg 							[7:0]  		yu_Q_360_2dly;
reg 							[7:0]  		K_360_CNT_REG;
reg 							[7:0]  		Q_360_CNT_REG;
reg 							[7:0]  		ram_rd_addr_tmp;


assign ram_wren =  (ram_clr_vld == 1'b1)?check_ram_wren:shift_vld_1dly;
assign ram_wraddress =  (ram_clr_vld == 1'b1)?check_ram_wraddress:yu_Q_360_2dly;
assign shift_index =  int_Q_360;//(shift_vld == 1'b1)?int_Q_360:9'h000;
assign order_idx =  yu_Q_360_1dly[2:0];// modified by wj 2014.06.01 

reg											frame_head_1dly;
reg											frame_head_2dly;
//reg											frame_head_3dly;

reg											ram_rd_out_flag_1dly;
reg											ram_rd_out_flag_2dly;


assign switch_vld =  ram_rd_out_flag_2dly;

reg 							[12:0]		order_period;
reg 							[7:0]		   switch_period;

reg 							[12:0]		order_period_cnt;
reg 							[7:0]		   switch_period_cnt;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		order_period_cnt <= #Delay 13'd0 ;
		order_array <= #Delay 3'b000;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_out_flag_2dly)begin
				if(order_period_cnt == order_period)begin
					order_array <= #Delay order_array + 1;
					order_period_cnt <= #Delay 13'd0 ;
				end
				else begin
					order_period_cnt <= #Delay order_period_cnt + 13'd1 ;
				end
		end
		else begin
			order_array <= #Delay 3'b000;
			order_period_cnt <= #Delay 13'd0 ;
		end
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		switch_period_cnt <= #Delay 8'd0 ;
		switch_array <= #Delay 6'b000000;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_out_flag_2dly)begin
				if(switch_period_cnt == switch_period)begin
					if(switch_array == 6'd44)begin
						switch_array <= #Delay 6'b000000;
					end
					else begin
						switch_array <= #Delay switch_array + 1;
					end
					switch_period_cnt <= #Delay 8'd0 ;
				end
				else begin
					switch_period_cnt <= #Delay switch_period_cnt + 8'd1 ;
				end
		end
		else begin
			switch_period_cnt <= #Delay 8'd0 ;
			switch_array <= #Delay 6'b000000;
		end
	end
	else begin
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		order_period <= #Delay 13'd0 ;
	end
	else begin
	case({ldpc_mode,frame_mode})
	  5'b00000:order_period <= #Delay 13'd6075;
		5'b00010:order_period <= #Delay 13'd674 ;//N - 1
		5'b00100:order_period <= #Delay 13'd1214;
		5'b00110:order_period <= #Delay 13'd2024;
		5'b01000:order_period <= #Delay 13'd404 ;
		5'b01010:order_period <= #Delay 13'd674 ;
		5'b01100:order_period <= #Delay 13'd2024;
		5'b01110:order_period <= #Delay 13'd404 ;
		5'b10000:order_period <= #Delay 13'd674 ;
		5'b10010:order_period <= #Delay 13'd224 ;
		5'b10100:order_period <= #Delay 13'd404 ;
		5'b00001:order_period <= #Delay 13'd404 ;
		5'b00011:order_period <= #Delay 13'd674 ;
		5'b00101:order_period <= #Delay 13'd1214;
		5'b00111:order_period <= #Delay 13'd1124;
		5'b01001:order_period <= #Delay 13'd404 ;
		5'b01011:order_period <= #Delay 13'd674 ;
		5'b01101:order_period <= #Delay 13'd134 ;
		5'b01111:order_period <= #Delay 13'd224 ;
		5'b10001:order_period <= #Delay 13'd44  ;
		5'b10011:order_period <= #Delay 13'd224 ;
		default:order_period <= #Delay 13'd0 ;
	endcase
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		switch_period <= #Delay 8'd0 ;
	end
	else begin
	case({ldpc_mode,frame_mode})
		   5'b00000:switch_period <= #Delay 8'd134;//N - 1
			5'b00010:switch_period <= #Delay 8'd14 ;
			5'b00100:switch_period <= #Delay 8'd26 ;
			5'b00110:switch_period <= #Delay 8'd44 ;
			5'b01000:switch_period <= #Delay 8'd8  ;
			5'b01010:switch_period <= #Delay 8'd14 ;
			5'b01100:switch_period <= #Delay 8'd44 ;
			5'b01110:switch_period <= #Delay 8'd8  ;
			5'b10000:switch_period <= #Delay 8'd14 ;
			5'b10010:switch_period <= #Delay 8'd4  ;
			5'b10100:switch_period <= #Delay 8'd8  ;
			5'b00001:switch_period <= #Delay 8'd8 ;
			5'b00011:switch_period <= #Delay 8'd14;
			5'b00101:switch_period <= #Delay 8'd26;
			5'b00111:switch_period <= #Delay 8'd24;
			5'b01001:switch_period <= #Delay 8'd8 ;
			5'b01011:switch_period <= #Delay 8'd14;
			5'b01101:switch_period <= #Delay 8'd2 ;
			5'b01111:switch_period <= #Delay 8'd4 ;
			5'b10001:switch_period <= #Delay 8'd0 ;
			5'b10011:switch_period <= #Delay 8'd4 ;
			default:switch_period <= #Delay 8'd0 ;
	endcase
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		shift_vld <= #Delay 1'b0;
		shift_vld_1dly <= #Delay 1'b0;
		yu_Q_360_1dly <= #Delay 8'h0;
		yu_Q_360_2dly <= #Delay 8'h0;
		frame_head_1dly <= #Delay 1'b0;
		frame_head_2dly <= #Delay 1'b0;
		//frame_head_3dly <= #Delay 1'b0;
		ram_rd_out_flag_1dly <= #Delay 1'b0;
		ram_rd_out_flag_2dly <= #Delay 1'b0;
	end
	else if(fs_en == 1'b1)begin	
		shift_vld <= #Delay ((ram_rd_out_flag == 1'b0)&&(ram_rden||equal_flag));
		shift_vld_1dly <= #Delay shift_vld;
		yu_Q_360_1dly <= #Delay yu_Q_360;
		yu_Q_360_2dly <= #Delay yu_Q_360_1dly;
		frame_head_1dly <= #Delay frame_head;
		frame_head_2dly <= #Delay frame_head_1dly;
		//frame_head_3dly <= #Delay frame_head_2dly;
		ram_rd_out_flag_1dly <= #Delay ram_rd_out_flag;
		ram_rd_out_flag_2dly <= #Delay ram_rd_out_flag_1dly;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ldpc_ctrl_state 		<= #Delay IDLE_STATE;
		reg_clr_vld 			<= #Delay 1'b0;
		check_ram_wraddress 	<= #Delay 8'h00;
		check_ram_wren      	<= #Delay 1'b0;
		ram_rden            	<= #Delay 1'b0;
		equal_flag           <= #Delay 1'b0;
		rd_h_matrix_en 		<= #Delay 1'b0;
		rd_h_matrix_addr 		<= #Delay 13'h0000;
		K_360_CNT_REG 			<= #Delay 8'h00;
		Q_360_CNT_REG 			<= #Delay 8'h00;
		ram_rd_addr_tmp      <= #Delay 8'h00;
		ram_rd_out_flag 		<= #Delay 1'b0;
		ram_rd_addr_gen_en	<= #Delay 1'b0;
	end
	else if(fs_en == 1'b1)begin
		case(ldpc_ctrl_state)
		IDLE_STATE:begin
				if(ram_clr_vld)begin
					ldpc_ctrl_state <= #Delay RAM_CLR_STATE;
					reg_clr_vld <= #Delay 1'b1;
					check_ram_wraddress <= #Delay 8'h00;
					check_ram_wren      <= #Delay 1'b1;
				end
				else begin
					ldpc_ctrl_state <= #Delay IDLE_STATE;
					reg_clr_vld <= #Delay 1'b0;
					check_ram_wraddress <= #Delay 8'h00;
					check_ram_wren      <= #Delay 1'b0;
				end
		end
		RAM_CLR_STATE:begin
				if(check_ram_wraddress == (Q_360_NUM + 1))begin
					check_ram_wraddress <= #Delay 8'h00;
					check_ram_wren      <= #Delay 1'b0;
					ldpc_ctrl_state <= #Delay LDPC_START_STATE;
				end
				else begin
					check_ram_wraddress <= #Delay check_ram_wraddress + 8'h01;
					check_ram_wren      <= #Delay 1'b1;
					ldpc_ctrl_state <= #Delay RAM_CLR_STATE;
				end
				reg_clr_vld 		  <= #Delay 1'b0;
		end
		LDPC_START_STATE:begin
				if(frame_head)begin
					rd_h_matrix_en <= #Delay 1'b1;
					rd_h_matrix_addr <= #Delay h_matrix_offset_addr;
					ldpc_ctrl_state <= #Delay LDPC_CODING_ODD_STATE;
				end
				else begin
					rd_h_matrix_en <= #Delay 1'b0;
					rd_h_matrix_addr <= #Delay 0;
					ldpc_ctrl_state <= #Delay LDPC_START_STATE;
				end
		end
		LDPC_CODING_ODD_STATE:begin
				rd_h_matrix_en <= #Delay 1'b1;
				rd_h_matrix_addr <= #Delay rd_h_matrix_addr + 1'b1;
				ldpc_ctrl_state <= #Delay LDPC_CODING_EVEN_STATE;
				ram_rden		 <= #Delay 1'b0;
				equal_flag   <= #Delay 1'b0;
		end
		LDPC_CODING_EVEN_STATE:begin
				rd_h_matrix_en <= #Delay 1'b1;
				if(yu_Q_360 == 8'hff)begin
					if(K_360_CNT_REG == K_360_NUM)begin
						K_360_CNT_REG <= #Delay 0;
						ldpc_ctrl_state <= #Delay LDPC_FINISH_STATE;
						//ram_rd_addr_tmp <= #Delay ram_rd_addr_tmp + 1'b1;
						ram_rd_addr_gen_en		 <= #Delay 1'b1;
					end
					else begin
						K_360_CNT_REG <= #Delay K_360_CNT_REG + 1'b1;
						ldpc_ctrl_state <= #Delay LDPC_STOP_STATE;
					end
				end
				else begin
					ldpc_ctrl_state <= #Delay LDPC_CODING_ODD_STATE;
				end
				
				if((frame_head_2dly == 1'b0)&&(yu_Q_360 == yu_Q_360_1dly))begin
					ram_rden		 <= #Delay 1'b0;
					equal_flag   <= #Delay 1'b1;
				end
				else if(yu_Q_360 == 8'hff)begin
					ram_rden		 <= #Delay 1'b0;
					equal_flag   <= #Delay 1'b0;
				end
				else begin
					ram_rden		 <= #Delay 1'b1;
					equal_flag   <= #Delay 1'b0;
				end
		end
		LDPC_STOP_STATE:begin
				ram_rden		 <= #Delay 1'b0;
				equal_flag   <= #Delay 1'b0;
				if(frame_vld)begin
					rd_h_matrix_en <= #Delay 1'b1;
					//rd_h_matrix_addr <= #Delay rd_h_matrix_addr + 1;
					ldpc_ctrl_state <= #Delay LDPC_CODING_ODD_STATE;
				end
				else begin
					rd_h_matrix_en <= #Delay 1'b0;
					//rd_h_matrix_addr <= #Delay 0;
					ldpc_ctrl_state <= #Delay LDPC_STOP_STATE;
				end
		end
		LDPC_FINISH_STATE:begin
				ram_rden		 <= #Delay 1'b0;
				equal_flag   <= #Delay 1'b0;
				rd_h_matrix_en <= #Delay 1'b0;
				//rd_h_matrix_addr <= #Delay 0;
				//ram_rd_addr_tmp <= #Delay ram_rd_addr_tmp + 1'b1;
				ldpc_ctrl_state <= #Delay OUT_START_STATE;
		end
		OUT_START_STATE:begin
				ram_rden		 <= #Delay 1'b1;
				ram_rd_out_flag <= #Delay 1'b1;
				ldpc_ctrl_state <= #Delay OUT_PUTING_STATE;
				//ram_rd_addr_tmp <= #Delay ram_rd_addr_tmp + 1'b1;
		end
		OUT_PUTING_STATE:begin
				if(ram_rd_addr_tmp == Q_360_NUM)begin
					ram_rd_addr_tmp <= #Delay 8'h00;
					if(Q_360_CNT_REG == 44)begin
						Q_360_CNT_REG <= #Delay 0;
						ldpc_ctrl_state <= #Delay OUT_FINISH_STATE;
						ram_rden		 <= #Delay 1'b0;
						ram_rd_out_flag <= #Delay 1'b0;
						ram_rd_addr_gen_en	<= #Delay 1'b0;
					end
					else begin
						Q_360_CNT_REG <= #Delay Q_360_CNT_REG + 1'b1;
						ldpc_ctrl_state <= #Delay OUT_PUTING_STATE;
					end
				end
				else begin
					ram_rd_addr_tmp <= #Delay ram_rd_addr_tmp + 1'b1;
				end
		end
		OUT_FINISH_STATE:begin
				ldpc_ctrl_state 		<= #Delay IDLE_STATE;
				reg_clr_vld 			<= #Delay 1'b0;
				check_ram_wraddress 	<= #Delay 8'h00;
				check_ram_wren      	<= #Delay 1'b0;
				ram_rden            	<= #Delay 1'b0;
				equal_flag           <= #Delay 1'b0;
				rd_h_matrix_en 		<= #Delay 1'b0;
				rd_h_matrix_addr 		<= #Delay 13'h0000;
				K_360_CNT_REG 			<= #Delay 8'h00;
				Q_360_CNT_REG 			<= #Delay 8'h00;
				ram_rd_addr_tmp      <= #Delay 8'h00;
				ram_rd_out_flag 		<= #Delay 1'b0;
				ram_rd_addr_gen_en	<= #Delay 1'b0;
		end
		default:begin
			ldpc_ctrl_state 		<= #Delay IDLE_STATE;
			reg_clr_vld 			<= #Delay 1'b0;
			check_ram_wraddress 	<= #Delay 8'h00;
			check_ram_wren      	<= #Delay 1'b0;
			ram_rden            	<= #Delay 1'b0;
			equal_flag           <= #Delay 1'b0;
			rd_h_matrix_en 		<= #Delay 1'b0;
			rd_h_matrix_addr 		<= #Delay 13'h0000;
			K_360_CNT_REG 			<= #Delay 8'h00;
			Q_360_CNT_REG 			<= #Delay 8'h00;
			ram_rd_addr_tmp      <= #Delay 8'h00;
			ram_rd_out_flag 		<= #Delay 1'b0;
			ram_rd_addr_gen_en	<= #Delay 1'b0;
		end
		endcase
	end
	else begin
	end
end



always @(posedge sys_clk)begin
	if(~rst_n)begin
		K_360_NUM <= #Delay 8'b00000000;
	end
	else if(frame_mode == 0)begin
		case (ldpc_mode)
			4'h0:
				K_360_NUM <= #Delay 8'd44;// NUM - 1
			4'h1:
				K_360_NUM <= #Delay 8'd59;
			4'h2:
				K_360_NUM <= #Delay 8'd71;
			4'h3:
				K_360_NUM <= #Delay 8'd89;
			4'h4:
				K_360_NUM <= #Delay 8'd107;
			4'h5:
				K_360_NUM <= #Delay 8'd119;
			4'h6:
				K_360_NUM <= #Delay 8'd134;
			4'h7:
				K_360_NUM <= #Delay 8'd143;
			4'h8:
				K_360_NUM <= #Delay 8'd149;
			4'h9:
				K_360_NUM <= #Delay 8'd159;
			4'ha:
				K_360_NUM <= #Delay 8'd161;
			default:
				K_360_NUM <= #Delay 8'd0;
		endcase
	end
	else begin
		case (ldpc_mode)
			4'h0:
				K_360_NUM <= #Delay 8'd8;
			4'h1:
				K_360_NUM <= #Delay 8'd14;
			4'h2:
				K_360_NUM <= #Delay 8'd17;
			4'h3:
				K_360_NUM <= #Delay 8'd19;
			4'h4:
				K_360_NUM <= #Delay 8'd26;
			4'h5:
				K_360_NUM <= #Delay 8'd29;
			4'h6:
				K_360_NUM <= #Delay 8'd32;
			4'h7:
				K_360_NUM <= #Delay 8'd34;
			4'h8:
				K_360_NUM <= #Delay 8'd36;
			4'h9:
				K_360_NUM <= #Delay 8'd39;
			default:
				K_360_NUM <= #Delay 8'd0;
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		Q_360_NUM <= #Delay 8'b00000000;
	end
	else if(frame_mode == 0)begin
		case (ldpc_mode)
			4'h0:
				Q_360_NUM <= #Delay 8'd134;// NUM - 1
			4'h1:
				Q_360_NUM <= #Delay 8'd119;
			4'h2:
				Q_360_NUM <= #Delay 8'd107;
			4'h3:
				Q_360_NUM <= #Delay 8'd89;
			4'h4:
				Q_360_NUM <= #Delay 8'd71;
			4'h5:
				Q_360_NUM <= #Delay 8'd59;
			4'h6:
				Q_360_NUM <= #Delay 8'd44;
			4'h7:
				Q_360_NUM <= #Delay 8'd35;
			4'h8:
				Q_360_NUM <= #Delay 8'd29;
			4'h9:
				Q_360_NUM <= #Delay 8'd19;
			4'ha:
				Q_360_NUM <= #Delay 8'd17;
			default:
				Q_360_NUM <= #Delay 8'd0;
		endcase
	end
	else begin
		case (ldpc_mode)
			4'h0:
				Q_360_NUM <= #Delay 8'd35;
			4'h1:
				Q_360_NUM <= #Delay 8'd29;
			4'h2:
				Q_360_NUM <= #Delay 8'd26;
			4'h3:
				Q_360_NUM <= #Delay 8'd24;
			4'h4:
				Q_360_NUM <= #Delay 8'd17;
			4'h5:
				Q_360_NUM <= #Delay 8'd14;
			4'h6:
				Q_360_NUM <= #Delay 8'd11;
			4'h7:
				Q_360_NUM <= #Delay 8'd9;
			4'h8:
				Q_360_NUM <= #Delay 8'd7;
			4'h9:
				Q_360_NUM <= #Delay 8'd4;
			default:
				Q_360_NUM <= #Delay 8'd0;
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		h_matrix_offset_addr <= #Delay 13'h0000;
	end
	else begin
		case ({ldpc_mode,frame_mode})
			5'h00: h_matrix_offset_addr <= #Delay 13'h0000;
			5'h01: h_matrix_offset_addr <= #Delay 13'h013b;
			5'h02: h_matrix_offset_addr <= #Delay 13'h0183;
			5'h03: h_matrix_offset_addr <= #Delay 13'h0327;
			5'h04: h_matrix_offset_addr <= #Delay 13'h0390;
			5'h05: h_matrix_offset_addr <= #Delay 13'h0588;
			5'h06: h_matrix_offset_addr <= #Delay 13'h0606;
			5'h07: h_matrix_offset_addr <= #Delay 13'h0822;
			5'h08: h_matrix_offset_addr <= #Delay 13'h088b;
			5'h09: h_matrix_offset_addr <= #Delay 13'h0b7f;
			5'h0a: h_matrix_offset_addr <= #Delay 13'h0c3c;
			5'h0b: h_matrix_offset_addr <= #Delay 13'h0e94;
			5'h0c: h_matrix_offset_addr <= #Delay 13'h0f2a;
			5'h0d: h_matrix_offset_addr <= #Delay 13'h11cd;
			5'h0e: h_matrix_offset_addr <= #Delay 13'h125a;
			5'h0f: h_matrix_offset_addr <= #Delay 13'h152a;
			5'h10: h_matrix_offset_addr <= #Delay 13'h15b6;
			5'h11: h_matrix_offset_addr <= #Delay 13'h18a4;
			5'h12: h_matrix_offset_addr <= #Delay 13'h1942;
			5'h13: h_matrix_offset_addr <= #Delay 13'h1bd6;
			5'h14: h_matrix_offset_addr <= #Delay 13'h1c7b;
			default:h_matrix_offset_addr <= #Delay 13'h0000;
		endcase
	end
end	


always @(posedge sys_clk)begin
	if(~rst_n)begin
		order_period_cnt_for_rdaddr <= #Delay 13'd0 ;
		order_array_for_rdaddr <= #Delay 3'b000;
	end
	else if(fs_en == 1'b1)begin
		if(ram_rd_addr_gen_en_1dly)begin
				if(order_period_cnt_for_rdaddr == order_period)begin
					order_array_for_rdaddr <= #Delay order_array_for_rdaddr + 1;
					order_period_cnt_for_rdaddr <= #Delay 13'd0 ;
				end
				else begin
					order_period_cnt_for_rdaddr <= #Delay order_period_cnt_for_rdaddr + 13'd1 ;
				end
		end
		else begin
			order_array_for_rdaddr <= #Delay 3'b000;
			order_period_cnt_for_rdaddr <= #Delay 13'd0 ;
		end
	end
	else begin
	end
end


endmodule
