
`timescale 1ns / 1ps
module dvb_s2_system_tb();

reg	          							hard_rst_n;
//////////////////////////////////////////////////////////////
reg	          			[1:0]			mod_mode_cfg;
reg	          			[3:0] 		ldpc_mode_cfg;
reg	          							frame_mode_cfg;
reg   			 				    		pilot_mode_cfg;
reg							[1:0]			srrc_mode;//00:0.35; 01:0.25; 10:0.20 
reg							[2:0]			dvb_s_convolution_mode;
reg   			 				    		dvb_s_mode;// 0:dvb-s; 1:dvb-s2    
reg   			 				    		fs_en_switch;  //0:from outer;1:from inner                      
//reg							[1:0]			SYS_Baud_mode;
reg							[1:0]			TS_Source_mode;
reg							[31:0]		    SYS_Baud_Num;//32'd2500 --> 25M BaudRate   SYS_Baud_mode,// 00:10M; 01:25M; 
reg											Freq_Inv_mode;// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);
//////////////////////////////////////////////////////////////
reg                  					sys_clk;
reg							[7:0]       ts_din;// @ sys_clk
reg                  					ts_syn;// @ sys_clk
reg                  					ts_head;// @ sys_clk
wire	               					ts_clk;// @ sys_clk
reg                 						fs_en_outer;
reg                						fs_en2_outer;

wire                 						fs_en;
wire                						fs_en2;

wire                 						fs_en_inner;
wire                						fs_en2_inner;

wire				[7:0]       ts_din_tp;// @ sys_clk
wire         					ts_syn_tp;// @ sys_clk
wire         					ts_head_tp;// @ sys_clk
      
wire													symbol_1x_oe;             
wire				signed [15:0]					symbol_1x_re_out;  
wire				signed [15:0]					symbol_1x_im_out;  

//wire													symbol_2x_oe;             
//wire				signed [15:0]					symbol_2x_re_out;  
//wire				signed [15:0]					symbol_2x_im_out;  


wire                  							ts_clk_h264out;// @ ts_clk_out

assign	ts_clk_h264out = sys_clk;

dvb_s2_system_top uut(
.hard_rst_n				(hard_rst_n),
//////////////////////////////////////////////////////////////
.mod_mode_cfg			(mod_mode_cfg),
.ldpc_mode_cfg			(ldpc_mode_cfg),
.frame_mode_cfg		(frame_mode_cfg),
.pilot_mode_cfg		(pilot_mode_cfg),
.srrc_mode				(srrc_mode),//00:0.35; 01:0.25; 10:0.20 
.dvb_s_convolution_mode (dvb_s_convolution_mode),
.dvb_s_mode            (dvb_s_mode),
//.SYS_Baud_mode			(SYS_Baud_mode),
.TS_Source_mode		(TS_Source_mode),
.SYS_Baud_Num       (SYS_Baud_Num),
.Freq_Inv_mode      (Freq_Inv_mode),
//////////////////////////////////////////////////////////////
.sys_clk					(sys_clk),
.ts_din_h264out		(ts_din),// @ ts_clk_h264out
.ts_valid_h264out		(ts_syn),// @ ts_clk_h264out
.ts_syn_h264out		(ts_head),// @ ts_clk_h264out
.ts_clk_h264out		(ts_clk_h264out),// @ ts_clk_h264out
.ts_clk					(ts_clk),

.fs_en_switch               (fs_en_switch),
.fs_en_outer					(fs_en_outer),
.fs_en2_outer					(fs_en2_outer),
.ts_din                 (ts_din_tp),
.ts_syn                 (ts_syn_tp),
.ts_head                (ts_head_tp),
.fs_en_inner            (fs_en_inner),
.fs_en2_inner           (fs_en2_inner),
//////////////////////////////////////////////////////////////
.symbol_1x_oe			(symbol_1x_oe),
.symbol_1x_re_out		(symbol_1x_re_out),
.symbol_1x_im_out		(symbol_1x_im_out)//,
//.symbol_2x_oe			(symbol_2x_oe),
//.symbol_2x_re_out		(symbol_2x_re_out),
//.symbol_2x_im_out		(symbol_2x_im_out)
);

wire glb_rst_n;

assign  glb_rst_n = hard_rst_n;


reg							[7:0]       ts_din_1x;// @ fs_en
reg                  					ts_syn_1x;// @ fs_en
reg                  					ts_head_1x;// @ fs_en

//dvb_s2_system_top uut_1x(
//.glb_rst_n				(glb_rst_n),
////////////////////////////////////////////////////////////////
//.mod_mode_cfg			(mod_mode_cfg),
//.ldpc_mode_cfg			(ldpc_mode_cfg),
//.frame_mode_cfg		(frame_mode_cfg),
//.pilot_mode_cfg		(pilot_mode_cfg),
//.srrc_mode				(srrc_mode),//00:0.35; 01:0.25; 10:0.20 
////////////////////////////////////////////////////////////////
//.sys_clk					(fs_en),
//.ts_din					(ts_din_1x),// @ sys_clk
//.ts_syn					(ts_syn_1x),// @ sys_clk
//.ts_head					(ts_head_1x),// @ sys_clk
//.ts_clk					(ts_clk),// @ sys_clk
//.fs_en					(1'b1),
//.fs_en2					(fs_en2),
////////////////////////////////////////////////////////////////
//.symbol_1x_oe			(),
//.symbol_1x_re_out		(),
//.symbol_1x_im_out		(),
//
//.symbol_2x_oe			(),
//.symbol_2x_re_out		(),
//.symbol_2x_im_out		()
//);


parameter  N_len = 902300;
parameter  CRCEncoder_In_Bits_Len = 6040064;

reg 						[12:0] 			Kbch_Byte_Num;
reg 						[15:0] 			PL_Bytes_Num_Frm;
reg 						[12:0] 			TS_Bytes_Num_Frm;



always @(posedge sys_clk or negedge glb_rst_n)begin
	if(~glb_rst_n)begin
		Kbch_Byte_Num <= 13'd0;
		TS_Bytes_Num_Frm <= 13'd0;
	end
	else begin
		case({frame_mode_cfg,ldpc_mode_cfg})
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

always @(posedge sys_clk or negedge glb_rst_n)begin
	if(~glb_rst_n)begin
		PL_Bytes_Num_Frm <= 16'd0;
	end
	else begin
		case({frame_mode_cfg,pilot_mode_cfg,mod_mode_cfg})
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


reg 	CRCEncoder_In_Bits[CRCEncoder_In_Bits_Len - 1:0];

initial $readmemb("../../../CRCEncoder_In.txt",CRCEncoder_In_Bits);

initial // Clock generator
  begin
    sys_clk = 1;
    forever #15 sys_clk = !sys_clk;
  end		
		
initial // Clock generator
  begin
    fs_en_outer = 0;
    fs_en2_outer = 0;
    forever begin
    #30 fs_en_outer = 1; fs_en2_outer = 1;
    #30 fs_en_outer = 0; fs_en2_outer = 0;
	 #30 fs_en_outer = 0; fs_en2_outer = 1;
    #30 fs_en_outer = 0; fs_en2_outer = 0;
    //#10 fs_en = 0; fs_en2 = 0;
  end
  end			
//initial // Clock generator
//  begin
//    sys_clk = 1;
//    forever #15 sys_clk = !sys_clk;
//  end		
//  
//initial // Clock generator
//  begin
//    fs_en = 0;
//	 fs_en2 = 0;
//    forever begin
//    fs_en = 0; //fs_en2 = 1;
//    #30 fs_en = 1; //fs_en2 = 0;
//	 #30 fs_en = 0; //fs_en2 = 1;
//	 #180 fs_en = 0; //fs_en2 = 0;
//  end
//  end	 
		
initial	// Test stimulus
  begin
   // fs_en = 0;
    //fs_en2 = 0;
  	mod_mode_cfg   	= 2'b00;
  	ldpc_mode_cfg  	= 4'b0110;
  	frame_mode_cfg 	= 1'b0;
  	pilot_mode_cfg 	= 1'b0;
  	srrc_mode  	= 2'b10;
  	dvb_s_convolution_mode = 3'b000;
  	dvb_s_mode = 1'b1;
	//SYS_Baud_mode = 2'b00;//2'b10;
	TS_Source_mode = 2'b01;
	SYS_Baud_Num  = 32'd2500;
	Freq_Inv_mode = 1'b0;
	fs_en_switch = 0;//0:from outer;1:from inner
   hard_rst_n 	= 1;	 
	//ts_clk_h264out = 0;
#120 hard_rst_n = 0;
#120 hard_rst_n = 1;
  end
  
 assign    fs_en = (fs_en_switch == 1)?fs_en_inner:fs_en_outer;
 assign    fs_en2 = (fs_en_switch == 1)?fs_en2_inner:fs_en2_outer; 
  
//always @(posedge sys_clk)begin
//      if(~hard_rst_n)begin
//          fs_en <= 0;
//          fs_en2 <= 0;
//      end
//      else if(fs_en_switch == 1'b1)begin
//              fs_en <= fs_en_inner;
//              fs_en2 <= fs_en2_inner;
//      end
//      else begin
//         fs_en <= fs_en_outer;
//         fs_en2 <= fs_en2_outer;
//      end
//  end


  
  wire			[31:0]		fs_cnt;
  reg			[31:0]		fs_phase;
  
  assign 	fs_cnt		= (fs_en == 1)?(fs_phase+1):fs_phase;
  
  
  reg	[31:0]	PL_Bytes_Num_Frm_MUX;
  
   always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		fs_phase <= 32'h0;
		PL_Bytes_Num_Frm_MUX <= 32'd9990;
	end
	else if(fs_en)begin
		if(fs_cnt >= PL_Bytes_Num_Frm_MUX)begin
			fs_phase <= fs_cnt - PL_Bytes_Num_Frm_MUX;
			PL_Bytes_Num_Frm_MUX <= PL_Bytes_Num_Frm;
		end
		else begin
			fs_phase <= fs_cnt;
		end
	end
	end
  
  reg					bbscramb_frame_vld;
  
    always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
	  bbscramb_frame_vld <= 0;
	end
	else if((fs_phase <= Kbch_Byte_Num)&&(fs_phase > 0))begin
		bbscramb_frame_vld <= 1;
	end
	else begin
		bbscramb_frame_vld <= 0;
	end
end
  
  reg 				ts_rd_vld;
  
  always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		ts_rd_vld <= 0;
	end
	else if((fs_phase <= TS_Bytes_Num_Frm)&&(fs_phase > 0))begin
		ts_rd_vld <= 1;
	end
	else begin
		ts_rd_vld <= 0;
	end
end
  
  
 wire			[31:0]		ts_cnt;
  reg			[31:0]		ts_phase;
  
  assign 	ts_cnt		= (ts_clk == 1)?(ts_phase+1):ts_phase;
  
   always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		ts_phase <= 32'h0;
	end
	else if(ts_clk)begin
		if(ts_cnt >= 188)
			ts_phase <= ts_cnt - 188;
		else begin
			ts_phase <= ts_cnt;
		end
	end
	end 
  
  
  always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		ts_head <= 1'b0;
		ts_syn 	<= 0;
	end
	else if(ts_clk)begin
		ts_syn 	<= 1;
		if(ts_phase == 0)begin
			ts_head <= 1;
		end
		else begin
			ts_head <= 1'b0;
		end
	end
	else begin
		ts_head <= 1'b0;
		ts_syn 	<= 0;
	end
	end 
	
	
	  always @(posedge fs_en or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		ts_head_1x <= 1'b0;
		ts_syn_1x 	<= 0;
	end
	else begin
		ts_syn_1x 	<= ts_rd_vld;
		if(ts_phase == 0)begin
			ts_head_1x <= ts_rd_vld;
		end
		else begin
			ts_head_1x <= 1'b0;
		end
	end
	end 
  
  reg [31:0] input_cnt;
//  reg							[7:0]       ts_din;// @ sys_clk
//  reg                  					ts_syn;// @ sys_clk
//  reg                  					ts_head;// @ sys_clk
  
  always @(posedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
	  ts_din 	<= 0;
	  input_cnt <= 0;
	end
	else if((ts_clk == 1)) begin
		input_cnt <= input_cnt + 1;
		ts_din 	<= {CRCEncoder_In_Bits[8*input_cnt + 0],CRCEncoder_In_Bits[8*input_cnt + 1],CRCEncoder_In_Bits[8*input_cnt + 2],CRCEncoder_In_Bits[8*input_cnt + 3],CRCEncoder_In_Bits[8*input_cnt + 4],CRCEncoder_In_Bits[8*input_cnt + 5],CRCEncoder_In_Bits[8*input_cnt + 6],CRCEncoder_In_Bits[8*input_cnt + 7]};
	end
	else begin
	   
	end
end
  
    reg [31:0] input_cnt_1x;
    always @(posedge fs_en or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
	  ts_din_1x 	<= 0;
	  input_cnt_1x <= 0;
	end
	else if((ts_rd_vld == 1)) begin
		input_cnt_1x <= input_cnt_1x + 1;
		ts_din_1x 	<= {CRCEncoder_In_Bits[8*input_cnt_1x + 0],CRCEncoder_In_Bits[8*input_cnt_1x + 1],CRCEncoder_In_Bits[8*input_cnt_1x + 2],CRCEncoder_In_Bits[8*input_cnt_1x + 3],CRCEncoder_In_Bits[8*input_cnt_1x + 4],CRCEncoder_In_Bits[8*input_cnt_1x + 5],CRCEncoder_In_Bits[8*input_cnt_1x + 6],CRCEncoder_In_Bits[8*input_cnt_1x + 7]};
	end
	else begin
	   
	end
end
  
  
 //integer start_en_file_pointer;
 integer sys_ctrl_out_file_pointer;
 
 integer crc8_out_file_pointer;
 
 integer randn_input_symbol_out_file_pointer;
 integer bbscrambler_byte_out_file_pointer; 
 integer bchencoder_top_byte_out_file_pointer; 
 integer ldpc_encoder_top_byte_out_file_pointer; 
 integer BitInterLeaver_symbol_out_file_pointer; 
 integer mapper_symbol_out_file_pointer; 
 integer genpilot_symbol_out_file_pointer; 
 integer genplheader_symbol_out_file_pointer;
 integer plframer_symbol_out_file_pointer;
 integer symbol_out_file_pointer;
 
  //integer ldpc_encoder_top_byte_out_1x_file_pointer; 
// integer sys_ctrl_out_1x_file_pointer;
// integer randn_input_symbol_out_1x_file_pointer;
// integer bbscrambler_byte_out_1x_file_pointer; 
// integer bchencoder_top_byte_out_1x_file_pointer; 
// integer ldpc_encoder_top_byte_out_1x_file_pointer; 
// integer BitInterLeaver_symbol_out_1x_file_pointer; 
// integer mapper_symbol_out_1x_file_pointer; 
// integer genpilot_symbol_out_1x_file_pointer; 
// integer genplheader_symbol_out_1x_file_pointer;
// integer plframer_symbol_out_1x_file_pointer;
 integer symbol_out_1x_file_pointer;
 integer symbol_out_2x_file_pointer; 
 initial begin
//	start_en_file_pointer 								= $fopen("D:\Design\FPGA_Design\dvb_s2_system\ST\simulation\modelsim\start_en.txt","w");
	sys_ctrl_out_file_pointer 							= $fopen("sys_ctrl_out.txt","w");
	
	crc8_out_file_pointer                        = $fopen("crc8_out.txt","w");
	
	randn_input_symbol_out_file_pointer 			= $fopen("randn_input_symbol_out.txt","w");
	bbscrambler_byte_out_file_pointer 				= $fopen("bbscrambler_byte_out.txt","w");
	bchencoder_top_byte_out_file_pointer 			= $fopen("bchencoder_top_byte_out.txt","w");
	ldpc_encoder_top_byte_out_file_pointer 		= $fopen("ldpc_encoder_top_byte_out.txt","w");
	BitInterLeaver_symbol_out_file_pointer 		= $fopen("BitInterLeaver_symbol_out.txt","w");
	mapper_symbol_out_file_pointer 					= $fopen("mapper_symbol_out.txt","w");
	genpilot_symbol_out_file_pointer 				= $fopen("genpilot_symbol_out.txt","w");
	genplheader_symbol_out_file_pointer 			= $fopen("genplheader_symbol_out.txt","w");
	plframer_symbol_out_file_pointer 				= $fopen("plframer_symbol_out.txt","w");
	symbol_out_file_pointer 							= $fopen("symbol_out.txt","w");
	
	//ldpc_encoder_top_byte_out_1x_file_pointer 	= $fopen("ldpc_encoder_top_byte_out_1x.txt","w");
//	sys_ctrl_out_1x_file_pointer 							= $fopen("sys_ctrl_out_1x.txt","w");
//	randn_input_symbol_out_1x_file_pointer 			= $fopen("randn_input_symbol_out_1x.txt","w");
//	bbscrambler_byte_out_1x_file_pointer 				= $fopen("bbscrambler_byte_out_1x.txt","w");
//	bchencoder_top_byte_out_1x_file_pointer 			= $fopen("bchencoder_top_byte_out_1x.txt","w");
//	ldpc_encoder_top_byte_out_1x_file_pointer 		= $fopen("ldpc_encoder_top_byte_out_1x.txt","w");
//	BitInterLeaver_symbol_out_1x_file_pointer 		= $fopen("BitInterLeaver_symbol_out_1x.txt","w");
//	mapper_symbol_out_1x_file_pointer 					= $fopen("mapper_symbol_out_1x.txt","w");
//	genpilot_symbol_out_1x_file_pointer 				= $fopen("genpilot_symbol_out_1x.txt","w");
//	genplheader_symbol_out_1x_file_pointer 			= $fopen("genplheader_symbol_out_1x.txt","w");
//	plframer_symbol_out_1x_file_pointer 				= $fopen("plframer_symbol_out_1x.txt","w");
	symbol_out_1x_file_pointer 							= $fopen("symbol_out_1x.txt","w");
	symbol_out_2x_file_pointer 							= $fopen("symbol_out_2x.txt","w");
end
 
 
//always @(negedge sys_clk or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if(glb_start_en == 1) begin
//		$fwrite(start_en_file_pointer,"%x \n",uut.start_en);
//	end
//	else begin
//		$fclose(start_en_file_pointer);
//	end
//end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.glb_start_en == 1)&&(fs_en == 1)) begin
		$fwrite(sys_ctrl_out_file_pointer,"%x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",uut.ldpc_ram_clr_vld,uut.ts_rd_head,uut.ts_rd_vld,uut.bbscramb_init_vld,uut.bbscramb_frame_vld,uut.bit_itl_read_head,uut.bit_itl_read_en,uut.xfec_ready,uut.null_vld,uut.plscramb_init_vld,uut.plscramb_vld,uut.pl_pilot_vld,uut.plheader_head,uut.pl_header_vld);
	end
	else begin
		//$fclose(sys_ctrl_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.crc8_oe == 1)) begin
		$fwrite(crc8_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.crc_byte_out[7],uut.crc_byte_out[6],uut.crc_byte_out[5],uut.crc_byte_out[4],uut.crc_byte_out[3],uut.crc_byte_out[2],uut.crc_byte_out[1],uut.crc_byte_out[0]);
	end
	else begin
		//$fclose(randn_input_symbol_out_file_pointer);
	end
end


always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.randn_input_oe == 1)&&(fs_en == 1)) begin
		$fwrite(randn_input_symbol_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.randn_input_symbol_out[7],uut.randn_input_symbol_out[6],uut.randn_input_symbol_out[5],uut.randn_input_symbol_out[4],uut.randn_input_symbol_out[3],uut.randn_input_symbol_out[2],uut.randn_input_symbol_out[1],uut.randn_input_symbol_out[0]);
	end
	else begin
		//$fclose(randn_input_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.bbscrambler_oe == 1)&&(fs_en == 1)) begin
		$fwrite(bbscrambler_byte_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.bbscrambler_byte_out[7],uut.bbscrambler_byte_out[6],uut.bbscrambler_byte_out[5],uut.bbscrambler_byte_out[4],uut.bbscrambler_byte_out[3],uut.bbscrambler_byte_out[2],uut.bbscrambler_byte_out[1],uut.bbscrambler_byte_out[0]);
	end
	else begin
		//$fclose(bbscrambler_byte_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.bchencoder_top_oe == 1)&&(fs_en == 1)) begin
		$fwrite(bchencoder_top_byte_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.bchencoder_top_byte_out[7],uut.bchencoder_top_byte_out[6],uut.bchencoder_top_byte_out[5],uut.bchencoder_top_byte_out[4],uut.bchencoder_top_byte_out[3],uut.bchencoder_top_byte_out[2],uut.bchencoder_top_byte_out[1],uut.bchencoder_top_byte_out[0]);
	end
	else begin
		//$fclose(bchencoder_top_byte_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.ldpcencoder_mux_oe == 1)&&(fs_en == 1)) begin
		$fwrite(ldpc_encoder_top_byte_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.ldpcencoder_mux_byte_out[7],uut.ldpcencoder_mux_byte_out[6],uut.ldpcencoder_mux_byte_out[5],uut.ldpcencoder_mux_byte_out[4],uut.ldpcencoder_mux_byte_out[3],uut.ldpcencoder_mux_byte_out[2],uut.ldpcencoder_mux_byte_out[1],uut.ldpcencoder_mux_byte_out[0]);
	end
	else begin
		//$fclose(ldpc_encoder_top_byte_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.BitInterLeaver_oe == 1)&&(fs_en == 1)) begin
	case(mod_mode_cfg)
	2'b00:$fwrite(BitInterLeaver_symbol_out_file_pointer,"%x \n%x \n",uut.BitInterLeaver_symbol_out[4],uut.BitInterLeaver_symbol_out[3]);
	2'b01:$fwrite(BitInterLeaver_symbol_out_file_pointer,"%x \n%x \n%x \n",uut.BitInterLeaver_symbol_out[4],uut.BitInterLeaver_symbol_out[3],uut.BitInterLeaver_symbol_out[2]);
	2'b10:$fwrite(BitInterLeaver_symbol_out_file_pointer,"%x \n%x \n%x \n%x \n",uut.BitInterLeaver_symbol_out[4],uut.BitInterLeaver_symbol_out[3],uut.BitInterLeaver_symbol_out[2],uut.BitInterLeaver_symbol_out[1]);
	2'b11:$fwrite(BitInterLeaver_symbol_out_file_pointer,"%x \n%x \n%x \n%x \n%x \n",uut.BitInterLeaver_symbol_out[4],uut.BitInterLeaver_symbol_out[3],uut.BitInterLeaver_symbol_out[2],uut.BitInterLeaver_symbol_out[1],uut.BitInterLeaver_symbol_out[0]);
	endcase
	end
	else begin
		//$fclose(BitInterLeaver_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.mapper_oe == 1)&&(fs_en == 1)) begin
	if(mod_mode_cfg == 2'b00)
	case({uut.mapper_symbol_re_out,uut.mapper_symbol_im_out})
	32'hf4b0f4b0:$fwrite(mapper_symbol_out_file_pointer,"%s \n","-0.70710678118654757 - 0.70710678118654757j");
	32'hf4b00b50:$fwrite(mapper_symbol_out_file_pointer,"%s \n","-0.70710678118654757 + 0.70710678118654757j");
	32'h0b50f4b0:$fwrite(mapper_symbol_out_file_pointer,"%s \n","0.70710678118654757 - 0.70710678118654757j");
	32'h0b500b50:$fwrite(mapper_symbol_out_file_pointer,"%s \n","0.70710678118654757 + 0.70710678118654757j");
	endcase
	else begin
		$fwrite(mapper_symbol_out_file_pointer,"%08f %08f j \n",uut.mapper_symbol_re_out/(2^12),uut.mapper_symbol_im_out/(2^12));
	end
		
	end
	else begin
		//$fclose(mapper_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.genpilot_oe == 1)&&(fs_en == 1)) begin
	if(mod_mode_cfg == 2'b00)
	case({uut.genpilot_symbol_re_out,uut.genpilot_symbol_im_out})
	32'hf4b0f4b0:$fwrite(genpilot_symbol_out_file_pointer,"%s \n","-0.70710678118654757 - 0.70710678118654757j");
	32'hf4b00b50:$fwrite(genpilot_symbol_out_file_pointer,"%s \n","-0.70710678118654757 + 0.70710678118654757j");
	32'h0b50f4b0:$fwrite(genpilot_symbol_out_file_pointer,"%s \n","0.70710678118654757 - 0.70710678118654757j");
	32'h0b500b50:$fwrite(genpilot_symbol_out_file_pointer,"%s \n","0.70710678118654757 + 0.70710678118654757j");
	endcase
	else begin
		$fwrite(genpilot_symbol_out_file_pointer,"%08f %08f j \n",uut.genpilot_symbol_re_out/(2^12),uut.genpilot_symbol_im_out/(2^12));
	end
	end
	else begin
		//$fclose(genpilot_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.genplheader_oe == 1)&&(fs_en == 1)) begin
		
	if(mod_mode_cfg == 2'b00)
		case({uut.genplheader_symbol_re_out,uut.genplheader_symbol_im_out})
	32'hf4b0f4b0:$fwrite(genplheader_symbol_out_file_pointer,"%s \n","-0.70710678118654757 - 0.70710678118654757j");
	32'hf4b00b50:$fwrite(genplheader_symbol_out_file_pointer,"%s \n","-0.70710678118654757 + 0.70710678118654757j");
	32'h0b50f4b0:$fwrite(genplheader_symbol_out_file_pointer,"%s \n","0.70710678118654757 - 0.70710678118654757j");
	32'h0b500b50:$fwrite(genplheader_symbol_out_file_pointer,"%s \n","0.70710678118654757 + 0.70710678118654757j");
	endcase
	else begin
		$fwrite(genplheader_symbol_out_file_pointer,"%08f %08f j \n",uut.genplheader_symbol_re_out/(2^12),uut.genplheader_symbol_im_out/(2^12));
	end
	end
	else begin
		//$fclose(genplheader_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.plframer_oe == 1)&&(fs_en == 1)) begin
		
	if(mod_mode_cfg == 2'b00)
		case({uut.plframer_symbol_re_out,uut.plframer_symbol_im_out})
	32'hf4b0f4b0:$fwrite(plframer_symbol_out_file_pointer,"%s \n","-0.70710678118654757 - 0.70710678118654757j");
	32'hf4b00b50:$fwrite(plframer_symbol_out_file_pointer,"%s \n","-0.70710678118654757 + 0.70710678118654757j");
	32'h0b50f4b0:$fwrite(plframer_symbol_out_file_pointer,"%s \n","0.70710678118654757 - 0.70710678118654757j");
	32'h0b500b50:$fwrite(plframer_symbol_out_file_pointer,"%s \n","0.70710678118654757 + 0.70710678118654757j");
	endcase
	else begin
		$fwrite(plframer_symbol_out_file_pointer,"%08f %08f j \n",uut.plframer_symbol_re_out/(2^12),uut.plframer_symbol_im_out/(2^12));
	end
	end
	else begin
		//$fclose(plframer_symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin      
	if(~glb_rst_n)begin
		
	end
	else if((uut.symbol_1x_oe == 1)&&(fs_en == 1)) begin
		
	if(mod_mode_cfg == 2'b00)
		case({uut.symbol_1x_re_out,uut.symbol_1x_im_out})
	32'hf4b0f4b0:$fwrite(symbol_out_file_pointer,"%s \n","-0.70710678118654757 - 0.70710678118654757j");
	32'hf4b00b50:$fwrite(symbol_out_file_pointer,"%s \n","-0.70710678118654757 + 0.70710678118654757j");
	32'h0b50f4b0:$fwrite(symbol_out_file_pointer,"%s \n","0.70710678118654757 - 0.70710678118654757j");
	32'h0b500b50:$fwrite(symbol_out_file_pointer,"%s \n","0.70710678118654757 + 0.70710678118654757j");
	endcase
	else begin
		$fwrite(symbol_out_file_pointer,"%08f %08f j \n",uut.symbol_1x_re_out/(2^12),uut.symbol_1x_im_out/(2^12));
	end
	end
	else begin
		//$fclose(symbol_out_file_pointer);
	end
end

always @(negedge sys_clk or negedge glb_rst_n)begin    
	if(~glb_rst_n)begin
		
	end
	else if((uut.symbol_1x_oe == 1)) begin
		$fwrite(symbol_out_1x_file_pointer,"%f %f \n",uut.symbol_1x_re_out,uut.symbol_1x_im_out);
	end
	else begin
		//$fclose(symbol_out_file_pointer);
	end
end

//always @(negedge sys_clk or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
		
//	end
//	else if((uut.symbol_2x_oe == 1)) begin
//		$fwrite(symbol_out_2x_file_pointer,"%f %f \n",uut.symbol_2x_re_out,uut.symbol_2x_im_out);
//	end
//	else begin
//		//$fclose(symbol_out_file_pointer);
//	end
//end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////always @(negedge fs_en or negedge glb_rst_n)begin      
////	if(~glb_rst_n)begin
////		
////	end
////	else if((uut.ldpcencoder_mux_oe_1x == 1)) begin
////		$fwrite(ldpc_encoder_top_byte_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut.ldpcencoder_mux_byte_out_1x[7],uut.ldpcencoder_mux_byte_out_1x[6],uut.ldpcencoder_mux_byte_out_1x[5],uut.ldpcencoder_mux_byte_out_1x[4],uut.ldpcencoder_mux_byte_out_1x[3],uut.ldpcencoder_mux_byte_out_1x[2],uut.ldpcencoder_mux_byte_out_1x[1],uut.ldpcencoder_mux_byte_out_1x[0]);
////	end
////	else begin
////		//$fclose(ldpc_encoder_top_byte_out_file_pointer);
////	end
////end



//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.glb_start_en == 1)) begin
//		$fwrite(sys_ctrl_out_1x_file_pointer,"%x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",uut_1x.ldpc_ram_clr_vld,uut_1x.ts_rd_head,uut_1x.ts_rd_vld,uut_1x.bbscramb_init_vld,uut_1x.bbscramb_frame_vld,uut_1x.bit_itl_read_head,uut_1x.bit_itl_read_en,uut_1x.xfec_ready,uut_1x.null_vld,uut_1x.plscramb_init_vld,uut_1x.plscramb_vld,uut_1x.pl_pilot_vld,uut_1x.plheader_head,uut_1x.pl_header_vld);
//	end
//	else begin
//		//$fclose(sys_ctrl_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.randn_input_oe == 1)) begin
//		$fwrite(randn_input_symbol_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut_1x.randn_input_symbol_out[7],uut_1x.randn_input_symbol_out[6],uut_1x.randn_input_symbol_out[5],uut_1x.randn_input_symbol_out[4],uut_1x.randn_input_symbol_out[3],uut_1x.randn_input_symbol_out[2],uut_1x.randn_input_symbol_out[1],uut_1x.randn_input_symbol_out[0]);
//	end
//	else begin
//		//$fclose(randn_input_symbol_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.bbscrambler_oe == 1)) begin
//		$fwrite(bbscrambler_byte_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut_1x.bbscrambler_byte_out[7],uut_1x.bbscrambler_byte_out[6],uut_1x.bbscrambler_byte_out[5],uut_1x.bbscrambler_byte_out[4],uut_1x.bbscrambler_byte_out[3],uut_1x.bbscrambler_byte_out[2],uut_1x.bbscrambler_byte_out[1],uut_1x.bbscrambler_byte_out[0]);
//	end
//	else begin
//		//$fclose(bbscrambler_byte_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.bchencoder_top_oe == 1)) begin
//		$fwrite(bchencoder_top_byte_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut_1x.bchencoder_top_byte_out[7],uut_1x.bchencoder_top_byte_out[6],uut_1x.bchencoder_top_byte_out[5],uut_1x.bchencoder_top_byte_out[4],uut_1x.bchencoder_top_byte_out[3],uut_1x.bchencoder_top_byte_out[2],uut_1x.bchencoder_top_byte_out[1],uut_1x.bchencoder_top_byte_out[0]);
//	end
//	else begin
//		//$fclose(bchencoder_top_byte_out_file_pointer);
//	end
//end
//
//always @(negedge sys_clk or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.ldpcencoder_mux_oe == 1)) begin
//		$fwrite(ldpc_encoder_top_byte_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut_1x.ldpcencoder_mux_byte_out[7],uut_1x.ldpcencoder_mux_byte_out[6],uut_1x.ldpcencoder_mux_byte_out[5],uut_1x.ldpcencoder_mux_byte_out[4],uut_1x.ldpcencoder_mux_byte_out[3],uut_1x.ldpcencoder_mux_byte_out[2],uut_1x.ldpcencoder_mux_byte_out[1],uut_1x.ldpcencoder_mux_byte_out[0]);
//	end
//	else begin
//		//$fclose(ldpc_encoder_top_byte_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.BitInterLeaver_oe == 1)) begin
//		$fwrite(BitInterLeaver_symbol_out_1x_file_pointer,"%x \n%x \n%x \n%x \n%x \n%x \n%x \n%x \n",uut_1x.BitInterLeaver_symbol_out[7],uut_1x.BitInterLeaver_symbol_out[6],uut_1x.BitInterLeaver_symbol_out[5],uut_1x.BitInterLeaver_symbol_out[4],uut_1x.BitInterLeaver_symbol_out[3],uut_1x.BitInterLeaver_symbol_out[2],uut_1x.BitInterLeaver_symbol_out[1],uut_1x.BitInterLeaver_symbol_out[0]);
//	end
//	else begin
//		//$fclose(BitInterLeaver_symbol_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.mapper_oe == 1)) begin
//		$fwrite(mapper_symbol_out_1x_file_pointer,"%04x %04x \n",uut_1x.mapper_symbol_re_out,uut_1x.mapper_symbol_im_out);
//	end
//	else begin
//		//$fclose(mapper_symbol_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.genpilot_oe == 1)) begin
//		$fwrite(genpilot_symbol_out_1x_file_pointer,"%04x %04x \n",uut_1x.genpilot_symbol_re_out,uut_1x.genpilot_symbol_im_out);
//	end
//	else begin
//		//$fclose(genpilot_symbol_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.genplheader_oe == 1)) begin
//		$fwrite(genplheader_symbol_out_1x_file_pointer,"%04x %04x \n",uut_1x.genplheader_symbol_re_out,uut_1x.genplheader_symbol_im_out);
//	end
//	else begin
//		//$fclose(genplheader_symbol_out_file_pointer);
//	end
//end
//
//always @(negedge fs_en or negedge glb_rst_n)begin      
//	if(~glb_rst_n)begin
//		
//	end
//	else if((uut_1x.plframer_oe == 1)) begin
//		$fwrite(plframer_symbol_out_1x_file_pointer,"%04x %04x \n",uut_1x.plframer_symbol_re_out,uut_1x.plframer_symbol_im_out);
//	end
//	else begin
//		//$fclose(plframer_symbol_out_file_pointer);
//	end
//end
//

endmodule
