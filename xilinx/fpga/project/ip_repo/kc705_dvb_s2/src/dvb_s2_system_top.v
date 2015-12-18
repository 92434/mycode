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
input							[31:0]		    SYS_Baud_Num,//32'd2500 --> 25M BaudRate   SYS_Baud_mode,// 00:10M; 01:25M; 
input											Freq_Inv_mode,// 0:不执行频谱翻转; 1:执行频谱翻转 通过交换I和Q发送基带信号翻转频谱，具体地：Im=sin(ωmt) 及Qm=cos(ωmt);
//////////////////////////////////////////////////////////////
input                  					ts_clk_h264out,// clock from h.264 encoder
input							[7:0]       ts_din_h264out,// @ ts_clk_out
input                  					ts_syn_h264out,// @ ts_clk_out ts stream head
input                  					ts_valid_h264out,// @ ts_clk_out
//////////////////////////////////////////////////////////////
input                  					sys_clk,
//input							[7:0]       ts_din,// @ sys_clk
//input                  					ts_syn,// @ sys_clk
//input                  					ts_head,// @ sys_clk
output               					ts_clk,// @ sys_clk
input                 					fs_en_switch,
input                 					fs_en_outer,
input                					fs_en2_outer,
//////////////////////////////////////////////////////////////
output		reg				[7:0]       ts_din,// @ sys_clk
output      reg         					ts_syn,// @ sys_clk
output      reg         					ts_head,// @ sys_clk
//////////////////////////////////////////////////////////////
output 		reg							symbol_1x_oe,
output 	    reg 	signed [15:0]			symbol_1x_re_out,
output 		reg    signed [15:0]			symbol_1x_im_out//,

//output 									symbol_2x_oe,
//output 		signed [15:0]			symbol_2x_re_out,
//output 		signed [15:0]			symbol_2x_im_out
);


endmodule

