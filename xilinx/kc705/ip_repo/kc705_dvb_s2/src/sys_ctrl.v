`timescale 1ns / 1ps
module sys_ctrl(
input	          							sys_clk,
input											fs_en,
input	          							rst_n,
//////////////////////////////////////////////////////////////
input	          							start_en,

input	          			[1:0]			mod_mode,
input	          			[3:0] 		ldpc_mode,
input	          							frame_mode,
input   			 				    		pilot_mode,
//////////////////////////////////////////////////////////////
output  reg  								ldpc_ram_clr_vld,      // align with start_en_4dly  
output  reg  								ts_rd_head,            // align with start_en_4dly  
output  reg  								ts_rd_vld,             // align with start_en_4dly
output  reg  								bbscramb_init_vld,     // align with start_en_4dly
output  reg  								bbscramb_frame_vld,    // align with start_en_4dly
output  reg  								bit_itl_read_head,     // align with start_en_4dly
output  reg  								bit_itl_read_en,       // align with start_en_4dly
output  reg  								xfec_ready,            // align with start_en_4dly
output  reg  								null_vld,              // align with start_en_4dly
output  reg  								plscramb_init_vld,     // align with start_en_4dly
output  reg  								plscramb_vld,          // align with start_en_4dly
output  reg  								pl_pilot_vld,          // align with start_en_4dly
output  reg  								plheader_head,         // align with start_en_4dly
output  reg  								pl_header_vld          // align with start_en_4dly
//////////////////////////////////////////////////////////////
);

parameter NumSlot_Cycles = 90;
parameter NumPilot_Cycles = 36;
parameter Dummy_Frm_Cycles = 3330;

reg 						[12:0] 			Kbch_Byte_Num;
reg 						[7:0] 			LDPC_Ram_Clr_Cycles;
//reg 						[1:0] 			Dummy_Frm_Num;
reg 						[15:0] 			PL_Bytes_Num_Frm;
reg 						[12:0] 			TS_Bytes_Num_Frm;


always @(posedge sys_clk)begin
	if(~rst_n)begin
		Kbch_Byte_Num <= 13'd0;
		TS_Bytes_Num_Frm <= 13'd0;
	end
	else begin
		case({frame_mode,ldpc_mode})
		5'h00:begin
			Kbch_Byte_Num <= 13'd2001;
			TS_Bytes_Num_Frm <= 13'd1991;
		end
		5'h01:begin
			Kbch_Byte_Num <= 13'd2676;
			TS_Bytes_Num_Frm <= 13'd2666;
		end
		5'h02:begin
			Kbch_Byte_Num <= 13'd3216;
			TS_Bytes_Num_Frm <= 13'd3206;
		end
		5'h03:begin
			Kbch_Byte_Num <= 13'd4026;
			TS_Bytes_Num_Frm <= 13'd4016;
		end
		5'h04:begin
			Kbch_Byte_Num <= 13'd4836;
			TS_Bytes_Num_Frm <= 13'd4826;
		end
		5'h05:begin
			Kbch_Byte_Num <= 13'd5380;
			TS_Bytes_Num_Frm <= 13'd5370;
		end
		5'h06:begin
			Kbch_Byte_Num <= 13'd6051;
			TS_Bytes_Num_Frm <= 13'd6041;
		end
		5'h07:begin
			Kbch_Byte_Num <= 13'd6456;
			TS_Bytes_Num_Frm <= 13'd6446;
		end
		5'h08:begin
			Kbch_Byte_Num <= 13'd6730;
			TS_Bytes_Num_Frm <= 13'd6720;
		end
		5'h09:begin
			Kbch_Byte_Num <= 13'd7184;
			TS_Bytes_Num_Frm <= 13'd7174;
		end
		5'h0a:begin
			Kbch_Byte_Num <= 13'd7274;
			TS_Bytes_Num_Frm <= 13'd7264;
		end
		5'h10:begin
			Kbch_Byte_Num <= 13'd384;
			TS_Bytes_Num_Frm <= 13'd374;
		end
		5'h11:begin
			Kbch_Byte_Num <= 13'd654;
			TS_Bytes_Num_Frm <= 13'd644;
		end
		5'h12:begin
			Kbch_Byte_Num <= 13'd789;
			TS_Bytes_Num_Frm <= 13'd779;
		end
		5'h13:begin
			Kbch_Byte_Num <= 13'd879;
			TS_Bytes_Num_Frm <= 13'd869;
		end
		5'h14:begin
			Kbch_Byte_Num <= 13'd1194;
			TS_Bytes_Num_Frm <= 13'd1184;
		end
		5'h15:begin
			Kbch_Byte_Num <= 13'd1329;
			TS_Bytes_Num_Frm <= 13'd1319;
		end
		5'h16:begin
			Kbch_Byte_Num <= 13'd1464;
			TS_Bytes_Num_Frm <= 13'd1454;
		end
		5'h17:begin
			Kbch_Byte_Num <= 13'd1554;
			TS_Bytes_Num_Frm <= 13'd1544;
		end
		5'h18:begin
			Kbch_Byte_Num <= 13'd1644;
			TS_Bytes_Num_Frm <= 13'd1634;
		end
		5'h19:begin
			Kbch_Byte_Num <= 13'd1779;
			TS_Bytes_Num_Frm <= 13'd1769;
		end
		default:begin
			Kbch_Byte_Num <= 13'd0;
			TS_Bytes_Num_Frm <= 13'd0;
		end
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		PL_Bytes_Num_Frm <= 16'd0;
	end
	else begin
		case({frame_mode,pilot_mode,mod_mode})
		4'h0:begin
			PL_Bytes_Num_Frm <= 16'd32490;
		end
		4'h1:begin
			PL_Bytes_Num_Frm <= 16'd21690;
		end
		4'h2:begin
			PL_Bytes_Num_Frm <= 16'd16290;
		end
		4'h3:begin
			PL_Bytes_Num_Frm <= 16'd13050;
		end
		4'h4:begin
			PL_Bytes_Num_Frm <= 16'd33282;
		end
		4'h5:begin
			PL_Bytes_Num_Frm <= 16'd22194;
		end
		4'h6:begin
			PL_Bytes_Num_Frm <= 16'd16686;
		end
		4'h7:begin
			PL_Bytes_Num_Frm <= 16'd13338;
		end
		4'h8:begin
			PL_Bytes_Num_Frm <= 16'd8190;
		end
		4'h9:begin
			PL_Bytes_Num_Frm <= 16'd5490;
		end
		4'ha:begin
			PL_Bytes_Num_Frm <= 16'd4140;
		end
		4'hb:begin
			PL_Bytes_Num_Frm <= 16'd3330;
		end
		4'hc:begin
			PL_Bytes_Num_Frm <= 16'd8370;
		end
		4'hd:begin
			PL_Bytes_Num_Frm <= 16'd5598;
		end
		4'he:begin
			PL_Bytes_Num_Frm <= 16'd4212;
		end
		4'hf:begin
			PL_Bytes_Num_Frm <= 16'd3402;
		end
		default:begin
			PL_Bytes_Num_Frm <= 16'd0;
		end
		endcase
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		LDPC_Ram_Clr_Cycles <= 8'd0;
		//Dummy_Frm_Num <= 2'd0;
	end
	else begin
		case(frame_mode)
		1'b0:begin
			LDPC_Ram_Clr_Cycles <= 8'd136;
			//Dummy_Frm_Num <= 2'd3;
		end
		1'b1:begin
			LDPC_Ram_Clr_Cycles <= 8'd37;
			//Dummy_Frm_Num <= 2'd1;
		end
		default:begin
			LDPC_Ram_Clr_Cycles <= 8'd0;
			//Dummy_Frm_Num <= 2'd0;
		end
		endcase
	end
end

reg  								ldpc_ram_clr_vld_reg;    // align with start_en_1dly
reg  								ts_rd_head_reg;          // align with start_en_1dly
reg  								ts_rd_vld_reg;           // align with start_en_1dly
reg  								bbscramb_init_vld_reg;   // align with start_en_1dly
reg  								bbscramb_frame_vld_reg;  // align with start_en_1dly
reg  								bit_itl_read_head_reg;   // align with start_en_3dly
reg  								bit_itl_read_en_reg;     // align with start_en_2dly 
reg  								xfec_ready_reg;          // align with start_en_2dly 
reg  								null_vld_reg;            // align with start_en_2dly  
reg  								plscramb_init_vld_reg;   // align with start_en_3dly
reg  								plscramb_vld_reg;        // align with start_en_2dly  
reg  								pl_pilot_vld_reg;        // align with start_en_1dly  
reg  								plheader_head_reg;       // align with start_en_1dly 
reg  								pl_header_vld_reg;       // align with start_en_1dly  


reg  								ldpc_ram_clr_vld_reg_1dly;    // align with start_en_2dly
reg  								ts_rd_head_reg_1dly;          // align with start_en_2dly
reg  								ts_rd_vld_reg_1dly;           // align with start_en_2dly
reg  								bbscramb_init_vld_reg_1dly;   // align with start_en_2dly
reg  								bbscramb_frame_vld_reg_1dly;  // align with start_en_2dly

reg  								ldpc_ram_clr_vld_reg_2dly;    // align with start_en_3dly
reg  								ts_rd_head_reg_2dly;          // align with start_en_3dly
reg  								ts_rd_vld_reg_2dly;           // align with start_en_3dly
reg  								bbscramb_init_vld_reg_2dly;   // align with start_en_3dly
reg  								bbscramb_frame_vld_reg_2dly;  // align with start_en_3dly

//reg  								bit_itl_read_head_reg;   // align with start_en_3dly
reg  								bit_itl_read_en_reg_1dly;     // align with start_en_3dly 
reg  								xfec_ready_reg_1dly;          // align with start_en_3dly 
reg  								null_vld_reg_1dly;            // align with start_en_3dly  

//reg  								plscramb_init_vld_reg;   // align with start_en_3dly
reg  								plscramb_vld_reg_1dly;        // align with start_en_3dly    

reg  								pl_pilot_vld_reg_1dly;        // align with start_en_2dly  
reg  								plheader_head_reg_1dly;       // align with start_en_2dly 
reg  								pl_header_vld_reg_1dly;       // align with start_en_2dly 

reg  								pl_pilot_vld_reg_2dly;        // align with start_en_3dly 
reg  								plheader_head_reg_2dly;       // align with start_en_3dly 
reg  								pl_header_vld_reg_2dly;       // align with start_en_3dly 

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ldpc_ram_clr_vld_reg_1dly<= 1'b0;    // align with start_en_2dly  
		ts_rd_head_reg_1dly<= 1'b0;          // align with start_en_2dly  
		ts_rd_vld_reg_1dly<= 1'b0;           // align with start_en_2dly  
		bbscramb_init_vld_reg_1dly<= 1'b0;   // align with start_en_2dly  
		bbscramb_frame_vld_reg_1dly<= 1'b0;  // align with start_en_2dly  
		                                                           
		ldpc_ram_clr_vld_reg_2dly<= 1'b0;    // align with start_en_3dly  
		ts_rd_head_reg_2dly<= 1'b0;          // align with start_en_3dly  
		ts_rd_vld_reg_2dly<= 1'b0;           // align with start_en_3dly  
		bbscramb_init_vld_reg_2dly<= 1'b0;   // align with start_en_3dly  
		bbscramb_frame_vld_reg_2dly<= 1'b0;  // align with start_en_3dly  
		                                                           
		//	bit_itl_read_head_reg<= 1'b0;   // align with start_en_3dly     
		bit_itl_read_en_reg_1dly<= 1'b0;     // align with start_en_3dly  
		xfec_ready_reg_1dly<= 1'b0;          // align with start_en_3dly  
		null_vld_reg_1dly<= 1'b0;            // align with start_en_3dly  
		                                                           	                                                           
		//	plscramb_init_vld_reg<= 1'b0;   // align with start_en_3dly     
		plscramb_vld_reg_1dly<= 1'b0;        // align with start_en_3dly  
		                                                           
		pl_pilot_vld_reg_1dly<= 1'b0;        // align with start_en_2dly  
		plheader_head_reg_1dly<= 1'b0;       // align with start_en_2dly  
		pl_header_vld_reg_1dly<= 1'b0;       // align with start_en_2dly  
		                                                           
		pl_pilot_vld_reg_2dly<= 1'b0;        // align with start_en_3dly  
		plheader_head_reg_2dly<= 1'b0;       // align with start_en_3dly  
		pl_header_vld_reg_2dly<= 1'b0;       // align with start_en_3dly  

		ldpc_ram_clr_vld	<= 1'b0;  
		ts_rd_head	<= 1'b0;        
		ts_rd_vld	<= 1'b0;         
		bbscramb_init_vld	<= 1'b0; 
		bbscramb_frame_vld	<= 1'b0;
		bit_itl_read_head	<= 1'b0;
		bit_itl_read_en	<= 1'b0; 
		xfec_ready	<= 1'b0;       
		null_vld	<= 1'b0;         
		plscramb_init_vld	<= 1'b0; 
		plscramb_vld	<= 1'b0;     
		pl_pilot_vld	<= 1'b0;      
		plheader_head	<= 1'b0;     
		pl_header_vld <= 1'b0;     
	end
	else if(fs_en == 1'b1)begin
		ldpc_ram_clr_vld_reg_1dly<= ldpc_ram_clr_vld_reg;    // align with start_en_2dly  
		ts_rd_head_reg_1dly<= ts_rd_head_reg;          // align with start_en_2dly  
		ts_rd_vld_reg_1dly<= ts_rd_vld_reg;           // align with start_en_2dly  
		bbscramb_init_vld_reg_1dly<= bbscramb_init_vld_reg;   // align with start_en_2dly  
		bbscramb_frame_vld_reg_1dly<= bbscramb_frame_vld_reg;  // align with start_en_2dly  
		                                                           
		ldpc_ram_clr_vld_reg_2dly<= ldpc_ram_clr_vld_reg_1dly;    // align with start_en_3dly  
		ts_rd_head_reg_2dly<= ts_rd_head_reg_1dly;          // align with start_en_3dly  
		ts_rd_vld_reg_2dly<= ts_rd_vld_reg_1dly;           // align with start_en_3dly  
		bbscramb_init_vld_reg_2dly<= bbscramb_init_vld_reg_1dly;   // align with start_en_3dly  
		bbscramb_frame_vld_reg_2dly<= bbscramb_frame_vld_reg_1dly;  // align with start_en_3dly  
		                                                           
		//	bit_itl_read_head_reg<= 1'b0;   // align with start_en_3dly     
		bit_itl_read_en_reg_1dly<= bit_itl_read_en_reg;     // align with start_en_3dly  
		xfec_ready_reg_1dly<= xfec_ready_reg;          // align with start_en_3dly  
		null_vld_reg_1dly<= null_vld_reg;            // align with start_en_3dly  
		                                                           	                                                           
		//	plscramb_init_vld_reg<= 1'b0;   // align with start_en_3dly     
		plscramb_vld_reg_1dly<= plscramb_vld_reg;        // align with start_en_3dly  
		                                                           
		pl_pilot_vld_reg_1dly<= pl_pilot_vld_reg;        // align with start_en_2dly  
		plheader_head_reg_1dly<= plheader_head_reg;       // align with start_en_2dly  
		pl_header_vld_reg_1dly<= pl_header_vld_reg;       // align with start_en_2dly  
		                                                           
		pl_pilot_vld_reg_2dly<= pl_pilot_vld_reg_1dly;        // align with start_en_3dly  
		plheader_head_reg_2dly<= plheader_head_reg_1dly;       // align with start_en_3dly  
		pl_header_vld_reg_2dly<= pl_header_vld_reg_1dly;       // align with start_en_3dly  

		ldpc_ram_clr_vld	<= ldpc_ram_clr_vld_reg_2dly;  
		ts_rd_head	<= ts_rd_head_reg_2dly;        
		ts_rd_vld	<= ts_rd_vld_reg_2dly;         
		bbscramb_init_vld	<= bbscramb_init_vld_reg_2dly; 
		bbscramb_frame_vld	<= bbscramb_frame_vld_reg_2dly;
		bit_itl_read_head	<= bit_itl_read_head_reg;
		bit_itl_read_en	<= bit_itl_read_en_reg_1dly; 
		xfec_ready	<= xfec_ready_reg_1dly;       
		null_vld	<= null_vld_reg_1dly;         
		plscramb_init_vld	<= plscramb_init_vld_reg; 
		plscramb_vld	<= plscramb_vld_reg_1dly;     
		pl_pilot_vld	<= pl_pilot_vld_reg_2dly;      
		plheader_head	<= plheader_head_reg_2dly;     
		pl_header_vld <= pl_header_vld_reg_2dly; 
	end  
	else begin
	end
end
    
reg  								start_en_1dly;    
//reg  								plscramb_vld_reg_1dly;    
//reg  								bit_itl_read_en_reg_1dly;    
    
always @(posedge sys_clk)begin
	if(~rst_n)begin
		start_en_1dly					<= 1'b0;           
//		plscramb_vld_reg_1dly	<= 1'b0;   
//		bit_itl_read_en_reg_1dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin
			start_en_1dly					<= start_en;           
//			plscramb_vld_reg_1dly	<= plscramb_vld_reg;   
//			bit_itl_read_en_reg_1dly <= bit_itl_read_en_reg;
	end
	else begin
	end
end


reg								[15:0]						Symbols_Cnt_Frm_reg;	// align with start_en
reg								[15:0]						Out_Symbols_Cnt_Frm_reg;	// align with start_en

reg																	dummy_flag_reg;// align with start_en_1dly

always @(posedge sys_clk)begin
	if(~rst_n)begin
		dummy_flag_reg					<= 1'b0;           
	end
	else if(fs_en == 1'b1)begin
		if((start_en == 1'b1)&&(start_en_1dly == 1'b0))begin
		dummy_flag_reg					<= 1'b1;   
		end
		else if((Symbols_Cnt_Frm_reg == 16'h00)&&(start_en_1dly == 1'b1))begin
			dummy_flag_reg					<= 1'b0;  
		end
		else if(start_en == 1'b0)begin
			dummy_flag_reg					<= 1'b0;  
		end
	end
	else begin
	end
end

reg									[15:0]						Dummy_Frm_Cycles_M_Num; 
reg									[15:0]						Out_Dummy_Frm_Cycles_M_Num;
// modified by wanjin 2014.08.17
always @(posedge sys_clk)begin
	if(~rst_n)begin
		Dummy_Frm_Cycles_M_Num <= 16'd0;
		Out_Dummy_Frm_Cycles_M_Num <= 16'd0;
	end
	else if(fs_en == 1'b1)begin
		Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
		Out_Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
//		case({dummy_flag_reg,frame_mode})
//		2'b00:begin
//			Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
//			Out_Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
//		end
//		2'b01:begin
//			Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
//			Out_Dummy_Frm_Cycles_M_Num <= PL_Bytes_Num_Frm;
//		end
//		2'b10:begin
//			Dummy_Frm_Cycles_M_Num <= 9990;
//			Out_Dummy_Frm_Cycles_M_Num <= 3330;
//		end
//		2'b11:begin
//			Dummy_Frm_Cycles_M_Num <= 3330;
//			Out_Dummy_Frm_Cycles_M_Num <= 3330;
//		end
//		default:begin
//			Dummy_Frm_Cycles_M_Num <= 16'd0;
//			Out_Dummy_Frm_Cycles_M_Num <= 16'd0;
//		end
//		endcase
	end
	else begin
	end
end


always @(posedge sys_clk)begin
	if(~rst_n)begin
		Symbols_Cnt_Frm_reg					<= 16'h0000;           
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if(Symbols_Cnt_Frm_reg == Dummy_Frm_Cycles_M_Num - 1)begin
				Symbols_Cnt_Frm_reg					<= 16'h0000;  
			end
			else begin
				Symbols_Cnt_Frm_reg					<= Symbols_Cnt_Frm_reg + 16'h0001;  
			end
		end
		else begin
			 Symbols_Cnt_Frm_reg					<= 16'h0000; 
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		Out_Symbols_Cnt_Frm_reg					<= 16'h0000;           
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if(Out_Symbols_Cnt_Frm_reg == Out_Dummy_Frm_Cycles_M_Num - 1)begin
				Out_Symbols_Cnt_Frm_reg					<= 16'h0000;  
			end
			else begin
				Out_Symbols_Cnt_Frm_reg					<= Out_Symbols_Cnt_Frm_reg + 16'h0001;  
			end
		end
		else begin
			 Out_Symbols_Cnt_Frm_reg					<= 16'h0000; 
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ldpc_ram_clr_vld_reg					<= 1'b0;  // align with start_en_1dly         
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Symbols_Cnt_Frm_reg >= 0)&&(Symbols_Cnt_Frm_reg < LDPC_Ram_Clr_Cycles))begin
				ldpc_ram_clr_vld_reg					<= 1'b1;    
			end
			else begin
				ldpc_ram_clr_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 ldpc_ram_clr_vld_reg					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ts_rd_head_reg					<= 1'b0;          // align with start_en_1dly 
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Symbols_Cnt_Frm_reg == LDPC_Ram_Clr_Cycles))begin
				ts_rd_head_reg					<= 1'b1;    
			end
			else begin
				ts_rd_head_reg					<= 1'b0;     
			end
		end
		else begin
			 ts_rd_head_reg					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		ts_rd_vld_reg					<= 1'b0;           // align with start_en_1dly
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Symbols_Cnt_Frm_reg >= 0 + LDPC_Ram_Clr_Cycles)&&(Symbols_Cnt_Frm_reg < LDPC_Ram_Clr_Cycles + TS_Bytes_Num_Frm))begin
				ts_rd_vld_reg					<= 1'b1;    
			end
			else begin
				ts_rd_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 ts_rd_vld_reg					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bbscramb_init_vld_reg					<= 1'b0;       // align with start_en_1dly    
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Symbols_Cnt_Frm_reg == LDPC_Ram_Clr_Cycles))begin
				bbscramb_init_vld_reg					<= 1'b1;    
			end
			else begin
				bbscramb_init_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 bbscramb_init_vld_reg					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bbscramb_frame_vld_reg					<= 1'b0;        // align with start_en_1dly   
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Symbols_Cnt_Frm_reg >= 0 + LDPC_Ram_Clr_Cycles)&&(Symbols_Cnt_Frm_reg < LDPC_Ram_Clr_Cycles + Kbch_Byte_Num))begin
				bbscramb_frame_vld_reg					<= 1'b1;    
			end
			else begin
				bbscramb_frame_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 bbscramb_frame_vld_reg 					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		plheader_head_reg					<= 1'b0;           // align with start_en_1dly
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Out_Symbols_Cnt_Frm_reg == 0))begin
				plheader_head_reg					<= 1'b1;    
			end
			else begin
				plheader_head_reg					<= 1'b0;     
			end
		end
		else begin
			 plheader_head_reg					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		pl_header_vld_reg					<= 1'b0;           // align with start_en_1dly
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if((Out_Symbols_Cnt_Frm_reg >= 0)&&(Out_Symbols_Cnt_Frm_reg < NumSlot_Cycles))begin
				pl_header_vld_reg					<= 1'b1;    
			end
			else begin
				pl_header_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 pl_header_vld_reg 					<= 1'b0;   
		end
	end
	else begin
	end
end

reg								[15:0]						pl_pilot_vld_lowth;
reg								[15:0]						pl_pilot_vld_highth;

always @(posedge sys_clk)begin
	if(~rst_n)begin
		pl_pilot_vld_lowth					<= 16'd1530;           // align with start_en_2dly
		pl_pilot_vld_highth					<= 16'd1566;           // align with start_en_2dly
	end
	else if(fs_en == 1'b1)begin
		if(start_en == 1'b1)begin
			if(Out_Symbols_Cnt_Frm_reg == 0)begin
			  pl_pilot_vld_lowth					<= 16'd1530;           // align with start_en_2dly
			  pl_pilot_vld_highth					<= 16'd1566;           // align with start_en_2dly  
			end		
			else if((dummy_flag_reg == 1'b0)&&(pilot_mode == 1'b1)&&(Out_Symbols_Cnt_Frm_reg == pl_pilot_vld_highth))begin
				pl_pilot_vld_lowth        <= pl_pilot_vld_lowth + 1476;
				pl_pilot_vld_highth					<= pl_pilot_vld_highth + 1476;    
			end
		end
		else begin //if(Out_Symbols_Cnt_Frm_reg == 0)begin
			  pl_pilot_vld_lowth					<= 16'd1530;           // align with start_en_2dly
			  pl_pilot_vld_highth					<= 16'd1566;           // align with start_en_2dly  
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		pl_pilot_vld_reg					<= 1'b0;           // align with start_en_1dly
	end
	else if(fs_en == 1'b1)begin
		if((dummy_flag_reg == 1'b0)&&(pilot_mode == 1'b1)&&(start_en == 1'b1))begin
			if((Out_Symbols_Cnt_Frm_reg >= pl_pilot_vld_lowth)&&(Out_Symbols_Cnt_Frm_reg < pl_pilot_vld_highth))begin
				pl_pilot_vld_reg					<= 1'b1;    
			end
			else begin
				pl_pilot_vld_reg					<= 1'b0;     
			end
		end
		else begin
			 pl_pilot_vld_reg 					<= 1'b0;   
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		null_vld_reg <= 1'b0;  // align with start_en_2dly
		plscramb_vld_reg <= 1'b0;// align with start_en_2dly
		xfec_ready_reg <= 0;// align with start_en_2dly
		bit_itl_read_en_reg <= 0;// align with start_en_2dly
	end
	else if(fs_en == 1'b1)begin
			 if(start_en_1dly)begin
				if(dummy_flag_reg == 1)begin
					if(pl_header_vld_reg == 0)begin
						null_vld_reg <= 1;
						plscramb_vld_reg <= 1;
				  end
				  else begin
						null_vld_reg <= 0;
						plscramb_vld_reg <= 0;
				  end
				  xfec_ready_reg <= 0;
				  bit_itl_read_en_reg <= 0;
				end
				else begin
						xfec_ready_reg <= 1;
				  null_vld_reg <= 0;
				  if(pl_header_vld_reg == 1)begin
						bit_itl_read_en_reg <= 0;
						plscramb_vld_reg <= 0;
				  end
				  else if(pl_pilot_vld_reg == 1)begin
						bit_itl_read_en_reg <= 0;
						plscramb_vld_reg <= 1;
				  end
				  else begin
						bit_itl_read_en_reg <= 1;
						plscramb_vld_reg <= 1;
				  end
				end
		end
		else begin
			null_vld_reg <= 1'b0;  // align with start_en_2dly
		 plscramb_vld_reg <= 1'b0;// align with start_en_2dly
		 xfec_ready_reg <= 0;// align with start_en_2dly
		 bit_itl_read_en_reg <= 0;// align with start_en_2dly
		end
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~rst_n)begin
		bit_itl_read_head_reg <= 0; // align with start_en_3dly
		plscramb_init_vld_reg <= 0; // align with start_en_3dly
	end
	else if(fs_en == 1'b1)begin
		bit_itl_read_head_reg <= (xfec_ready_reg == 1)&&(bit_itl_read_en_reg == 1)&&(bit_itl_read_en_reg_1dly == 0)&&(pl_header_vld_reg_2dly == 1);
		plscramb_init_vld_reg <= (plscramb_vld_reg == 1)&&(plscramb_vld_reg_1dly == 0);
	end
	else begin
	end
end


endmodule
