`timescale 1ns / 1ps
module dvb_s2_system_top(
input	          							hard_rst_n,// modified by 2014.09.22
//////////////////////////////////////////////////////////////
input	          			[1:0]			mod_mode_cfg,
input	          			[3:0] 		     ldpc_mode_cfg,
input	          							frame_mode_cfg,
input   			 				    		pilot_mode_cfg,
input							[1:0]			srrc_mode,//00:0.35; 01:0.25; 10:0.20(default) 
input							[2:0]			dvb_s_convolution_mode,
input   			 				    		dvb_s_mode,// 0:dvb-s; 1:dvb-s2
input							[1:0]			TS_Source_mode,// 00:TS Source inside by ts_clk; 01:TS Source outside input without Empty Frame; 10: TS Source outside input with Empty Frame;
input							[31:0]		    SYS_Baud_Num,//32'd2500 --> 25M BaudRate  
input							[31:0]		    SYS_Freq_Num,//32'd10000 --> 100M System Clock,
input											Freq_Inv_mode,// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);
//////////////////////////////////////////////////////////////
input                  					ts_clk_h264out,// clock from h.264 encoder
input							[7:0]       ts_din_h264out,// @ ts_clk_out
input                  					ts_syn_h264out,// @ ts_clk_out ts stream head
input                  					ts_valid_h264out,// @ ts_clk_out
//////////////////////////////////////////////////////////////
input                  					sys_clk,
//input                  					sys_clk_fs_en,
//input							[7:0]       ts_din,// @ sys_clk
//input                  					ts_syn,// @ sys_clk
//input                  					ts_head,// @ sys_clk
output               					ts_clk,// @ sys_clk
//input                 					fs_en_switch,//0:from outer;1:from inner
//input                 					fs_en_outer,
//input                 					fs_en2_outer,
input                					fs_0p5_en,
//////////////////////////////////////////////////////////////
output		reg				[7:0]       ts_din,// @ sys_clk
output      reg         					ts_syn,// @ sys_clk
output      reg         				ts_head,// @ sys_clk
//output                             fs_en_inner,
//output                             fs_en2_inner,
output                             fs_en_1cycle,
//////////////////////////////////////////////////////////////
output 		reg							symbol_1x_oe = 0,
output 	    reg 	signed [15:0]			symbol_1x_re_out = 0,
output 		reg    signed [15:0]			symbol_1x_im_out = 0//,

//output 									symbol_2x_oe,
//output 		signed [15:0]			symbol_2x_re_out,
//output 		signed [15:0]			symbol_2x_im_out
);

//parameter SYS_Baud_mode = 2'b01;// 00:10M; 01:25M;

wire                             fs_en;
wire                             fs_en2;

wire                            fs_en_1cycle_1dly;

//fs_transform uut_fs_transform(
//.hard_rst_n(hard_rst_n),// modified by 2014.09.22
//.sys_clk(sys_clk),
//.fs_0p5_en(fs_0p5_en),
//.fs_en_1cycle(fs_en_1cycle),
//.fs_en_1cycle_1dly(fs_en_1cycle_1dly)
//);

fs_en_process fs_en_process_inst(
	.sys_clk(sys_clk),
	.rst_n(hard_rst_n),

	.fs_en2(fs_0p5_en),

	.fs_en_on_sys_clk(fs_en_1cycle)
);


//gen_fs_en uut_gen_fs_en(
//    .sys_clk(sys_clk),
//    .glb_rst_n(hard_rst_n),
//    .SYS_Baud_Num(SYS_Baud_Num),//32'd2500 --> 25M BaudRate   SYS_Baud_mode,
//    .SYS_Freq_Num(SYS_Freq_Num),
//    .fs_en(fs_en_inner),
//    .fs_en2(fs_en2_inner)
//    );

assign    fs_en = fs_en_1cycle;
//assign    fs_en = (fs_en_switch == 1)?fs_en_inner:fs_en_1cycle;
//assign    fs_en2 = (fs_en_switch == 1)?fs_en2_inner:fs_en2_outer;

//always @(posedge sys_clk)begin
//	if(~hard_rst_n)begin
//		fs_en <= 0;
//		fs_en2 <= 0;
//	end
//	else if(fs_en_switch == 1'b1)begin
//			fs_en <= fs_en_inner;
//            fs_en2 <= fs_en2_inner;
//	end
//	else begin
//	   fs_en <= fs_en_outer;
//       fs_en2 <= fs_en2_outer;
//	end
//end


//reg					  [7:0]       ts_din;// @ sys_clk
//reg                  			  ts_syn;// @ sys_clk
//reg                  			  ts_head;// @ sys_clk
//wire               				  ts_clk;// @ sys_clk

//reg	  								  mepg2_ts_fifo_rdreq;// @ sys_clk
wire					  [7:0]       mepg2_ts_fifo_q;// @ sys_clk
wire					 [13:0]  	  mepg2_ts_fifo_rdusedw;// @ sys_clk

parameter				FIFOOUT_IDLE_STATE 	= 6'b000001;
parameter				FIFOOUT_WORK_STATE 	= 6'b000010;
parameter				FIFOOUT_NULL_STATE_0 = 6'b000100;
parameter				FIFOOUT_NULL_STATE_1 = 6'b001000;
parameter				FIFOOUT_NULL_STATE_2 = 6'b010000;
parameter				FIFOOUT_NULL_STATE_3 = 6'b100000;


reg					  [5:0]		  fifoout_state;
reg					  [7:0]		  ts_cnt;// 1dly
reg					  [3:0]		  null_idx;

always @(posedge sys_clk)begin
	if(~hard_rst_n)begin
		ts_cnt <= 8'h00;
	end
	else if(ts_clk == 1'b1)begin
			if(ts_cnt == 187)begin
				ts_cnt <= 8'h00;
			end
			else begin
				ts_cnt <= ts_cnt + 1;
			end
	end
end


reg					rd_fifo_en;//1dly
reg					rd_fifo_en_1dly;//2dly
reg					ts_clk_1dly;
reg					ts_clk_2dly;
reg					ts_clk_3dly;
reg					  [7:0]		  ts_cnt_1dly;// 1dly
reg					  [7:0]		  ts_cnt_2dly;// 2dly

always @(posedge sys_clk)begin
	if(~hard_rst_n)begin
		rd_fifo_en <= 1'b0;
		rd_fifo_en_1dly <= 1'b0;
		ts_clk_1dly <= 1'b0;
		ts_clk_2dly <= 1'b0;
		ts_clk_3dly <= 1'b0;
		ts_cnt_1dly <= 8'h00;
		ts_cnt_2dly <= 8'h00;
	end
	else begin
		ts_clk_1dly <= ts_clk;
		ts_clk_2dly <= ts_clk_1dly;
		ts_clk_3dly <= ts_clk_2dly;
		rd_fifo_en_1dly <= rd_fifo_en;
		ts_cnt_1dly <= ts_cnt;
		ts_cnt_2dly <= ts_cnt_1dly;
		
			if(ts_clk == 1'b1)begin
				if(ts_cnt == 8'd187)begin
					if(mepg2_ts_fifo_rdusedw >= 188)begin//13'h00ff)begin
						rd_fifo_en <= 1'b1;
					end
					else begin
						rd_fifo_en <= 1'b0;
					end
				end
				else begin
				end
			end
			else begin
			end
		end
end

always @(posedge sys_clk)begin
	if(~hard_rst_n)begin
		fifoout_state <= FIFOOUT_IDLE_STATE;
		ts_din <= 8'h00;
		ts_syn <= 1'b0;
		ts_head <= 1'b0;
		null_idx <= 4'h0;
	end
	else begin
		case(fifoout_state)
		FIFOOUT_IDLE_STATE:begin
			if(ts_clk_2dly == 1'b1)begin
				if(ts_cnt_2dly == 8'h00)begin
					if(rd_fifo_en_1dly == 1'b1)begin//13'h00ff)begin
						fifoout_state <= FIFOOUT_WORK_STATE;
						ts_din <= mepg2_ts_fifo_q;
						ts_syn <= ts_clk_2dly;
						ts_head <= 1'b1;
					end
					else begin
						fifoout_state <= FIFOOUT_NULL_STATE_0;
						ts_din <= 8'h47;
						ts_syn <= ts_clk_2dly;
						ts_head <= 1'b1;
					end
				end
				else begin
				end
			end
			else begin
				ts_syn <= 1'b0;
			end
		end
		FIFOOUT_WORK_STATE:begin
			ts_syn <= ts_clk_2dly;
			ts_head <= 1'b0;
			
			if(ts_clk_2dly == 1'b1)begin
				ts_din <= mepg2_ts_fifo_q;
				if(ts_cnt_2dly == 8'd187)begin
					fifoout_state <= FIFOOUT_IDLE_STATE;
				end
				else begin
					fifoout_state <= FIFOOUT_WORK_STATE;
				end	
			end
			else begin
				fifoout_state <= FIFOOUT_WORK_STATE;
			end
		end
		FIFOOUT_NULL_STATE_0:begin
			ts_syn <= ts_clk_2dly;
			ts_head <= 1'b0;
			
			if(ts_clk_2dly == 1'b1)begin
				fifoout_state <= FIFOOUT_NULL_STATE_1;
				ts_din <= 8'h1f;
			end
			else begin
				fifoout_state <= FIFOOUT_NULL_STATE_0;
			end
		end
		FIFOOUT_NULL_STATE_1:begin
			ts_syn <= ts_clk_2dly;
			ts_head <= 1'b0;
			
			if(ts_clk_2dly == 1'b1)begin
				fifoout_state <= FIFOOUT_NULL_STATE_2;
				ts_din <= 8'hff;
			end
			else begin
				fifoout_state <= FIFOOUT_NULL_STATE_1;
			end
		end
		FIFOOUT_NULL_STATE_2:begin
			ts_syn <= ts_clk_2dly;
			ts_head <= 1'b0;
	
			if(ts_clk_2dly == 1'b1)begin
				ts_din <= {4'h1,null_idx};
				fifoout_state <= FIFOOUT_NULL_STATE_3;
				null_idx <= null_idx + 1;
			end
			else begin
				fifoout_state <= FIFOOUT_NULL_STATE_2;
			end
		end
		FIFOOUT_NULL_STATE_3:begin
			ts_syn <= ts_clk_2dly;
			ts_head <= 1'b0;
			
			if(ts_clk_2dly == 1'b1)begin
				ts_din <= 8'hff;
				if(ts_cnt_2dly == 8'd187)begin
					fifoout_state <= FIFOOUT_IDLE_STATE;
				end
				else begin
					fifoout_state <= FIFOOUT_NULL_STATE_3;
				end	
			end
			else begin
					fifoout_state <= FIFOOUT_NULL_STATE_3;
			end
		end
		default:begin
			fifoout_state <= FIFOOUT_IDLE_STATE;
			ts_din <= 8'h00;
			ts_syn <= 1'b0;
			ts_head <= 1'b0;
			null_idx <= 4'h0;
		end
		endcase
	end
end

reg							[7:0]       ts_din_h264out_1dly;// @ ts_clk_out
reg                  					ts_syn_h264out_1dly;// @ ts_clk_out ts stream head
reg                  					ts_valid_h264out_1dly;// @ ts_clk_out
reg							[7:0]       ts_din_h264out_2dly;// @ ts_clk_out
reg                  					ts_syn_h264out_2dly;// @ ts_clk_out ts stream head
reg                  					ts_valid_h264out_2dly;// @ ts_clk_out
reg							[7:0]       ts_din_h264out_3dly;// @ ts_clk_out
reg                  					ts_syn_h264out_3dly;// @ ts_clk_out ts stream head
reg                  					ts_valid_h264out_3dly;// @ ts_clk_out
reg							[7:0]       ts_din_h264out_4dly;// @ ts_clk_out
reg                  					ts_syn_h264out_4dly;// @ ts_clk_out ts stream head
reg                  					ts_valid_h264out_4dly;// @ ts_clk_out

always @(posedge ts_clk_h264out)begin
	if(~hard_rst_n)begin
		ts_din_h264out_1dly <= 8'h00;
		ts_syn_h264out_1dly <= 1'b0;
		ts_valid_h264out_1dly <= 1'b0;
		ts_din_h264out_2dly <= 8'h00;
		ts_syn_h264out_2dly <= 1'b0;
		ts_valid_h264out_2dly <= 1'b0;
		ts_din_h264out_3dly <= 8'h00;
		ts_syn_h264out_3dly <= 1'b0;
		ts_valid_h264out_3dly <= 1'b0;
		ts_din_h264out_4dly <= 8'h00;
		ts_syn_h264out_4dly <= 1'b0;
		ts_valid_h264out_4dly <= 1'b0;
	end
	else begin
		ts_din_h264out_1dly <= ts_din_h264out;
		ts_syn_h264out_1dly <= ts_syn_h264out;
		ts_valid_h264out_1dly <= ts_valid_h264out;
		ts_din_h264out_2dly <= ts_din_h264out_1dly;
		ts_syn_h264out_2dly <= ts_syn_h264out_1dly;
		ts_valid_h264out_2dly <= ts_valid_h264out_1dly;
		ts_din_h264out_3dly <= ts_din_h264out_2dly;
		ts_syn_h264out_3dly <= ts_syn_h264out_2dly;
		ts_valid_h264out_3dly <= ts_valid_h264out_2dly;
		ts_din_h264out_4dly <= ts_din_h264out_3dly;
		ts_syn_h264out_4dly <= ts_syn_h264out_3dly;
		ts_valid_h264out_4dly <= ts_valid_h264out_3dly;
	end
end


parameter				NULLCHK_IDLE_STATE   = 4'b0001;
parameter				NULLCHK_PHASE0_STATE = 4'b0010;
parameter				NULLCHK_PHASE1_STATE = 4'b0100;
parameter				NULLCHK_PHASE2_STATE = 4'b1000;

reg				[3:0]				null_chk_state;
reg										null_chk_flag;// 1:null


always @(posedge ts_clk_h264out)begin
	if(~hard_rst_n)begin
		null_chk_state <= NULLCHK_IDLE_STATE;
		null_chk_flag			 <= 1'b0;
	end
	else begin
		case(null_chk_state)
		NULLCHK_IDLE_STATE:begin
			if(ts_syn_h264out == 1'b1)begin
				null_chk_state <= NULLCHK_PHASE0_STATE;
				null_chk_flag			 <= 1'b0;
			end
			else begin
				null_chk_state <= NULLCHK_IDLE_STATE;
			end
		end
		NULLCHK_PHASE0_STATE:begin
			if(ts_valid_h264out == 1'b1)begin
				if(ts_din_h264out == 8'h1F)begin
					null_chk_state <= NULLCHK_PHASE1_STATE;
				end
				else begin
					null_chk_state <= NULLCHK_IDLE_STATE;
				end
			end
			else begin
				null_chk_state <= NULLCHK_PHASE0_STATE;
			end
		end
		NULLCHK_PHASE1_STATE:begin
			if(ts_valid_h264out == 1'b1)begin
				if(ts_din_h264out == 8'hFF)begin
					null_chk_state <= NULLCHK_PHASE2_STATE;
				end
				else begin
					null_chk_state <= NULLCHK_IDLE_STATE;
				end
			end
			else begin
				null_chk_state <= NULLCHK_PHASE1_STATE;
			end
		end
		NULLCHK_PHASE2_STATE:begin
			if(ts_valid_h264out == 1'b1)begin
				if(ts_din_h264out[7:4] ==4'h1)begin
					null_chk_flag			 <= 1'b1;
				end
				else begin
					null_chk_flag			 <= 1'b0;
				end
				null_chk_state <= NULLCHK_IDLE_STATE;
			end
			else begin
				null_chk_state <= NULLCHK_PHASE2_STATE;
			end
		end
		default:begin
			null_chk_state <= NULLCHK_IDLE_STATE;
		  null_chk_flag			 <= 1'b0;
		end
		endcase
	end
end


reg					  [7:0]  	  mepg2_ts_fifo_data;// @ ts_clk_out
reg	  								  mepg2_ts_fifo_wrreq;// @ ts_clk_out

parameter				FIFOIN_IDLE_STATE = 2'b01;
parameter				FIFOIN_WORK_STATE = 2'b10;

reg				[1:0]				fifo_in_state;


always @(posedge ts_clk_h264out)begin
	if(~hard_rst_n)begin
		fifo_in_state <= FIFOIN_IDLE_STATE;
		mepg2_ts_fifo_data <= 8'H00;
		mepg2_ts_fifo_wrreq <= 1'b0;
	end
	else begin
		case(fifo_in_state)
		FIFOIN_IDLE_STATE:begin
			if((ts_syn_h264out_4dly == 1'b1)&&(null_chk_flag == 1'b0))begin
				fifo_in_state <= FIFOIN_WORK_STATE;
				mepg2_ts_fifo_data <= ts_din_h264out_4dly;
				mepg2_ts_fifo_wrreq <= ts_valid_h264out_4dly;
			end
			else begin
				fifo_in_state <= FIFOIN_IDLE_STATE;
				mepg2_ts_fifo_data <= 8'H00;
				mepg2_ts_fifo_wrreq <= 1'b0;
			end
		end
		FIFOIN_WORK_STATE:begin
			fifo_in_state <= FIFOIN_WORK_STATE;
			mepg2_ts_fifo_data <= ts_din_h264out_4dly;
			mepg2_ts_fifo_wrreq <= ts_valid_h264out_4dly;
		end
		default:begin
			fifo_in_state <= FIFOIN_IDLE_STATE;
			mepg2_ts_fifo_data <= 8'H00;
			mepg2_ts_fifo_wrreq <= 1'b0;
		end
		endcase
	end
end

//mepg2_ts_fifo uut_mepg2_ts_fifo(
//.aclr										(~hard_rst_n),
//.data										(mepg2_ts_fifo_data),
//.rdclk									(sys_clk),
//.rdreq									(rd_fifo_en&&ts_clk),
//.wrclk									(ts_clk_h264out),
//.wrreq									(mepg2_ts_fifo_wrreq),
//.q											(mepg2_ts_fifo_q),
//.rdempty									(),
//.rdusedw									(mepg2_ts_fifo_rdusedw),
//.wrfull									()
//);

mepg2_ts_fifo uut_mepg2_ts_fifo (
  .rst(~hard_rst_n),                      // input wire rst
  .wr_clk(ts_clk_h264out),                // input wire wr_clk
  .rd_clk(sys_clk),                // input wire rd_clk
  .din(mepg2_ts_fifo_data),                      // input wire [7 : 0] din
  .wr_en(mepg2_ts_fifo_wrreq),                  // input wire wr_en
  .rd_en(rd_fifo_en&&ts_clk),                  // input wire rd_en
  .dout(mepg2_ts_fifo_q),                    // output wire [7 : 0] dout
  .full(),                    // output wire full
  .wr_ack(),                // output wire wr_ack
  .overflow(),            // output wire overflow
  .empty(),                  // output wire empty
  .valid(),                  // output wire valid
  .underflow(),          // output wire underflow
  .rd_data_count(mepg2_ts_fifo_rdusedw),  // output wire [13 : 0] rd_data_count
  .wr_data_count()  // output wire [13 : 0] wr_data_count
);

//parameter	valid_restrictions      = 1;// modified by 2014.09.22

wire											glb_rst_n;// modified by 2014.09.22
reg											soft_rst_n;// modified by 2014.09.22

reg					[43:0]				soft_rst_n_cnt;// modified by 2014.09.22

always @(posedge sys_clk)begin // modified by 2014.09.22
	if(~hard_rst_n)begin
		soft_rst_n <= 1'b1;//1'b1;// modified by 2014.10.04
		soft_rst_n_cnt <= 40'h000000000;
	end
	else begin
		if((soft_rst_n_cnt >= 44'h7DBA8218000))begin//40'h218711A000
			soft_rst_n <= 1'b0;
		end//dec2hex(24*60*60*100e6) 44'h7DBA8218000
		else begin
			soft_rst_n <= 1'b1;
			soft_rst_n_cnt <= soft_rst_n_cnt + 44'h000000001;
		end
	end
end

assign	glb_rst_n						= 	hard_rst_n&&soft_rst_n; // modified by 2014.09.22

wire											bbscrambler_en;

wire	          			[1:0]			mod_mode;
wire	          			[3:0] 		    ldpc_mode;// modified by 2014.09.22
wire	          							frame_mode;
wire   			 				    		pilot_mode;


assign	frame_mode 	= frame_mode_cfg;
assign	pilot_mode 	= pilot_mode_cfg;
assign	ldpc_mode 	= ldpc_mode_cfg;
assign	mod_mode 	= mod_mode_cfg;

assign bbscrambler_en 					= 1'b1;

//always @(*)begin// modified by 2014.09.22
//	case({frame_mode_cfg,pilot_mode_cfg,ldpc_mode_cfg,mod_mode_cfg})
//	8'b00010000:begin
//		frame_mode <= frame_mode_cfg;
//		pilot_mode <= pilot_mode_cfg;
//		ldpc_mode <= ldpc_mode_cfg;
//		mod_mode <= mod_mode_cfg;
//	end
//	8'b00100001:begin
//		frame_mode <= frame_mode_cfg;
//		pilot_mode <= pilot_mode_cfg;
//		ldpc_mode <= ldpc_mode_cfg;
//		mod_mode <= mod_mode_cfg;
//	end
//	default:begin
//		frame_mode <= 1'b0;
//		pilot_mode <= 1'b0;
//		ldpc_mode <= 4'b0100;
//		mod_mode <= 2'b00;
//	end
//	endcase
//end



wire											start_en;
wire			  								ldpc_ram_clr_vld;      // align with start_en_4dly  
wire			  								ts_rd_head;            // align with start_en_4dly  
wire			  								ts_rd_vld;             // align with start_en_4dly
wire			  								bbscramb_init_vld;     // align with start_en_4dly
wire			  								bbscramb_frame_vld;    // align with start_en_4dly
wire			  								bit_itl_read_head;     // align with start_en_4dly
wire			  								bit_itl_read_en;       // align with start_en_4dly
wire			  								xfec_ready;            // align with start_en_4dly
wire			  								null_vld;              // align with start_en_4dly
wire			  								plscramb_init_vld;     // align with start_en_4dly
wire			  								plscramb_vld;          // align with start_en_4dly
wire			  								pl_pilot_vld;          // align with start_en_4dly
wire			  								plheader_head;         // align with start_en_4dly
wire			  								pl_header_vld;         // align with start_en_4dly
wire			  								randn_input_oe_head		;
wire			  								randn_input_oe				;
wire			  				[7:0]		randn_input_symbol_out;
wire			  								bbscrambler_oe;
wire			  			   [7:0]    	bbscrambler_byte_out;
wire			  								bchencoder_top_oe;
wire			  			   [7:0]    	bchencoder_top_byte_out;
wire			  										ldpcencoder_mux_oe;
wire			  			   [7:0]    	ldpcencoder_mux_byte_out;
wire									          		BitInterLeaver_oe;       
wire									    [4:0]			BitInterLeaver_symbol_out;
wire			  														mapper_oe;
wire			  	signed	   	[15:0]     		mapper_symbol_re_out;
wire			  	signed	   	[15:0]     		mapper_symbol_im_out;
wire																	genpilot_oe;
wire							signed	[15:0]			genpilot_symbol_re_out;
wire							signed	[15:0]			genpilot_symbol_im_out;
wire																genplheader_oe;
wire						signed	[15:0]			genplheader_symbol_re_out;
wire						signed	[15:0]			genplheader_symbol_im_out;
wire															plframer_oe;                  
wire				signed [15:0]					plframer_symbol_re_out;
wire				signed [15:0]					plframer_symbol_im_out;


wire 									oe;
wire 		signed [15:0]			symbol_re_out;
wire 		signed [15:0]			symbol_im_out;


wire[8:0]			create_null_dout;
wire					create_null_wren;

create_null	uut_create_null(
.sys_clk					(sys_clk),
.glb_rst_n				(glb_rst_n),// 
.byte_en					(ts_clk),
.dout						(create_null_dout),
.wren						(create_null_wren)	
);


wire			[7:0]		ts_din_mux;
wire						ts_syn_mux;
wire						ts_head_mux;

assign ts_din_mux = (TS_Source_mode == 2'b00)?create_null_dout[7:0]:((TS_Source_mode == 2'b01)?ts_din_h264out:ts_din);
assign ts_syn_mux = (TS_Source_mode == 2'b00)?create_null_wren:((TS_Source_mode == 2'b01)?ts_valid_h264out:ts_syn);
assign ts_head_mux = (TS_Source_mode == 2'b00)?(create_null_wren&&create_null_dout[8]):((TS_Source_mode == 2'b01)?ts_syn_h264out:ts_head);

// 00:TS Source inside by ts_clk; 01:TS Source outside input without Empty Frame; 10: TS Source outside input with Empty Frame;

wire						crc8_oe;
wire			[7:0]		crc_byte_out;

crc8 uut_crc8(
.sys_clk					(sys_clk),
.glb_rst_n				(glb_rst_n),
//////////////////////////////////////////////////////////////
.ts_in					(ts_din_mux),// @ sys_clk
.ts_syn					(ts_syn_mux),// @ sys_clk
.ts_head					(ts_head_mux),// @ sys_clk
//////////////////////////////////////////////////////////////
.crc8_cync_en			(),//(crc8_cync_en),
.crc8_oe					(crc8_oe),
.crc						(),//(crc),
.crc_byte_out			(crc_byte_out)
//////////////////////////////////////////////////////////////
);


wire	          							glb_start_en;
wire			  								ts_byte_en;
wire			  					[7:0]		ts_byte_in;

ts_interface uut_ts_interface(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.glb_rst_n									(glb_rst_n),
.SYS_Baud_Num								(SYS_Baud_Num),// modified by 2014.09.22
.SYS_Freq_Num                               (SYS_Freq_Num),
.ts_din										(crc_byte_out),
.ts_syn										(crc8_oe),
.glb_start_en								(glb_start_en),
.ts_rd_vld									(ts_rd_vld),
.bbscramb_frame_vld						(bbscramb_frame_vld),
//.clk_1x										(clk_1x),
.ts_byte_en									(ts_byte_en),//delay: 10 cycles at clk_1x cmp bbscramb_frame_vld
.ts_byte_in									(ts_byte_in),
.srrc_mode									(srrc_mode),//00:0.35; 01:0.25; 00:0.20 
.mod_mode									(mod_mode),
.ldpc_mode									(ldpc_mode),
.frame_mode									(frame_mode),
.pilot_mode									(pilot_mode),
.ts_clk										(ts_clk)
);



assign	start_en = glb_start_en;

sys_ctrl uut_sys_ctrl(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.start_en									(start_en),

.mod_mode									(mod_mode),
.ldpc_mode									(ldpc_mode),
.frame_mode									(frame_mode),
.pilot_mode									(pilot_mode),
//////////////////////////////////////////////////////////////
.ldpc_ram_clr_vld						     (ldpc_ram_clr_vld),      // align with start_en_4dly  
.ts_rd_head									 (ts_rd_head),      // align with start_en_4dly  
.ts_rd_vld									 (ts_rd_vld),      // align with start_en_4dly
.bbscramb_init_vld					         (bbscramb_init_vld),      // align with start_en_4dly
.bbscramb_frame_vld					         (bbscramb_frame_vld),      // align with start_en_4dly
.bit_itl_read_head					         (bit_itl_read_head),      // align with start_en_4dly
.bit_itl_read_en						     (bit_itl_read_en),      // align with start_en_4dly
.xfec_ready									 (xfec_ready					),      // align with start_en_4dly
.null_vld										(null_vld						),      // align with start_en_4dly
.plscramb_init_vld					(plscramb_init_vld	),      // align with start_en_4dly
.plscramb_vld								(plscramb_vld				),      // align with start_en_4dly
.pl_pilot_vld								(pl_pilot_vld				),      // align with start_en_4dly
.plheader_head							(plheader_head			),      // align with start_en_4dly
.pl_header_vld							(pl_header_vld			)       // align with start_en_4dly
//////////////////////////////////////////////////////////////
);


randn_input uut_randn_input(// Module delay: 10 cycles at clk_1x 
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.ts_rd_head									(bbscramb_init_vld					),      // align with start_en_4dly  
.ts_rd_vld									(bbscramb_frame_vld					),      // align with start_en_4dly
.oe_head										(randn_input_oe_head		),
.oe													(),//(randn_input_oe				  ),
.symbol_out									()//(randn_input_symbol_out )
);


assign randn_input_oe		 	= ts_byte_en;
assign randn_input_symbol_out = ts_byte_in;

bbscrambler uut_bbscrambler(// Module delay: 2 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
.bbscrambler_en							(bbscrambler_en),
//////////////////////////////////////////////////////////////
.init_vld									(randn_input_oe_head		),
.frame_vld								(randn_input_oe				  ),
.byte_in									(randn_input_symbol_out ),
//////////////////////////////////////////////////////////////
.oe												(bbscrambler_oe),
.byte_out									(bbscrambler_byte_out)
//////////////////////////////////////////////////////////////
);

reg											randn_input_oe_head_1dly;
reg											randn_input_oe_head_2dly;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		randn_input_oe_head_1dly <= 1'b0;
		randn_input_oe_head_2dly <= 1'b0;
	end
	else if(fs_en == 1'b1)begin	
		randn_input_oe_head_1dly <= randn_input_oe_head;
		randn_input_oe_head_2dly <= randn_input_oe_head_1dly;
	end
	else begin
	end
end

bchencoder_top uut_bchencoder_top(// Module delay: 8 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
.init_vld									(randn_input_oe_head_2dly),
.frame_vld									(bbscrambler_oe),
.byte_in										(bbscrambler_byte_out),
//////////////////////////////////////////////////////////////
.oe											(bchencoder_top_oe),
.byte_out									(bchencoder_top_byte_out)
//////////////////////////////////////////////////////////////
);


reg			  								bchencoder_top_oe_1dly;

wire			  										ldpc_encoder_top_oe;
wire			  			   [7:0]    	ldpc_encoder_top_byte_out;

ldpc_encoder_top uut_ldpc_encoder_top(
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.frame_mode									(frame_mode),
.ldpc_mode									(ldpc_mode),
//////////////////////////////////////////////////////////////
.init_vld									((bchencoder_top_oe_1dly == 1'b0)&&(bchencoder_top_oe == 1'b1)),
.frame_vld									(bchencoder_top_oe),
.byte_in										(bchencoder_top_byte_out),

.ram_clr_vld								(ldpc_ram_clr_vld),// before byte_in Q_360 cycles but after ldpc_check output
//////////////////////////////////////////////////////////////
.oe													(ldpc_encoder_top_oe				),
.byte_out										(ldpc_encoder_top_byte_out  )
//////////////////////////////////////////////////////////////
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//reg	          											bchencoder_top_oe_1dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_1dly;
reg	          											bchencoder_top_oe_2dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_2dly;
reg	          											bchencoder_top_oe_3dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_3dly;
reg	          											bchencoder_top_oe_4dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_4dly;
reg	          											bchencoder_top_oe_5dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_5dly;
reg	          											bchencoder_top_oe_6dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_6dly;
reg	          											bchencoder_top_oe_7dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_7dly;
reg	          											bchencoder_top_oe_8dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_8dly;
reg	          											bchencoder_top_oe_9dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_9dly;
reg	          											bchencoder_top_oe_10dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_10dly;
reg	          											bchencoder_top_oe_11dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_11dly;
reg	          											bchencoder_top_oe_12dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_12dly;
reg	          											bchencoder_top_oe_13dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_13dly;
reg	          											bchencoder_top_oe_14dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_14dly;
reg	          											bchencoder_top_oe_15dly;
reg   			 		[7:0]     		bchencoder_top_byte_out_15dly;
reg	          								infor_frame_vld;
reg   			 		[7:0]     		infor_byte_in;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		  						bchencoder_top_oe_1dly  <= 1'b0;  
						bchencoder_top_byte_out_1dly  <= 8'h00;  
									bchencoder_top_oe_2dly  <= 1'b0;  
						bchencoder_top_byte_out_2dly  <= 8'h00;  
									bchencoder_top_oe_3dly  <= 1'b0;  
						bchencoder_top_byte_out_3dly  <= 8'h00;  
									bchencoder_top_oe_4dly  <= 1'b0;  
						bchencoder_top_byte_out_4dly  <= 8'h00;  
									bchencoder_top_oe_5dly  <= 1'b0;  
						bchencoder_top_byte_out_5dly  <= 8'h00;  
									bchencoder_top_oe_6dly  <= 1'b0;  
						bchencoder_top_byte_out_6dly  <= 8'h00;  
									bchencoder_top_oe_7dly  <= 1'b0;  
						bchencoder_top_byte_out_7dly  <= 8'h00;  
									bchencoder_top_oe_8dly  <= 1'b0;  
						bchencoder_top_byte_out_8dly  <= 8'h00;  
									bchencoder_top_oe_9dly  <= 1'b0;  
						bchencoder_top_byte_out_9dly  <= 8'h00;  
									bchencoder_top_oe_10dly <= 1'b0; 
						bchencoder_top_byte_out_10dly <= 8'h00; 
									bchencoder_top_oe_11dly <= 1'b0; 
						bchencoder_top_byte_out_11dly <= 8'h00; 
									bchencoder_top_oe_12dly <= 1'b0; 
						bchencoder_top_byte_out_12dly <= 8'h00; 
									bchencoder_top_oe_13dly <= 1'b0; 
						bchencoder_top_byte_out_13dly <= 8'h00; 
									bchencoder_top_oe_14dly <= 1'b0; 
						bchencoder_top_byte_out_14dly <= 8'h00; 
									bchencoder_top_oe_15dly <= 1'b0; 
						bchencoder_top_byte_out_15dly <= 8'h00; 
						infor_frame_vld 							<= 1'b0	;               
						infor_byte_in 								<= 8'h00;                 
	end
	else if(fs_en)begin
									bchencoder_top_oe_1dly  <= bchencoder_top_oe;  
						bchencoder_top_byte_out_1dly  <= bchencoder_top_byte_out;  
									bchencoder_top_oe_2dly  <= 			bchencoder_top_oe_1dly;    
						bchencoder_top_byte_out_2dly  <= bchencoder_top_byte_out_1dly ;  
									bchencoder_top_oe_3dly  <= 			bchencoder_top_oe_2dly;    
						bchencoder_top_byte_out_3dly  <= bchencoder_top_byte_out_2dly ;  
									bchencoder_top_oe_4dly  <= 			bchencoder_top_oe_3dly;    
						bchencoder_top_byte_out_4dly  <= bchencoder_top_byte_out_3dly ;  
									bchencoder_top_oe_5dly  <= 			bchencoder_top_oe_4dly;    
						bchencoder_top_byte_out_5dly  <= bchencoder_top_byte_out_4dly ;  
									bchencoder_top_oe_6dly  <= 			bchencoder_top_oe_5dly;    
						bchencoder_top_byte_out_6dly  <= bchencoder_top_byte_out_5dly ;  
									bchencoder_top_oe_7dly  <= 			bchencoder_top_oe_6dly;    
						bchencoder_top_byte_out_7dly  <= bchencoder_top_byte_out_6dly ;  
									bchencoder_top_oe_8dly  <= 			bchencoder_top_oe_7dly;    
						bchencoder_top_byte_out_8dly  <= bchencoder_top_byte_out_7dly ;  
									bchencoder_top_oe_9dly  <= 			bchencoder_top_oe_8dly;    
						bchencoder_top_byte_out_9dly  <= bchencoder_top_byte_out_8dly ;  
									bchencoder_top_oe_10dly <= 			bchencoder_top_oe_9dly;   
						bchencoder_top_byte_out_10dly <= bchencoder_top_byte_out_9dly ; 
									bchencoder_top_oe_11dly <= 			bchencoder_top_oe_10dly;  
						bchencoder_top_byte_out_11dly <= bchencoder_top_byte_out_10dly; 
									bchencoder_top_oe_12dly <= 			bchencoder_top_oe_11dly;  
						bchencoder_top_byte_out_12dly <= bchencoder_top_byte_out_11dly; 
									bchencoder_top_oe_13dly <= 			bchencoder_top_oe_12dly;  
						bchencoder_top_byte_out_13dly <= bchencoder_top_byte_out_12dly; 
									bchencoder_top_oe_14dly <= 			bchencoder_top_oe_13dly;  
						bchencoder_top_byte_out_14dly <= bchencoder_top_byte_out_13dly; 
									bchencoder_top_oe_15dly <= 			bchencoder_top_oe_14dly;  
						bchencoder_top_byte_out_15dly <= bchencoder_top_byte_out_14dly; 
						infor_frame_vld 							<= 			bchencoder_top_oe_15dly;               
						infor_byte_in 								<= bchencoder_top_byte_out_15dly;  
	end
	else begin
	end
end

ldpcencoder_mux uut_ldpcencoder_mux(///////////////////// will check
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.infor_frame_vld					(infor_frame_vld),
.infor_byte_in						(infor_byte_in	),
.check_frame_vld					(ldpc_encoder_top_oe),
.check_byte_in						(ldpc_encoder_top_byte_out),
//////////////////////////////////////////////////////////////
.oe												(ldpcencoder_mux_oe	     ),
.byte_out									(ldpcencoder_mux_byte_out)
//////////////////////////////////////////////////////////////
);


BitInterLeaver uut_BitInterLeaver(// Module delay: 7 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.mod_mode									(mod_mode),
.frame_mode								(frame_mode),
.except_order							((ldpc_mode == 4'h4)&&(mod_mode == 2'b01)),
.frame_vld								(ldpcencoder_mux_oe),
.byte_in									(ldpcencoder_mux_byte_out),
.bit_itl_read_head				(bit_itl_read_head),
.bit_itl_read_en					(bit_itl_read_en),

.oe												(BitInterLeaver_oe),
.symbol_out								(BitInterLeaver_symbol_out)
);


mapper uut_mapper(// Module delay: 3 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.mod_mode									(mod_mode),
.ldpc_mode									(ldpc_mode),
.init_vld									(1'b0),
.frame_vld								(BitInterLeaver_oe),
.byte_in									(BitInterLeaver_symbol_out),
//////////////////////////////////////////////////////////////
.oe												(mapper_oe          ),
.symbol_re_out						(mapper_symbol_re_out),
.symbol_im_out						(mapper_symbol_im_out)
//////////////////////////////////////////////////////////////
);

//wire																	genpilot_oe;
//wire							signed	[15:0]			genpilot_symbol_re_out;
//wire							signed	[15:0]			genpilot_symbol_im_out;

reg																		genpilot_oe_1dly;
reg								signed	[15:0]			genpilot_symbol_re_out_1dly;
reg								signed	[15:0]			genpilot_symbol_im_out_1dly;

reg																		genpilot_oe_2dly;
reg								signed	[15:0]			genpilot_symbol_re_out_2dly;
reg								signed	[15:0]			genpilot_symbol_im_out_2dly;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		  genpilot_oe_1dly						<= 1'b0;           
			genpilot_symbol_re_out_1dly <= 16'h0000;
			genpilot_symbol_im_out_1dly <= 16'h0000;
			                           
			genpilot_oe_2dly						<= 1'b0;           
			genpilot_symbol_re_out_2dly <= 16'h0000;
			genpilot_symbol_im_out_2dly <= 16'h0000;
	end
	else if(fs_en)begin
			genpilot_oe_1dly						<= genpilot_oe;                  
			genpilot_symbol_re_out_1dly <= genpilot_symbol_re_out;
			genpilot_symbol_im_out_1dly <= genpilot_symbol_im_out;
			                           
			genpilot_oe_2dly						<= genpilot_oe_1dly;                  
			genpilot_symbol_re_out_2dly <= genpilot_symbol_re_out_1dly;
			genpilot_symbol_im_out_2dly <= genpilot_symbol_im_out_1dly;
	end
	else begin
	end
end

reg													pl_pilot_vld_1dly;
reg													pl_pilot_vld_2dly;
reg													pl_pilot_vld_3dly;
reg													pl_pilot_vld_4dly;
reg													pl_pilot_vld_5dly;
reg													pl_pilot_vld_6dly;
reg													pl_pilot_vld_align;     

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		pl_pilot_vld_1dly  <= 1'b0; 
		pl_pilot_vld_2dly  <= 1'b0; 
		pl_pilot_vld_3dly  <= 1'b0; 
		pl_pilot_vld_4dly  <= 1'b0; 
		pl_pilot_vld_5dly  <= 1'b0; 
		pl_pilot_vld_6dly  <= 1'b0; 
		pl_pilot_vld_align <= 1'b0;
	end
	else if(fs_en)begin
		pl_pilot_vld_1dly  <= pl_pilot_vld; 
		pl_pilot_vld_2dly  <= pl_pilot_vld_1dly; 
		pl_pilot_vld_3dly  <= pl_pilot_vld_2dly; 
		pl_pilot_vld_4dly  <= pl_pilot_vld_3dly; 
		pl_pilot_vld_5dly  <= pl_pilot_vld_4dly; 
		pl_pilot_vld_6dly  <= pl_pilot_vld_5dly; 
		pl_pilot_vld_align <= pl_pilot_vld_6dly;
	end
	else begin
	end
end

genpilot uut_genpilot(// Module delay: 1 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.frame_vld									(pl_pilot_vld_align),
.oe													(genpilot_oe          ),
.symbol_re_out							(genpilot_symbol_re_out),
.symbol_im_out							(genpilot_symbol_im_out)
);

reg																	genplheader_oe_1dly;
reg							signed	[15:0]			genplheader_symbol_re_out_1dly;
reg							signed	[15:0]			genplheader_symbol_im_out_1dly;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		           genplheader_oe_1dly  <= 1'b0; 
		genplheader_symbol_re_out_1dly <= 16'sh0000;
		genplheader_symbol_im_out_1dly <= 16'sh0000;
	end
	else if(fs_en)begin
							 genplheader_oe_1dly <= genplheader_oe					 ; 
		genplheader_symbol_re_out_1dly <= genplheader_symbol_re_out;
		genplheader_symbol_im_out_1dly <= genplheader_symbol_im_out;
	end
	else begin
	end
end

reg													xfec_ready_1dly;
reg													xfec_ready_2dly;
reg													xfec_ready_3dly;
reg													xfec_ready_4dly;
reg													xfec_ready_align;     

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		xfec_ready_1dly  <= 1'b0; 
		xfec_ready_2dly  <= 1'b0;
		xfec_ready_3dly  <= 1'b0;
		xfec_ready_4dly  <= 1'b0;
		xfec_ready_align <= 1'b0;
	end
	else if(fs_en)begin
		xfec_ready_1dly  <= xfec_ready; 
		xfec_ready_2dly  <= xfec_ready_1dly; 
		xfec_ready_3dly  <= xfec_ready_2dly; 
		xfec_ready_4dly  <= xfec_ready_3dly; 
		xfec_ready_align <= xfec_ready_4dly;
	end
	else begin
	end
end

reg													pl_header_vld_1dly;
reg													pl_header_vld_2dly;
reg													pl_header_vld_3dly;
reg													pl_header_vld_4dly;
reg													pl_header_vld_5dly;
reg													pl_header_vld_6dly;
reg													pl_header_vld_align;     

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		pl_header_vld_1dly  <= 1'b0; 
		pl_header_vld_2dly  <= 1'b0; 
		pl_header_vld_3dly  <= 1'b0; 
		pl_header_vld_4dly  <= 1'b0; 
		pl_header_vld_5dly  <= 1'b0; 
		pl_header_vld_6dly  <= 1'b0; 
		pl_header_vld_align <= 1'b0;
	end
	else if(fs_en)begin
		pl_header_vld_1dly  <= pl_header_vld; 
		pl_header_vld_2dly  <= pl_header_vld_1dly; 
		pl_header_vld_3dly  <= pl_header_vld_2dly; 
		pl_header_vld_4dly  <= pl_header_vld_3dly; 
		pl_header_vld_5dly  <= pl_header_vld_4dly; 
		pl_header_vld_6dly  <= pl_header_vld_5dly;
		pl_header_vld_align <= pl_header_vld_6dly;
	end
	else begin
	end
end

reg													plheader_head_1dly;
reg													plheader_head_2dly;
reg													plheader_head_3dly;
reg													plheader_head_4dly;
reg													plheader_head_5dly;
reg													plheader_head_6dly;
reg													plheader_head_align;     

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		plheader_head_1dly  <= 1'b0; 
		plheader_head_2dly  <= 1'b0; 
		plheader_head_3dly  <= 1'b0; 
		plheader_head_4dly  <= 1'b0; 
		plheader_head_5dly  <= 1'b0; 
		plheader_head_6dly  <= 1'b0; 
		plheader_head_align <= 1'b0;
	end
	else if(fs_en)begin
		plheader_head_1dly  <= plheader_head; 
		plheader_head_2dly  <= plheader_head_1dly; 
		plheader_head_3dly  <= plheader_head_2dly; 
		plheader_head_4dly  <= plheader_head_3dly; 
		plheader_head_5dly  <= plheader_head_4dly; 
		plheader_head_6dly  <= plheader_head_5dly; 
		plheader_head_align <= plheader_head_6dly;
	end
	else begin
	end
end


genplheader uut_genplheader(// Module delay: 2 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.frame_vld								(pl_header_vld_align),
.frame_head								(plheader_head_align),
.xfec_ready								(xfec_ready_align),// must earlier 2 cycles than frame_head
.mod_mode										(mod_mode),    
.ldpc_mode									(ldpc_mode), 
.frame_mode									(frame_mode),
.pilot_mode									(pilot_mode),

.oe													(genplheader_oe          ),
.symbol_re_out							(genplheader_symbol_re_out),
.symbol_im_out							(genplheader_symbol_im_out)
);

reg													xfec_ready_align_1dly;
reg													xfec_ready_align_2dly;
reg													xfec_ready_align_3dly;
reg													xfec_ready_align_4dly;
reg													xfec_ready_align_5dly;

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		xfec_ready_align_1dly <= 1'b0; 
		xfec_ready_align_2dly <= 1'b0; 
		xfec_ready_align_3dly <= 1'b0; 
		xfec_ready_align_4dly <= 1'b0;
		xfec_ready_align_5dly <= 1'b0;
	end
	else if(fs_en)begin
		xfec_ready_align_1dly <= xfec_ready_align; 
		xfec_ready_align_2dly <= xfec_ready_align_1dly; 
		xfec_ready_align_3dly <= xfec_ready_align_2dly; 
		xfec_ready_align_4dly <= xfec_ready_align_3dly;
		xfec_ready_align_5dly <= xfec_ready_align_4dly;
	end
	else begin
	end
end

reg													null_vld_1dly ;
reg													null_vld_2dly ;
reg													null_vld_3dly ;
reg													null_vld_4dly ;
reg													null_vld_5dly ;
reg													null_vld_6dly ;
reg													null_vld_7dly ;
reg													null_vld_8dly ;
reg													null_vld_9dly ;

reg			  									null_vld_align;              // align with start_en_11dly


always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		null_vld_1dly  <= 1'b0; 
		null_vld_2dly  <= 1'b0; 
		null_vld_3dly  <= 1'b0; 
		null_vld_4dly  <= 1'b0;
		null_vld_5dly  <= 1'b0;
		null_vld_6dly  <= 1'b0;
		null_vld_7dly  <= 1'b0;
		null_vld_8dly  <= 1'b0;
		null_vld_9dly  <= 1'b0;
		null_vld_align <= 1'b0;
	end
	else if(fs_en)begin
		null_vld_1dly  <= null_vld; 
		null_vld_2dly  <= null_vld_1dly; 
		null_vld_3dly  <= null_vld_2dly; 
		null_vld_4dly  <= null_vld_3dly;
		null_vld_5dly  <= null_vld_4dly;
		null_vld_6dly  <= null_vld_5dly;
		null_vld_7dly  <= null_vld_6dly;
		null_vld_8dly  <= null_vld_7dly;
		null_vld_9dly  <= null_vld_8dly;
		null_vld_align <= null_vld_9dly;
	end
	else begin
	end
end

plframer uut_plframer(// Module delay: 1 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.xfec_frame_vld						(mapper_oe),
.pl_header_vld						(genplheader_oe_1dly),/////////ok
.pl_pilot_vld							(genpilot_oe_2dly),
.xfec_ready								(xfec_ready_align_5dly),/////////////////////???????????? 3 + earlier than frame_head
.null_vld											(null_vld_align),
.xfec_re_in										(mapper_symbol_re_out),
.xfec_im_in										(mapper_symbol_im_out),
.pl_header_re_in							(genplheader_symbol_re_out_1dly),//////ok
.pl_header_im_in							(genplheader_symbol_im_out_1dly),//////ok
.pl_pilot_re_in									(genpilot_symbol_re_out_2dly),
.pl_pilot_im_in									(genpilot_symbol_im_out_2dly),

.oe															(plframer_oe           ),
.symbol_re_out									(plframer_symbol_re_out),
.symbol_im_out									(plframer_symbol_im_out)
);


reg				  								plscramb_init_vld_1dly ;
reg				  								plscramb_init_vld_2dly ;
reg				  								plscramb_init_vld_3dly ;
reg				  								plscramb_init_vld_4dly ;
reg				  								plscramb_init_vld_5dly ;
reg				  								plscramb_init_vld_6dly ;
reg				  								plscramb_init_vld_7dly ;
reg				  								plscramb_init_vld_8dly ;
reg				  								plscramb_init_vld_9dly ;
reg				  								plscramb_init_vld_10dly ;
reg				  								plscramb_init_vld_align;     // align with start_en_12dly

reg				  								plscramb_vld_1dly ;
reg				  								plscramb_vld_2dly ;
reg				  								plscramb_vld_3dly ;
reg				  								plscramb_vld_4dly ;
reg				  								plscramb_vld_5dly ;
reg				  								plscramb_vld_6dly ;
reg				  								plscramb_vld_7dly ;
reg				  								plscramb_vld_8dly ;
reg				  								plscramb_vld_9dly ;
reg				  								plscramb_vld_10dly ;
reg				  								plscramb_vld_align;          // align with start_en_12dly

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		plscramb_init_vld_1dly  <= 1'b0; 
		plscramb_init_vld_2dly  <= 1'b0; 
		plscramb_init_vld_3dly  <= 1'b0; 
		plscramb_init_vld_4dly  <= 1'b0;
		plscramb_init_vld_5dly  <= 1'b0;
		plscramb_init_vld_6dly  <= 1'b0;
		plscramb_init_vld_7dly  <= 1'b0;
		plscramb_init_vld_8dly  <= 1'b0;
		plscramb_init_vld_9dly  <= 1'b0;
		plscramb_init_vld_10dly  <= 1'b0;
		plscramb_init_vld_align <= 1'b0;
	end
	else if(fs_en)begin
		plscramb_init_vld_1dly  <= plscramb_init_vld; 
		plscramb_init_vld_2dly  <= plscramb_init_vld_1dly; 
		plscramb_init_vld_3dly  <= plscramb_init_vld_2dly; 
		plscramb_init_vld_4dly  <= plscramb_init_vld_3dly;
		plscramb_init_vld_5dly  <= plscramb_init_vld_4dly;
		plscramb_init_vld_6dly  <= plscramb_init_vld_5dly;
		plscramb_init_vld_7dly  <= plscramb_init_vld_6dly;
		plscramb_init_vld_8dly  <= plscramb_init_vld_7dly;
		plscramb_init_vld_9dly  <= plscramb_init_vld_8dly;
		plscramb_init_vld_10dly  <= plscramb_init_vld_9dly;
		plscramb_init_vld_align <= plscramb_init_vld_10dly;
	end
	else begin
	end
end

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		plscramb_vld_1dly  <= 1'b0; 
		plscramb_vld_2dly  <= 1'b0; 
		plscramb_vld_3dly  <= 1'b0; 
		plscramb_vld_4dly  <= 1'b0;
		plscramb_vld_5dly  <= 1'b0;
		plscramb_vld_6dly  <= 1'b0;
		plscramb_vld_7dly  <= 1'b0;
		plscramb_vld_8dly  <= 1'b0;
		plscramb_vld_9dly  <= 1'b0;
		plscramb_vld_10dly  <= 1'b0;
		plscramb_vld_align <= 1'b0;
	end
	else if(fs_en)begin
		plscramb_vld_1dly  <= plscramb_vld; 
		plscramb_vld_2dly  <= plscramb_vld_1dly; 
		plscramb_vld_3dly  <= plscramb_vld_2dly; 
		plscramb_vld_4dly  <= plscramb_vld_3dly;
		plscramb_vld_5dly  <= plscramb_vld_4dly;
		plscramb_vld_6dly  <= plscramb_vld_5dly;
		plscramb_vld_7dly  <= plscramb_vld_6dly;
		plscramb_vld_8dly  <= plscramb_vld_7dly;
		plscramb_vld_9dly  <= plscramb_vld_8dly;
		plscramb_vld_10dly  <= plscramb_vld_9dly;
		plscramb_vld_align <= plscramb_vld_10dly;
	end
	else begin
	end
end


plscrambler uut_plscrambler(// Module delay: 3 cycles at clk_1x
.sys_clk										(sys_clk),
.fs_en										(fs_en),
.rst_n										(glb_rst_n),
//////////////////////////////////////////////////////////////
.init_vld									(plscramb_init_vld_align),
.scrambler_vld								(plscramb_vld_align),
.frame_vld									(plframer_oe           ),
.re_in										(plframer_symbol_re_out),
.im_in										(plframer_symbol_im_out),

.oe											(oe			  ),
.symbol_re_out								(symbol_re_out),
.symbol_im_out								(symbol_im_out)
);

wire      signed  [15:0]        symbol_re_out_tmp;
wire      signed  [15:0]        symbol_im_out_tmp;
wire                            symbol_1x_oe_tmp;

assign symbol_1x_oe_tmp = oe;
assign symbol_re_out_tmp = symbol_re_out;
assign symbol_im_out_tmp = symbol_im_out;

//assign symbol_1x_re_out = (Freq_Inv_mode == 0)?($signed({symbol_re_out_tmp[15],symbol_re_out_tmp[13:0],1'b0})):($signed({symbol_im_out_tmp[15],symbol_im_out_tmp[13:0],1'b0}));
//assign symbol_1x_im_out = (Freq_Inv_mode == 0)?($signed({symbol_im_out_tmp[15],symbol_im_out_tmp[13:0],1'b0})):($signed({symbol_re_out_tmp[15],symbol_re_out_tmp[13:0],1'b0}));

always @(posedge sys_clk)begin
	if(~glb_rst_n)begin
		symbol_1x_oe <= 1'b0;
		symbol_1x_re_out <= 16'sh0000;
		symbol_1x_im_out <= 16'sh0000;
	end
	else if(fs_en == 1'b1)begin
	   symbol_1x_oe <= symbol_1x_oe_tmp;
		if(Freq_Inv_mode == 1'b0)begin
		  symbol_1x_re_out <= ($signed({symbol_re_out_tmp[15],symbol_re_out_tmp[13:0],1'b0}));
          symbol_1x_im_out <= ($signed({symbol_im_out_tmp[15],symbol_im_out_tmp[13:0],1'b0}));
		end
		else begin
		  symbol_1x_re_out <= ($signed({symbol_im_out_tmp[15],symbol_im_out_tmp[13:0],1'b0}));
          symbol_1x_im_out <= ($signed({symbol_re_out_tmp[15],symbol_re_out_tmp[13:0],1'b0}));
		end
	end
	else begin
	   symbol_1x_oe <= 1'b0;
       symbol_1x_re_out <= 16'sh0000;
       symbol_1x_im_out <= 16'sh0000;
	end
end



//dvb_s2_srrc_filter uut_dvb_s2_srrc_filter(
//.hard_rst_n									(glb_rst_n),// modified by 2014.09.22
//////////////////////////////////////////////////////////////////////
//.sys_clk										(sys_clk),
//////////////////////////////////////////////////////////////////////
//.symbol_1x_oe							(symbol_1x_oe),
//.symbol_1x_re_out						(symbol_1x_re_out),
//.symbol_1x_im_out						(symbol_1x_im_out),

//.symbol_2x_oe								(symbol_2x_oe),
//.symbol_2x_re_out							(symbol_2x_re_out),
//.symbol_2x_im_out							(symbol_2x_im_out)
//);

endmodule

