// -------------------------------------------------------------
// File Name: D:\altera\91\qdesigns\dvb_s2_system\dvb_s2_srrc_filter.v
// Created: 2013-01-19 09:20:30
// Generated by wan
// -------------------------------------------------------------
// -------------------------------------------------------------
// Module: dvb_s2_srrc_filter
// Hierarchy Level: 1
// -------------------------------------------------------------
// -------------------------------------------------------------
// Module delay: 4 cycles at clk_1x
// -------------------------------------------------------------

`timescale 1ns / 1ps
module dvb_s2_srrc_filter(
input	          							hard_rst_n,// modified by 2014.09.22
//////////////////////////////////////////////////////////////
input                  					sys_clk,
//////////////////////////////////////////////////////////////
input 									symbol_1x_oe,
input 		signed [15:0]			symbol_1x_re_out,
input 		signed [15:0]			symbol_1x_im_out,

output 									symbol_2x_oe,
output 		signed [15:0]			symbol_2x_re_out,
output 		signed [15:0]			symbol_2x_im_out
);

///////////////////////// Simulation bypass begin /////////////////////////////////
wire	signed	[39:0]	ast_source_data_re;
wire	signed	[39:0]	ast_source_data_im;

//srrc_filter_0p2 uut_srrc_filter_0p2_re(
//.clk							(sys_clk),
//.reset_n						(1'b1),//(hard_rst_n),
//.ast_sink_data				(symbol_1x_re_out),
//.ast_sink_valid			(symbol_1x_oe),
//.ast_source_ready			(hard_rst_n),//(1'b1),//(oe_2dly),
//.ast_sink_error			(),
//.ast_source_data			(ast_source_data_re),
//.ast_sink_ready			(),
//.ast_source_valid			(symbol_2x_oe),
//.ast_source_error			()
//);

fir_compiler_0 uut_srrc_filter_0p2_re (
  .aclk(sys_clk),                              // input wire aclk
  .s_axis_data_tvalid(symbol_1x_oe),  // input wire s_axis_data_tvalid
  .s_axis_data_tready(),  // output wire s_axis_data_tready
  .s_axis_data_tdata(symbol_1x_re_out),    // input wire [15 : 0] s_axis_data_tdata
  .m_axis_data_tvalid(symbol_2x_oe),  // output wire m_axis_data_tvalid
  .m_axis_data_tdata(ast_source_data_re)    // output wire [39 : 0] m_axis_data_tdata
);

//srrc_filter_0p2 uut_srrc_filter_0p2_im(
//.clk							(sys_clk),
//.reset_n						(1'b1),//(hard_rst_n),
//.ast_sink_data				(symbol_1x_im_out),
//.ast_sink_valid			(symbol_1x_oe),
//.ast_source_ready			(hard_rst_n),//(1'b1),//(oe_2dly),
//.ast_sink_error			(),
//.ast_source_data			(ast_source_data_im),
//.ast_sink_ready			(),
//.ast_source_valid			(),
//.ast_source_error			()
//);

fir_compiler_0 uut_srrc_filter_0p2_im (
  .aclk(sys_clk),                              // input wire aclk
  .s_axis_data_tvalid(symbol_1x_oe),  // input wire s_axis_data_tvalid
  .s_axis_data_tready(),  // output wire s_axis_data_tready
  .s_axis_data_tdata(symbol_1x_im_out),    // input wire [15 : 0] s_axis_data_tdata
  .m_axis_data_tvalid(),  // output wire m_axis_data_tvalid
  .m_axis_data_tdata(ast_source_data_im)    // output wire [39 : 0] m_axis_data_tdata
);

assign symbol_2x_re_out = $signed(ast_source_data_re[38:23]);
assign symbol_2x_im_out = $signed(ast_source_data_im[38:23]);
///////////////////////// Simulation bypass  end  /////////////////////////////////

endmodule
